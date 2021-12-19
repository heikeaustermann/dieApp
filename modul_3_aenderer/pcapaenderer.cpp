#include "pcapaenderer.h"
#include "../modul_0_werkzeugkasten/konfigleser.h"
#include "../modul_0_werkzeugkasten/removeSpaces.h"
#include "../modul_0_werkzeugkasten/stringexec.h"
#include "EthLayer.h"
#include "Packet.h"
#include "PcapFileDevice.h"
#include <math.h>
#include <random>
#include <time.h>

PcapAenderer::PcapAenderer(std::string eingabeKonfigOrt, std::string eingabeQuelldatei, std::string eingabeZieldatei) {
    konfigOrt = eingabeKonfigOrt;
    quelldatei = eingabeQuelldatei;
    zieldatei = eingabeZieldatei;
    // Mögliche Anpassungen: Variante ohne hin- und herkopieren und/ oder diese Namen noch in die Konfigdatei
    zwischenspeicherdatei = "zwischenspeicher.pcap";
    kopierspeicherdatei = "kopierspeicher.pcap";
    zwischenquelldatei = "zwischenquelle.pcap";
    zwischenzieldatei = "zwischenziel.pcap";
    Konfigleser konfigleser(konfigOrt);
    anzahlPositionen =(konfigleser.gebeFehlercode() != 0)?0:konfigleser.gebeAnzahlRauschen();
    srand(time(NULL));
}

bool PcapAenderer::aenderePcap() {
    if (anzahlPositionen == 0) {return true;}

    bool rueckgabe = false;
    uint32_t position = 1;

    std::string speicherquelle = quelldatei;
    std::string speicherziel = zieldatei;

    quelldatei = zwischenquelldatei;
    zieldatei = zwischenzieldatei;

    std::string befehl("cp ");
    befehl.append(speicherquelle).append(" ").append(quelldatei);
    const char *zeile = befehl.c_str();
    std::string ausgabe = Stringexec::exec(zeile);

    while (position < anzahlPositionen+1) {
        rueckgabe = aenderePcap(position);
        if (rueckgabe) {
            position++;
            befehl = "rm -f ";
            befehl.append(quelldatei);
            ausgabe = Stringexec::exec(zeile);
            befehl = "mv ";
            befehl.append(zieldatei).append(" ").append(quelldatei);
            ausgabe = Stringexec::exec(zeile);}
        else {position = anzahlPositionen+1;}
    }

    befehl = "mv ";
    befehl.append(quelldatei).append(" ").append(speicherziel);
    ausgabe = Stringexec::exec(zeile);

    quelldatei = speicherquelle;
    zieldatei = speicherziel;
    return true;
}

bool PcapAenderer::aenderePcap(uint32_t position) {
    Konfigleser konfigleser(konfigOrt);
    if (konfigleser.gebeFehlercode() != 0) {return false;}
    starttime = konfigleser.gebeRauschenTimestamp(position);
    endtime = konfigleser.gebeRauschenEndTimestamp(position);
    rauschenWahrscheinlichkeit = konfigleser.gebeRauschenWahrscheinlichkeit(position);
    rauschenZeitverschiebung = konfigleser.gebeRauschenZeitverschiebung(position);
    Rauschenbetroffene betroffene = konfigleser.gebeRauschenBetroffene(position);
    if (betroffene == Rauschenbetroffene::UNDEFINED) {return false;}
    switch (betroffene) {
        case Rauschenbetroffene::FROMGROUPTOGROUP:
            fromall = false;
            toall = false;
            break;
        case Rauschenbetroffene::FROMGROUPTOALL:
            fromall = false;
            toall = true;
            break;
        case Rauschenbetroffene::FROMALLTOGROUP:
            fromall = true;
            toall = false;
            break;
        case Rauschenbetroffene::FROMALLTOALL:
            fromall = true;
            toall = true;
            break;
        default:
            fromall = true;
            toall = true; 
            break; 
    }
    Aenderungsmodus aenderungsmodus = konfigleser.gebeRauschenVerfahren(position);
    if (aenderungsmodus == Aenderungsmodus::UNDEFINED) {return false;}
    switch (aenderungsmodus) {
        case Aenderungsmodus::DELETE:
            keeppacket = false;
            shiftpacket = false;
            break;
        case Aenderungsmodus::SHIFT:
            keeppacket = false;
            shiftpacket = true;
            break;
        case Aenderungsmodus::COPY:
            keeppacket = true;
            shiftpacket = true;
            break;
        default:
            keeppacket = true;
            shiftpacket = false;
            break;
    }

    anzahlQuellen = 1;
    anzahlZiele = 1;
    macQuellen.clear();
    macZiele.clear();
    if (!fromall) {
        while (konfigleser.gebeRauschenQuellMAC(position,anzahlQuellen) != "") {
            macQuellen.insert(macQuellen.end(),RemoveSpaces::removeSpaces(konfigleser.gebeRauschenQuellMAC(position,anzahlQuellen)));
            anzahlQuellen++;
        }
        anzahlQuellen--;
    }
    if (!toall) {
        while (konfigleser.gebeRauschenZielMAC(position,anzahlZiele) != "") {
            macZiele.insert(macZiele.end(),RemoveSpaces::removeSpaces(konfigleser.gebeRauschenZielMAC(position,anzahlZiele)));
            anzahlZiele++;
        }
        anzahlZiele--;
    }

    pcpp::PcapFileReaderDevice leser(quelldatei);
	leser.open();
    pcpp::PcapFileWriterDevice schreiber(zieldatei);
	schreiber.open();
    pcpp::PcapFileWriterDevice zwischenschreiber(zwischenspeicherdatei);
    pcpp::PcapFileReaderDevice zwischenleser(zwischenspeicherdatei);
    pcpp::PcapFileWriterDevice kopierschreiber(kopierspeicherdatei);
    pcpp::PcapFileReaderDevice kopierleser(kopierspeicherdatei);
    if (shiftpacket) {
        zwischenschreiber.open();
        zwischenschreiber.close();
    }

    struct timespec gelesenZeit;
    long long lesezeit;
    std::string macs;
    std::string macd;
    pcpp::RawPacket rawPacket;
    pcpp::Packet parsedPacket(&rawPacket);
    pcpp::EthLayer* ethernetLayer = parsedPacket.getLayerOfType<pcpp::EthLayer>();
    bool gelesen = leser.getNextPacket(rawPacket);
    bool uebernehmePaket = true;
    bool pruefQuelle = true;
    bool pruefZiel = true;
    uint32_t zaehler = 0;
    double vergleich;
    bool zwischenpakete = false;
    zwischenzeiten.clear();
    long long zwischenzeitvergleich=0;
    std::multiset<long long>::iterator zeitschritte;
    pcpp::RawPacket zwischenRawPacket;
    pcpp::Packet zwischenparsedPacket(&zwischenRawPacket);
    bool zwischengelesen;
    long long zwischenlesezeit;
    uint32_t zeitshift;
    int durchgang = 0;
    while (gelesen == true) { 
        durchgang++;
        parsedPacket = &rawPacket;
        gelesenZeit = parsedPacket.getRawPacket()->getPacketTimeStamp();
        lesezeit = floor(gelesenZeit.tv_nsec/1000)+gelesenZeit.tv_sec*1000000;

        // folgender if-Block enthalten da ggf falsche Reihenfolge im pcap
        // Vorschlag: noch aus- oder einschaltbar in Konfig, da Ressourcenverbrauch tendenziell hoch
        if (shiftpacket && zwischenpakete) {
            zwischenzeitvergleich = *zwischenzeiten.begin();
            while (zwischenzeitvergleich < lesezeit && zwischenpakete) {
                kopierschreiber.open();
                zwischenleser.open();
                zwischengelesen = zwischenleser.getNextPacket(zwischenRawPacket);
                while (zwischengelesen) {
                    zwischenparsedPacket = &zwischenRawPacket;
                    gelesenZeit = zwischenparsedPacket.getRawPacket()->getPacketTimeStamp();
                    zwischenlesezeit = floor(gelesenZeit.tv_nsec/1000)+gelesenZeit.tv_sec*1000000;
                    if (zwischenlesezeit == zwischenzeitvergleich) {
                        schreiber.writePacket(zwischenRawPacket);
                    } else {
                        kopierschreiber.writePacket(zwischenRawPacket);
                    }
                    zwischengelesen = zwischenleser.getNextPacket(zwischenRawPacket);
                }
                zwischenleser.close();
                kopierschreiber.close();

                zwischenschreiber.open();
                kopierleser.open();
                zwischengelesen = kopierleser.getNextPacket(zwischenRawPacket);
                while (zwischengelesen) {
                    zwischenschreiber.writePacket(zwischenRawPacket);
                    zwischengelesen = kopierleser.getNextPacket(zwischenRawPacket);
                }
                kopierleser.close();
                zwischenschreiber.close();

                zwischenzeiten.erase(zwischenzeiten.begin());
                if (zwischenzeiten.empty()) {
                    zwischenpakete = false;
                } else {
                    zwischenzeitvergleich = *zwischenzeiten.begin();
                }
            }
        }

        ethernetLayer = parsedPacket.getLayerOfType<pcpp::EthLayer>();      
        if (lesezeit < endtime && lesezeit >= starttime && ethernetLayer != NULL) {
            macs = RemoveSpaces::removeSpaces(ethernetLayer->getSourceMac().toString());
            macd = RemoveSpaces::removeSpaces(ethernetLayer->getDestMac().toString());

            if (fromall) {
                pruefQuelle = false;
            } else {
                zaehler = 1;
                while (zaehler < anzahlQuellen+1) {
                    if (macs.compare(macQuellen[zaehler-1]) == 0) {
                        pruefQuelle = false;
                    }
                    zaehler++;
                }
            }

            if (toall) {
                pruefZiel = false;
            } else {
                zaehler = 1;
                while (zaehler < anzahlZiele+1) {
                    if (macd.compare(macZiele[zaehler-1]) == 0) {
                        pruefZiel = false;
                    }
                    zaehler++;
                }
            } 

            if (!pruefQuelle && !pruefZiel) {
                vergleich =(double) (rand()%1000)/1000;
                if (vergleich<rauschenWahrscheinlichkeit) {
                    uebernehmePaket = false;
                }
            }         

            pruefQuelle = true;
            pruefZiel = true;
        }

        if (uebernehmePaket == true || keeppacket == true) {
            schreiber.writePacket(rawPacket);
        }

        if (uebernehmePaket == false && shiftpacket == true) {
            zeitshift = (rauschenZeitverschiebung==0)?0:rand()%rauschenZeitverschiebung;
            lesezeit = lesezeit + zeitshift;
            zeitstempel.tv_sec = lesezeit/1000000;
            zeitstempel.tv_usec = lesezeit%1000000;
            parsedPacket.getRawPacket()->setPacketTimeStamp(zeitstempel);
            parsedPacket.computeCalculateFields();
            // folgende Zeile ersetzt da ggf falsche Reihenfolge im pcap
            // aus- oder einschaltbar in Konfig, da Ressourcenverbrauch tendenziell hoch?
            // schreiber.writePacket(*parsedPacket.getRawPacket());
            zwischenzeiten.insert(lesezeit);
            zwischenpakete = true;
            zwischenschreiber.open(true);
            zwischenschreiber.writePacket(*parsedPacket.getRawPacket());
            zwischenschreiber.close();
        }

        gelesen = leser.getNextPacket(rawPacket);
        uebernehmePaket = true;
    }

    // ggf Rest in zwischenspeicher noch herausschreiben

    leser.close();
    schreiber.close();

    // Dateien kopierspeicher und zwischenspeicher noch löschen

    std::string befehl = "rm -f ";
    befehl.append(kopierspeicherdatei);
    const char *zeile = befehl.c_str();
    std::string ausgabe = Stringexec::exec(zeile);
    befehl = "rm -f ";
    befehl.append(zwischenspeicherdatei);
    ausgabe = Stringexec::exec(zeile);

    return true;
}