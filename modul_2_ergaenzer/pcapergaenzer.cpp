#include "pcapergaenzer.h"
#include "../modul_0_werkzeugkasten/konfigleser.h"
#include "../modul_0_werkzeugkasten/stringexec.h"


PcapErgaenzer::PcapErgaenzer(std::string eingabeKonfigOrt, std::string eingabeQuelldatei, std::string eingabeZieldatei)
: konfigOrt(eingabeKonfigOrt), quelldatei(eingabeQuelldatei), zieldatei(eingabeZieldatei) {
    // Diese Namen könnten in die Konfigdatei
    zwischenspeicherdatei = "zwischenspeicher.pcap";
    kopierspeicherdatei = "kopierspeicher.pcap";
    zwischenquelldatei = "zwischenquelle.pcap";
    zwischenzieldatei = "zwischenziel.pcap";
    Konfigleser konfigleser(konfigOrt);
    anzahlPositionen =(konfigleser.gebeFehlercode() != 0)?0:konfigleser.gebeAnzahlAnomalien();
}

bool PcapErgaenzer::ergaenzeAlle() {
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
        rueckgabe=ergaenzePcap(position);
        if (rueckgabe) {
            position++;
            befehl = "rm -f ";
            befehl.append(quelldatei);
            ausgabe = Stringexec::exec(zeile);
            befehl = "mv ";
            befehl.append(zieldatei).append(" ").append(quelldatei);
            ausgabe = Stringexec::exec(zeile);
        } else {
            position = anzahlPositionen+1;
        }
    }

    befehl = "mv ";
    befehl.append(quelldatei).append(" ").append(speicherziel);
    ausgabe = Stringexec::exec(zeile);

    quelldatei = speicherquelle;
    zieldatei = speicherziel;
    return rueckgabe;
}

bool PcapErgaenzer::ergaenzePcap(uint32_t position) {
    Konfigleser konfigleser(konfigOrt);
    if (konfigleser.gebeFehlercode() != 0) {return false;}

    Ergaenzungsmodus befehl = konfigleser.gebeAnomaliemethode(position);
    std::string aufruf;
    std::string ausgabe;
    std::string zusatzinfo;

    switch (befehl)
    {
    case Ergaenzungsmodus::PCAPSTEGO:
        aufruf = "python3 injector_bulk.py -r ";
        break;
    case Ergaenzungsmodus::ID2T:
        aufruf = "id2t ";
        break;
    case Ergaenzungsmodus::UNDEFINED:
        aufruf = "echo undefiniert in Quelle";
        break;
    default:
        aufruf = "echo undefiniert ";
        break;
    } 
    aufruf.append(quelldatei);
    switch (befehl)
    {
    case Ergaenzungsmodus::PCAPSTEGO:
        zusatzinfo = konfigleser.gebeAnomaliezusatzinfo(position);
        aufruf.append(" -a ").append(zusatzinfo);
        break;
    case Ergaenzungsmodus::ID2T:
        zusatzinfo = konfigleser.gebeAnomaliezusatzinfo(position);
        aufruf.append(" -i ").append(quelldatei).append(" -a ").append(zusatzinfo);
        break;
    case Ergaenzungsmodus::UNDEFINED:
        aufruf.append(" und Info: ");
        break;
    default:
        aufruf.append(" ");
        break;
    } 
    const char *zeile = aufruf.c_str();
    ausgabe = Stringexec::exec(zeile);

    switch (befehl)
    {
    case Ergaenzungsmodus::PCAPSTEGO:
        aufruf = "mv ";
        aufruf.append(zusatzinfo).append("_").append(quelldatei).append(" ").append(zieldatei);
        break;
    case Ergaenzungsmodus::ID2T:
        aufruf = "cp ";
        aufruf.append(quelldatei).append(" ").append(zieldatei);
        break;
    case Ergaenzungsmodus::UNDEFINED:
        aufruf = "cp ";
        aufruf.append(quelldatei).append(" ").append(zieldatei);
        break;
    default:
        aufruf = "cp ";
        aufruf.append(quelldatei).append(" ").append(zieldatei);
        break;
    } 
    const char *zeileumkopieren = aufruf.c_str();
    ausgabe = Stringexec::exec(zeileumkopieren);

    return true;
}