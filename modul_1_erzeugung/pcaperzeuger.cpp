#include "pcaperzeuger.h"
#include "intervallgeber.h"
#include "../modul_0_werkzeugkasten/hexbytes.h"
#include "../modul_0_werkzeugkasten/konfigleser.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include "SystemUtils.h"
#include "Packet.h"
#include "EthLayer.h"
#include "IPv6Layer.h"
#include "TcpLayer.h"
#include "PayloadLayer.h"
#include "PcapFileDevice.h"
#include "UdpLayer.h"
#include "DnsLayer.h"


PcapErzeuger::PcapErzeuger(std::string eingabeKonfigOrt, std::string eingabeZieldatei)
:konfigOrt(eingabeKonfigOrt), schreiber(pcpp::PcapFileWriterDevice(eingabeZieldatei, pcpp::LINKTYPE_ETHERNET))
{
    konfiggelesen = false;
    srand(time(NULL));
    // es werden auch hier keine Eingaben geprüft und bereinigt
    // wird keine zulässige eingabeZieldatei genutzt, kann schreiber nicht vernünftig initialisiert werden
}

bool PcapErzeuger::leseKonfig() {
    Konfigleser konfigleser(konfigOrt);
    if (konfigleser.gebeFehlercode() != 0) {return false;}

    // Setzen der globalen Eigenschaften
    
    maximalAnzahlPakete = konfigleser.gebeMaxPakete();
    startzeitpunkt = konfigleser.gebeTimestamp();
    endzeitpunkt = konfigleser.gebeEndTimestamp();
    intervallzwischenKetten = konfigleser.gebeIntervall();
    maximalAnzahlKetten = konfigleser.gebeMaxKetten();
    anzahlDefKetten = konfigleser.gebeAnzahlKetten(); 
    inklusiveLayer2 = konfigleser.gebeEinschliesslichLayer2();

    // Einlesen der Kettendefinitionen

    uint32_t intervall;
    uint32_t vergroesserungsfaktor;
    double vergroesserungswahrscheinlichkeit;
    uint32_t gewicht;
    Erzeugungsmethode methode;
    long long anfang;
    long long ende;
    uint32_t quellgruppenid;
    uint32_t zielgruppenid;
    std::vector<pcpp::IPv6Address> ipQuellen;
    std::vector<pcpp::IPv6Address> ipZiele;
    std::vector<pcpp::MacAddress> macQuellen;
    std::vector<pcpp::MacAddress> macZiele;
    std::vector<uint16_t> portQuellen; 
    uint32_t zaehlerQuellen;
    uint32_t zaehlerZiele;

    uint32_t kettennr = 1;
    while (kettennr < anzahlDefKetten+1) {
        zaehlerQuellen = 1;
        zaehlerZiele = 1;
        ipQuellen.clear();
        macQuellen.clear();
        ipZiele.clear();
        macZiele.clear();
        quellgruppenid = konfigleser.gebeQuellGruppenID(kettennr);
        zielgruppenid = konfigleser.gebeZielGruppenID(kettennr);
        while (konfigleser.gebeIP(quellgruppenid,zaehlerQuellen) != "")
        {
            ipQuellen.insert(ipQuellen.end(),konfigleser.gebeIP(quellgruppenid,zaehlerQuellen));
            macQuellen.insert(macQuellen.end(),konfigleser.gebeMAC(quellgruppenid,zaehlerQuellen));
            zaehlerQuellen++;
        }
        while (konfigleser.gebeIP(zielgruppenid,zaehlerZiele) != "")
        {
            ipZiele.insert(ipZiele.end(),konfigleser.gebeIP(zielgruppenid,zaehlerZiele));
            macZiele.insert(macZiele.end(),konfigleser.gebeMAC(zielgruppenid,zaehlerZiele));
            zaehlerZiele++;
        }
        ipQuellen.shrink_to_fit();
        macQuellen.shrink_to_fit();
        ipZiele.shrink_to_fit();
        macZiele.shrink_to_fit();

        anfang = startzeitpunkt + konfigleser.gebeStartverzoegerung(kettennr);
        ende = endzeitpunkt - konfigleser.gebeVerfruehtesende(kettennr);
        gewicht = konfigleser.gebeGewicht(kettennr);
        methode = konfigleser.gebeMethode(kettennr);
        intervall = konfigleser.gebeIntervall(kettennr);
        vergroesserungsfaktor = konfigleser.gebeVergroesserungsfaktor(kettennr);
        vergroesserungswahrscheinlichkeit = konfigleser.gebeVergroesserungswahrscheinlichkeit(kettennr);

        Kettendef kette(anfang,ende,gewicht,methode,intervall,vergroesserungsfaktor,vergroesserungswahrscheinlichkeit,ipQuellen,ipZiele,macQuellen,macZiele,zaehlerQuellen-1,zaehlerZiele-1);

        ketten.insert(ketten.end(),kette);

        kettennr++;
    }

    if (anzahlDefKetten != ketten.size()) {return false;}

    konfiggelesen = true;
    return true;
}

bool PcapErzeuger::schreibePcap() {
    if (!konfiggelesen) {if(!leseKonfig()){return false;}}
    
    initialeBefuellung();

    aktuelleZeit = *std::min_element(kettenzeiten.begin(),kettenzeiten.end());

    // pruefBool für die Schleife
    bool pruef = true;

    // Zähler für die beauftragten Pakete
    uint32_t paketnummer = 0;

    // Nummer der umzusetzenden Abfolge
    uint32_t abfolgenummer;

    // öffnen der Datei
	schreiber.open();

    while(pruef) {
        // bestimmen der nächsten Abfolge
        abfolgenummer = 0;
        while (kettenzeiten[abfolgenummer] != aktuelleZeit) {abfolgenummer++;}

        // schreiben des nächsten Pakets aus der Kette zur Abfolgenummer
        // verlassen der Schleife, falls Problem beim Schreiben des Pakets
        pruef = erstellePaket(abfolgenummer);

        paketnummer++;
        // verlassen der Schleife, falls Maximalanzahl geschriebener Paketen erreicht
        if (paketnummer == maximalAnzahlPakete) {pruef = false;}

        // nächsten Zeitstempel bestimmen
        kettenzeiten[abfolgenummer] = kettenzeitstempelgeber[abfolgenummer].gebeZeitstempel();

        // Zeit weitersetzen
        aktuelleZeit = *std::min_element(kettenzeiten.begin(),kettenzeiten.end());

        // ggf neue Kette zur Abfolgenummer
        if (kettenabfolgen[abfolgenummer].beendeteKette(aktuelleZeit)) {befuelle(abfolgenummer);}

        // verlassen der Schleife, falls Endzeitpunkt erreicht
        if (aktuelleZeit > endzeitpunkt) {pruef = false;}
    }
    
    // schließen der Datei
    schreiber.close();

    return true;
}

void PcapErzeuger::initialeBefuellung() {
    long long anfangszeit = startzeitpunkt;

    long long endzeitstempel;
    
    // initiale Befüllung der Ketten
    std::vector<uint32_t> kettengewichte;
    uint32_t summegewichte = 0;
    int zufall;
    uint32_t naechsteKette;
    for (uint32_t i = 0; i< anzahlDefKetten; i++) {kettengewichte.insert(kettengewichte.end(),0);}
    kettengewichte.shrink_to_fit();

    for (uint32_t i = 0; i<maximalAnzahlKetten; i++) {
        while ((summegewichte == 0) & (anfangszeit < endzeitpunkt)) {
            summegewichte = 0;
            for (uint32_t j = 0; j<anzahlDefKetten; j++) {kettengewichte[j] = 0;}
            for (uint32_t j = 0; j<anzahlDefKetten; j++) {
                if ((anfangszeit >= ketten[j].zeitraumanfang) & (anfangszeit <= ketten[j].zeitraumende)) {
                    kettengewichte[j] = ketten[j].gewicht;
                    summegewichte += ketten[j].gewicht;
                } 
            }
            anfangszeit += Intervallgeber::exponentiell(intervallzwischenKetten,2*intervallzwischenKetten);
        }
        kettenzeiten.insert(kettenzeiten.end(),anfangszeit);
        zufall = rand()%summegewichte + 1;
        naechsteKette = 0;
        while (zufall > 0) {
            zufall = zufall - ketten[naechsteKette].gewicht;
            naechsteKette++;
        }
        naechsteKette--;

        endzeitstempel = ketten[naechsteKette].zeitraumende;

        int quelle = rand()%ketten[naechsteKette].anzahlQuellen;
        ip1 = ketten[naechsteKette].ipQuellen[quelle];
        mac1 = ketten[naechsteKette].macQuellen[quelle];

        int ziel = rand()%ketten[naechsteKette].anzahlZiele;
        ip2 = ketten[naechsteKette].ipZiele[ziel];
        mac2 = ketten[naechsteKette].macZiele[ziel];

        if (ketten[naechsteKette].methode == Erzeugungsmethode::APPDATA)
        {
            port1 = 32768 + rand()%32768; // Vereinigung IANA-Empfehlung und Linux-Kernel
            port2 = 443; // service name: https
            Zeitstempelgeber zeitstempelgeber(anfangszeit,ketten[naechsteKette].intervall,ketten[naechsteKette].vergroesserungsfaktor,ketten[naechsteKette].vergroesserungswahrscheinlichkeit);
            kettenzeitstempelgeber.insert(kettenzeitstempelgeber.end(),zeitstempelgeber);
        } else {
            if (ketten[naechsteKette].methode == Erzeugungsmethode::DNS) {
                port1 = 32768 + rand()%32768; // Vereinigung IANA-Empfehlung und Linux-Kernel
                port2 = 53; // service name: domain
                Zeitstempelgeber zeitstempelgeber(anfangszeit,ketten[naechsteKette].intervall,ketten[naechsteKette].vergroesserungsfaktor);
                kettenzeitstempelgeber.insert(kettenzeitstempelgeber.end(),zeitstempelgeber);
            } else {
                port1 = 0;
                port2 = 0;
                Zeitstempelgeber zeitstempelgeber(anfangszeit,ketten[naechsteKette].intervall,ketten[naechsteKette].vergroesserungsfaktor);
                kettenzeitstempelgeber.insert(kettenzeitstempelgeber.end(),zeitstempelgeber);
            }
        }
        
        Abfolge abfolge(naechsteKette,endzeitstempel,ketten[naechsteKette].methode,0,inklusiveLayer2,mac1,mac2,ip1,ip2,port1,port2);
        kettenabfolgen.insert(kettenabfolgen.end(),abfolge);
        summegewichte = 0;
    }
    kettenzeiten.shrink_to_fit();
    kettenabfolgen.shrink_to_fit();
    kettenzeitstempelgeber.shrink_to_fit();
}

void PcapErzeuger::befuelle(uint32_t abfolgenummer) {
    long long anfangszeit = aktuelleZeit;
    std::vector<uint32_t> kettengewichte;
    uint32_t summegewichte = 0;
    int zufall;
    uint32_t naechsteKette;
    for (uint32_t i = 0; i< anzahlDefKetten; i++) {kettengewichte.insert(kettengewichte.end(),0);}
    kettengewichte.shrink_to_fit();

    // bestimmen der nächsten Anfangszeit
    while ((summegewichte == 0) & (anfangszeit < endzeitpunkt)) {
        summegewichte = 0;
        for (uint32_t j = 0; j<anzahlDefKetten; j++) {kettengewichte[j] = 0;}
        for (uint32_t j = 0; j<anzahlDefKetten; j++) {
            if ((anfangszeit >= ketten[j].zeitraumanfang) & (anfangszeit <= ketten[j].zeitraumende)) {
                kettengewichte[j] = ketten[j].gewicht;
                summegewichte += ketten[j].gewicht;
            } 
        }
        anfangszeit += Intervallgeber::exponentiell(intervallzwischenKetten,2*intervallzwischenKetten);
    }
    kettenzeiten[abfolgenummer] = anfangszeit;
    if (anfangszeit > endzeitpunkt) {return;}

    // bestimmen einer passenden Kette
    zufall = rand()%summegewichte + 1;
    naechsteKette = 0;
    while (zufall > 0) {
        zufall = zufall - ketten[naechsteKette].gewicht;
        naechsteKette++;
    }
    naechsteKette--;

    long long endzeitstempel = ketten[naechsteKette].zeitraumende;

    int quelle = rand()%ketten[naechsteKette].anzahlQuellen;
    ip1 = ketten[naechsteKette].ipQuellen[quelle];
    mac1 = ketten[naechsteKette].macQuellen[quelle];

    int ziel = rand()%ketten[naechsteKette].anzahlZiele;
    ip2 = ketten[naechsteKette].ipZiele[ziel];
    mac2 = ketten[naechsteKette].macZiele[ziel];

    if (ketten[naechsteKette].methode == Erzeugungsmethode::APPDATA)
    {
        port1 = 32768 + rand()%32768; // Vereinigung IANA-Empfehlung und Linux-Kernel
        port2 = 443; // service name: https
        Zeitstempelgeber zeitstempelgeber(anfangszeit,ketten[naechsteKette].intervall,ketten[naechsteKette].vergroesserungsfaktor,ketten[naechsteKette].vergroesserungswahrscheinlichkeit);
        kettenzeitstempelgeber.insert(kettenzeitstempelgeber.end(),zeitstempelgeber);
    } else {
        if (ketten[naechsteKette].methode == Erzeugungsmethode::DNS) {
            port1 = 32768 + rand()%32768; // Vereinigung IANA-Empfehlung und Linux-Kernel
            port2 = 53; // service name: domain
            Zeitstempelgeber zeitstempelgeber(anfangszeit,ketten[naechsteKette].intervall,ketten[naechsteKette].vergroesserungsfaktor);
            kettenzeitstempelgeber.insert(kettenzeitstempelgeber.end(),zeitstempelgeber);
        } else {
            port1 = 0;
            port2 = 0;
            Zeitstempelgeber zeitstempelgeber(anfangszeit,ketten[naechsteKette].intervall,ketten[naechsteKette].vergroesserungsfaktor);
            kettenzeitstempelgeber.insert(kettenzeitstempelgeber.end(),zeitstempelgeber);
        }
    }
    
    Abfolge abfolge(naechsteKette,endzeitstempel,ketten[naechsteKette].methode,0,inklusiveLayer2,mac1,mac2,ip1,ip2,port1,port2);
    kettenabfolgen[abfolgenummer] = abfolge;
}

bool PcapErzeuger::erstellePaket(uint32_t abfolgenummer) {
    bool pruef = true;
    mac1 = kettenabfolgen[abfolgenummer].macquelle;
    mac2 = kettenabfolgen[abfolgenummer].macziel;
    ip1 = kettenabfolgen[abfolgenummer].ipquelle;
    ip2 = kettenabfolgen[abfolgenummer].ipziel;
    port1 = kettenabfolgen[abfolgenummer].portquelle;
    port2 = kettenabfolgen[abfolgenummer].portziel;

    if (kettenabfolgen[abfolgenummer].methode == Erzeugungsmethode::APPDATA) {
        sequno = rand();
        ackno = rand();
        tcpack = true;
        tcpsyn = false;
        tcpfin= false;
        tcppsh = true;

        uint32_t laenge = 1+Intervallgeber::fraktalmaximalerwert(10,4000,2,20);
        payload.clear();
        payload.append("170303");
        payload.append(Hexbytes::hex2bytes(laenge));
        for (uint32_t j = 0; j<laenge; j++) {
            payload.append(Hexbytes::hexbyte(rand()%256));
        }
        pruef = schreibeTCPPaket();
    } else { 
        if (kettenabfolgen[abfolgenummer].methode == Erzeugungsmethode::DNS) {
            pruef = schreibeUDPPaket();
        } else {
            pruef = false;
        }
    }

    return pruef;
}

bool PcapErzeuger::schreibeTCPPaket() {
    // das Paket erstellen
    pcpp::Packet paket(100);

    // Setzen neuer Zeitstempel
    zeitstempel.tv_sec = aktuelleZeit/1000000;
    zeitstempel.tv_usec = aktuelleZeit%1000000;
    paket.getRawPacket()->setPacketTimeStamp(zeitstempel);

    // Layer für das Paket erstellen
    pcpp::EthLayer newEthernetLayer(mac1,mac2,PCPP_ETHERTYPE_IPV6);

	pcpp::IPv6Layer newIPv6Layer(ip1,ip2);
	newIPv6Layer.getIPv6Header()->hopLimit = 64;

    pcpp::TcpLayer newTcpLayer(port1,port2);
    newTcpLayer.getTcpHeader()->sequenceNumber = sequno;
    newTcpLayer.getTcpHeader()->ackNumber = ackno; 
    newTcpLayer.getTcpHeader()->ackFlag = tcpack;
    newTcpLayer.getTcpHeader()->synFlag = tcpsyn;
    newTcpLayer.getTcpHeader()->finFlag = tcpfin;
    newTcpLayer.getTcpHeader()->pshFlag = tcppsh;

    pcpp::PayloadLayer newPayloadLayer(payload);

    // das Paket anpassen
    paket.addLayer(&newEthernetLayer);
	paket.addLayer(&newIPv6Layer);
    paket.addLayer(&newTcpLayer);
    paket.addLayer(&newPayloadLayer);

    // Housekeeping
    paket.computeCalculateFields();

    // Paket schreiben
    schreiber.writePacket(*(paket.getRawPacket()));

    return true;
}

bool PcapErzeuger::schreibeUDPPaket() {
    // das Paket erstellen
    pcpp::Packet paket(100);

    // Setzen neuer Zeitstempel
    zeitstempel.tv_sec = aktuelleZeit/1000000;
    zeitstempel.tv_usec = aktuelleZeit%1000000;
    paket.getRawPacket()->setPacketTimeStamp(zeitstempel);

    // Layer für das Paket erstellen
    pcpp::EthLayer newEthernetLayer(mac1,mac2,PCPP_ETHERTYPE_IPV6);

	pcpp::IPv6Layer newIPv6Layer(ip1,ip2);
	newIPv6Layer.getIPv6Header()->hopLimit = 64;

    pcpp::UdpLayer newUdpLayer(port1, port2);

    pcpp::DnsLayer newDnsLayer;
	newDnsLayer.addQuery("www.fernuni-hagen.de", pcpp::DNS_TYPE_A, pcpp::DNS_CLASS_IN);

    // das Paket anpassen
    paket.addLayer(&newEthernetLayer);
	paket.addLayer(&newIPv6Layer);
	paket.addLayer(&newUdpLayer);
    paket.addLayer(&newDnsLayer);

    // Housekeeping
    paket.computeCalculateFields();

    // Paket schreiben
    schreiber.writePacket(*(paket.getRawPacket()));

    return true;
}