#include "konfigleser.h"
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;


Konfigleser::Konfigleser(string dateiOrt):xmlZugriff(&xmlKonfig) {
    // Achtung: es wird keinerlei Bereinigung des Dateiorts ausgeführt

    // Öffnen der Datei
    XMLDocument* doc = new XMLDocument();
	doc->LoadFile( &dateiOrt[0] );
    fehlercode = doc->ErrorID();

    // Einlesen nach xmlKonfig
    if (fehlercode == 0){
        FILE* xmlDatei = fopen( &dateiOrt[0] , "r");
        fseek(xmlDatei, 0, SEEK_END);
        long size = ftell(xmlDatei);
        if (size < 1) {
            fehlercode = -1;
        }
        fseek(xmlDatei, 0, SEEK_SET);
        char* dateiInhalt = new char[size +1];
        memset(dateiInhalt, 0xfe, size);
        size_t eingelesen = fread(dateiInhalt, 1, size, xmlDatei);
        if (uint32_t(eingelesen) < uint32_t(size) && fehlercode == 0) {
            fehlercode = -2;
        }
        fclose(xmlDatei);
        xmlKonfig.Parse(dateiInhalt);
        if (xmlKonfig.Error() == true && fehlercode == 0) {
            fehlercode = -3;
        }
        xmlZugriff = XMLHandle(&xmlKonfig);
    };
}

int Konfigleser::gebeFehlercode() {
    return fehlercode;
}


bool Konfigleser::gebeEinschliesslichLayer2() {
    XMLElement* einschliesslich2 = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("INCLUDELAYER2").ToElement();
    if (!einschliesslich2) {return true;}
    const char* einschliesslich = einschliesslich2->GetText();
    std::string inklusive(einschliesslich);
    bool ausgabe = (inklusive == "Y") || (inklusive == "YES") || (inklusive == "1");
    return ausgabe;
}


uint32_t Konfigleser::gebeAnzahlGruppen() {
    XMLElement* gruppe = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("GROUPS").FirstChildElement("GROUP").ToElement();
    uint32_t anzahl =0;
    while (gruppe) {
        anzahl++;
        gruppe = gruppe->NextSiblingElement("GROUP");
    }
    return anzahl;    
}

std::string Konfigleser::gebeGruppenname(uint32_t gruppenid) {
    XMLElement* gruppe = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("GROUPS").FirstChildElement("GROUP").ToElement();
    if (!gruppe) {return "";}
    uint32_t zaehler= 1;
    while (gruppe && zaehler != gruppenid) {
        gruppe = gruppe->NextSiblingElement("GROUP");
        if (!gruppe) {return "";}
        zaehler++;
    }
    XMLElement* grpname = gruppe->FirstChildElement("GROUPNAME");
    if (!grpname) {return "";}
    const char* grpnametext = grpname->GetText();
    std::string ausgabe(grpnametext);
    return ausgabe;    
}

uint32_t Konfigleser::gebeGruppennummer(std::string gruppenname) {
    XMLElement* gruppe = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("GROUPS").FirstChildElement("GROUP").ToElement();
    if (!gruppe) {return 0;}
    std::string ausgelesen = "";
    uint32_t zaehler=1;
    while (gruppe) {
        XMLElement* grpname = gruppe->FirstChildElement("GROUPNAME");
        if (!grpname) {return 0;}
        const char* grpnametext = grpname->GetText();
        std::string ausgabe(grpnametext);
        if (ausgabe == gruppenname){return zaehler;}
        zaehler ++;
        gruppe = gruppe->NextSiblingElement("GROUP");
    }
    return 0;
}

std::string Konfigleser::gebeMAC(uint32_t gruppenid, uint32_t quellid) {
    XMLElement* quelle = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("GROUPS").FirstChildElement("GROUP").ToElement();
    if (!quelle) {return "";}
    uint32_t zaehler = 1;
    while (quelle && zaehler != gruppenid) {
        quelle = quelle->NextSiblingElement("GROUP");
        if (!quelle) {return "";}
        zaehler++;
    }
    quelle = quelle->FirstChildElement("SRC");
    if (!quelle) {return "";}
    zaehler = 1;
    while (quelle && zaehler != quellid) {
        quelle = quelle->NextSiblingElement("SRC");
        if (!quelle) {return "";}
        zaehler++;
    }
    const char* adresse = quelle->FirstChildElement("MAC")->GetText();
    std::string ausgabe(adresse);
    return ausgabe;
}

std::string Konfigleser::gebeIP(uint32_t gruppenid, uint32_t quellid) {
    XMLElement* quelle = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("GROUPS").FirstChildElement("GROUP").ToElement();
    if (!quelle) {return "";}
    uint32_t zaehler=1;
    while (quelle && zaehler != gruppenid) {
        quelle = quelle->NextSiblingElement("GROUP");
        if (!quelle) {return "";}
        zaehler++;
    }
    quelle = quelle->FirstChildElement("SRC");
    if (!quelle) {return "";}
    zaehler=1;
    while (quelle && zaehler != quellid) {
        quelle = quelle->NextSiblingElement("SRC");
        if (!quelle) {return "";}
        zaehler++;
    }
    XMLElement* ipadr = quelle->FirstChildElement("IP");
    if (!ipadr) {return "";}
    const char* adresse = ipadr->GetText();
    std::string ausgabe(adresse);
    return ausgabe;
}


bool Konfigleser::ausfuehrenModul1() {
    XMLElement* ausfuehren = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("EXECUTE").ToElement();
    if (!ausfuehren) {return true;}
    const char* fuehreaus = ausfuehren->GetText();
    std::string inklusive(fuehreaus);
    bool ausgabe = (inklusive == "Y") || (inklusive == "YES") || (inklusive == "1");
    return ausgabe;
}

std::string Konfigleser::gebeNameModul1() {
    XMLElement* pcapname = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("NAME").ToElement();
    if (!pcapname) {return "";}
    const char* pcapbezeichner = pcapname->GetText();
    std::string ausgabe(pcapbezeichner);
    return ausgabe;    
}

uint32_t Konfigleser::gebeMaxPakete() {
    XMLElement* maxPakete = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("MAXNUMBER").ToElement();
    if (!maxPakete) {return 0;}
    const char* nummer = maxPakete->GetText();
    uint32_t ausgabe = atoi(nummer);
    return ausgabe;
}

long long Konfigleser::gebeTimestamp() {
    XMLElement* starttime = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("STARTTIME").ToElement();
    if (!starttime) {return -1;}
    const char* timestamp = starttime->GetText();
    long long zeitstempel = atoll(timestamp);
    return zeitstempel;
}

long long Konfigleser::gebeEndTimestamp() {
    XMLElement* endtime = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("ENDTIME").ToElement();
    if (!endtime) {return -1;}
    const char* timestamp = endtime->GetText();
    long long zeitstempel = atoll(timestamp);
    return zeitstempel;
}


uint32_t Konfigleser::gebeMaxKetten() {
    XMLElement* quelle = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("MAXCHAINS").ToElement();
    if (!quelle) {return 0;}
    const char* maxlinks = quelle->GetText();
    uint32_t ausgabe = atoi(maxlinks);
    return ausgabe;
}

uint32_t Konfigleser::gebeIntervall() {
    XMLElement* quelle = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("CHAINSTARTINTERVAL").ToElement();
    if (!quelle) {return 0;}
    const char* intervall = quelle->GetText();
    uint32_t ausgabe = atoi(intervall);
    return ausgabe;    
}

uint32_t Konfigleser::gebeAnzahlKetten() {
    XMLElement* kette = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("CHAINS").FirstChildElement("CHAIN").ToElement();
    uint32_t anzahl =0;
    while (kette) {
        anzahl++;
        kette = kette->NextSiblingElement("CHAIN");
    }
    return anzahl;
}

uint32_t Konfigleser::gebeGewicht(uint32_t kettenid) {
    XMLElement* quelle = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("CHAINS").FirstChildElement("CHAIN").ToElement();
    if (!quelle) {return 0;}
    uint32_t zaehler = 1;
    while (quelle && zaehler != kettenid) {
        quelle = quelle->NextSiblingElement("CHAIN");
        if (!quelle) {return 0;}
        zaehler++;
    }
    XMLElement* gewicht = quelle->FirstChildElement("WEIGHT");
    if (!gewicht) {return 0;}
    const char* weight = gewicht->GetText();
    uint32_t ausgabe = atoi(weight);
    return ausgabe;
}

uint32_t Konfigleser::gebeStartverzoegerung(uint32_t kettenid) {
    XMLElement* quelle = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("CHAINS").FirstChildElement("CHAIN").ToElement();
    if (!quelle) {return 0;}
    uint32_t zaehler = 1;
    while (quelle && zaehler != kettenid) {
        quelle = quelle->NextSiblingElement("CHAIN");
        if (!quelle) {return 0;}
        zaehler++;
    }
    XMLElement* zeit = quelle->FirstChildElement("LATESTART");
    if (!zeit) {return 0;}
    const char* verzoegerung = zeit->GetText();
    uint32_t ausgabe = atoi(verzoegerung);
    return ausgabe;
}

uint32_t Konfigleser::gebeVerfruehtesende(uint32_t kettenid) {
    XMLElement* quelle = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("CHAINS").FirstChildElement("CHAIN").ToElement();
    if (!quelle) {return 0;}
    uint32_t zaehler = 1;
    while (quelle && zaehler != kettenid) {
        quelle = quelle->NextSiblingElement("CHAIN");
        if (!quelle) {return 0;}
        zaehler++;
    }
    XMLElement* zeit = quelle->FirstChildElement("EARLYEND");
    if (!zeit) {return 0;}
    const char* frueher = zeit->GetText();
    uint32_t ausgabe = atoi(frueher);
    return ausgabe;
}

Erzeugungsmethode Konfigleser::gebeMethode(uint32_t kettenid) {
    XMLElement* quelle = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("CHAINS").FirstChildElement("CHAIN").ToElement();
    if (!quelle) {return Erzeugungsmethode::UNDEFINED;}
    uint32_t zaehler = 1;
    while (quelle && zaehler != kettenid) {
        quelle = quelle->NextSiblingElement("CHAIN");
        if (!quelle) {return Erzeugungsmethode::UNDEFINED;}
        zaehler++;
    }
    XMLElement* method = quelle->FirstChildElement("METHOD");
    if (!method) {return Erzeugungsmethode::UNDEFINED;}
    const char* kommando = method->GetText();
    std::string fuervergleich(kommando);
    Erzeugungsmethode ausgabe;
    if (fuervergleich == "APPDATA") {
        ausgabe = Erzeugungsmethode::APPDATA;
    } else {
        if (fuervergleich == "DNS") {
            ausgabe = Erzeugungsmethode::DNS;
        } else {
            if (fuervergleich == "RANDOMNUMBERS") {
                ausgabe = Erzeugungsmethode::RANDOMNUMBERS;
            } else {
                ausgabe = Erzeugungsmethode::UNDEFINED;
            }
        }
    }
    return ausgabe;
}

uint32_t Konfigleser::gebeIntervall(uint32_t kettenid) {
    XMLElement* quelle = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("CHAINS").FirstChildElement("CHAIN").ToElement();
    if (!quelle) {return 0;}
    uint32_t zaehler = 1;
    while (quelle && zaehler != kettenid) {
        quelle = quelle->NextSiblingElement("CHAIN");
        if (!quelle) {return 0;}
        zaehler++;
    }
    XMLElement* interval = quelle->FirstChildElement("INTERVAL");
    if (!interval) {return 0;}
    const char* zeitraum = interval->GetText();
    uint32_t ausgabe = atoi(zeitraum);
    return ausgabe;
}

uint32_t Konfigleser::gebeVergroesserungsfaktor(uint32_t kettenid) {
    XMLElement* quelle = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("CHAINS").FirstChildElement("CHAIN").ToElement();
    if (!quelle) {return 0;}
    uint32_t zaehler = 1;
    while (quelle && zaehler != kettenid) {
        quelle = quelle->NextSiblingElement("CHAIN");
        if (!quelle) {return 0;}
        zaehler++;
    }
    XMLElement* zahl = quelle->FirstChildElement("FACTOR");
    if (!zahl) {return 0;}
    const char* faktor = zahl->GetText();
    uint32_t ausgabe = atoi(faktor);
    return ausgabe;
}

double Konfigleser::gebeVergroesserungswahrscheinlichkeit(uint32_t kettenid) {
    XMLElement* quelle = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("CHAINS").FirstChildElement("CHAIN").ToElement();
    if (!quelle) {return 0;}
    uint32_t zaehler = 1;
    while (quelle && zaehler != kettenid) {
        quelle = quelle->NextSiblingElement("CHAIN");
        if (!quelle) {return 0;}
        zaehler++;
    }
    XMLElement* wahrschlkeit = quelle->FirstChildElement("RATIO");
    if (!wahrschlkeit) {return 0;}
    const char* wahrscheinlichkeit = wahrschlkeit->GetText();
    double ausgabe = atof(wahrscheinlichkeit);
    if ((ausgabe < 0) | (ausgabe > 1)) {return 0;}
    return ausgabe;
}

uint32_t Konfigleser::gebeQuellGruppenID(uint32_t kettenid) {
    XMLElement* quelle = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("CHAINS").FirstChildElement("CHAIN").ToElement();
    if (!quelle) {return 0;}
    uint32_t zaehler = 1;
    while (quelle && zaehler != kettenid) {
        quelle = quelle->NextSiblingElement("CHAIN");
        if (!quelle) {return 0;}
        zaehler++;
    }
    XMLElement* eintrag = quelle->FirstChildElement("FROMGROUP");
    if (!eintrag) {return 0;}
    const char* gruppe = eintrag->GetText();
    std::string gruppenname(gruppe);
    uint32_t nummer = gebeGruppennummer(gruppenname);
    return nummer;
}

uint32_t Konfigleser::gebeZielGruppenID(uint32_t kettenid) {
    XMLElement* quelle = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE1").FirstChildElement("CHAINS").FirstChildElement("CHAIN").ToElement();
    if (!quelle) {return 0;}
    uint32_t zaehler = 1;
    while (quelle && zaehler != kettenid) {
        quelle = quelle->NextSiblingElement("CHAIN");
        if (!quelle) {return 0;}
        zaehler++;
    }
    XMLElement* eintrag = quelle->FirstChildElement("TOGROUP");
    if (!eintrag) {return 0;}
    const char* gruppe = eintrag->GetText();
    std::string gruppenname(gruppe);
    uint32_t nummer = gebeGruppennummer(gruppenname);
    return nummer;
}

bool Konfigleser::ausfuehrenModul2() {
    XMLElement* ausfuehren = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE2").FirstChildElement("EXECUTE").ToElement();
    if (!ausfuehren) {return true;}
    const char* fuehreaus = ausfuehren->GetText();
    std::string inklusive(fuehreaus);
    bool ausgabe = (inklusive == "Y") || (inklusive == "YES") || (inklusive == "1");
    return ausgabe;
}

std::string Konfigleser::gebeNameModul2() {
    XMLElement* pcapname = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE2").FirstChildElement("NAME").ToElement();
    if (!pcapname) {return "";}
    const char* pcapbezeichner = pcapname->GetText();
    std::string ausgabe(pcapbezeichner);
    return ausgabe;    
}

std::string Konfigleser::gebeQuellnameModul2() {
    XMLElement* pcapname = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE2").FirstChildElement("SOURCE").ToElement();
    if (!pcapname) {return "";}
    const char* pcapbezeichner = pcapname->GetText();
    std::string ausgabe(pcapbezeichner);
    return ausgabe;    
}

uint32_t Konfigleser::gebeAnzahlAnomalien() {
    XMLElement* command = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE2").FirstChildElement("COMMAND").ToElement();
    uint32_t anzahl =0;
    while (command) {
        anzahl++;
        command = command->NextSiblingElement("COMMAND");
    }
    return anzahl;
}

Ergaenzungsmodus Konfigleser::gebeAnomaliemethode(uint32_t anoid) {
    XMLElement* komm = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE2").FirstChildElement("COMMAND").ToElement();
    if (!komm) {return Ergaenzungsmodus::UNDEFINED;}
    uint32_t zaehler = 1;
    while (komm && zaehler != anoid) {
        komm = komm->NextSiblingElement("COMMAND");
        if (!komm) {return Ergaenzungsmodus::UNDEFINED;}
        zaehler++;
    }
    XMLElement* method = komm->FirstChildElement("METHOD");
    if (!method) {return Ergaenzungsmodus::UNDEFINED;}
    const char* kommando = method->GetText();
    std::string fuervergleich(kommando);
    Ergaenzungsmodus ausgabe;
    if (fuervergleich == "PCAPSTEGO") {
        ausgabe = Ergaenzungsmodus::PCAPSTEGO;
    } else {
        if (fuervergleich == "ID2T") {
            ausgabe = Ergaenzungsmodus::ID2T;
        } else {
            ausgabe = Ergaenzungsmodus::UNDEFINED;
        }
    }
    return ausgabe;
}

std::string Konfigleser::gebeAnomaliezusatzinfo(uint32_t anoid) {
    XMLElement* komm = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE2").FirstChildElement("COMMAND").ToElement();
    if (!komm) {return "";}
    uint32_t zaehler= 1;
    while (komm && zaehler != anoid) {
        komm = komm->NextSiblingElement("COMMAND");
        if (!komm) {return "";}
        zaehler++;
    }
    XMLElement* info = komm->FirstChildElement("INFO");
    if (!info) {return "";}
    const char* infotext = info->GetText();
    std::string ausgabe(infotext);
    return ausgabe;
}


bool Konfigleser::ausfuehrenModul3() {
    XMLElement* ausfuehren = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE3").FirstChildElement("EXECUTE").ToElement();
    if (!ausfuehren) {return true;}
    const char* fuehreaus = ausfuehren->GetText();
    std::string inklusive(fuehreaus);
    bool ausgabe = (inklusive == "Y") || (inklusive == "YES") || (inklusive == "1");
    return ausgabe;
}

std::string Konfigleser::gebeNameModul3() {
    XMLElement* pcapname = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE3").FirstChildElement("NAME").ToElement();
    if (!pcapname) {return "";}
    const char* pcapbezeichner = pcapname->GetText();
    std::string ausgabe(pcapbezeichner);
    return ausgabe;    
}

std::string Konfigleser::gebeQuellnameModul3() {
    XMLElement* pcapname = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE3").FirstChildElement("SOURCE").ToElement();
    if (!pcapname) {return "";}
    const char* pcapbezeichner = pcapname->GetText();
    std::string ausgabe(pcapbezeichner);
    return ausgabe;    
}

uint32_t Konfigleser::gebeAnzahlRauschen() {
    XMLElement* faultyconn = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE3").FirstChildElement("FAULTYCONNECTION").ToElement();
    uint32_t anzahl =0;
    while (faultyconn) {
        anzahl++;
        faultyconn = faultyconn->NextSiblingElement("FAULTYCONNECTION");
    }
    return anzahl;
}

long long Konfigleser::gebeRauschenTimestamp(uint32_t rauschenid) {
    XMLElement* faultyconn = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE3").FirstChildElement("FAULTYCONNECTION").ToElement();
    if (!faultyconn) {return -1;}
    uint32_t zaehler=1;
    while (faultyconn && zaehler != rauschenid) {
        faultyconn = faultyconn->NextSiblingElement("FAULTYCONNECTION");
        if (!faultyconn) {return -1;}
        zaehler++;
    }
    XMLElement* starttime = faultyconn->FirstChildElement("STARTTIME");
    if (!starttime) {return 0;}
    const char* timestamp = starttime->GetText();
    long long zeitstempel = atoll(timestamp);
    return zeitstempel;
}

long long Konfigleser::gebeRauschenEndTimestamp(uint32_t rauschenid) {
    XMLElement* faultyconn = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE3").FirstChildElement("FAULTYCONNECTION").ToElement();
    if (!faultyconn) {return -1;}
    uint32_t zaehler =1;
    while (faultyconn && zaehler != rauschenid) {
        faultyconn = faultyconn->NextSiblingElement("FAULTYCONNECTION");
        if (!faultyconn) {return -1;}
        zaehler=1;
    }
    XMLElement* endtime = faultyconn->FirstChildElement("ENDTIME");
    if (!endtime) {return -1;}
    const char* timestamp = endtime->GetText();
    long long zeitstempel = atoll(timestamp);
    return zeitstempel;
}

double Konfigleser::gebeRauschenWahrscheinlichkeit(uint32_t rauschenid) {
    XMLElement* faultyconn = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE3").FirstChildElement("FAULTYCONNECTION").ToElement();
    if (!faultyconn) {return 0;}
    uint32_t zaehler = 1;
    while (faultyconn && zaehler != rauschenid) {
        faultyconn = faultyconn->NextSiblingElement("FAULTYCONNECTION");
        if (!faultyconn) {return 0;}
        zaehler++;
    }
    XMLElement* faultyratio = faultyconn->FirstChildElement("FAULTYRATIO");
    if (!faultyratio) {return 0;}
    const char* wahrscheinlichkeit = faultyratio->GetText();
    double ausgabe = atof(wahrscheinlichkeit);
    if ((ausgabe < 0) | (ausgabe > 1)) {return 0;}
    return ausgabe;
}

std::string Konfigleser::gebeRauschenQuellMAC(uint32_t rauschenid, uint32_t position) {
    XMLElement* faultyconn = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE3").FirstChildElement("FAULTYCONNECTION").ToElement();
    if (!faultyconn) {return "";}
    uint32_t zaehler = 1;
    while (faultyconn && zaehler != rauschenid) {
        faultyconn = faultyconn->NextSiblingElement("FAULTYCONNECTION");
        if (!faultyconn) {return "";}
        zaehler++;
    }
    XMLElement* fromgroup = faultyconn->FirstChildElement("FROMGROUP");
    if (!fromgroup) {return "";}
    const char* gruppe = fromgroup->GetText();
    std::string gruppenname(gruppe);
    uint32_t gruppenid = gebeGruppennummer(gruppenname);
    std::string ausgabe(gebeMAC(gruppenid,position));
    return ausgabe;
}

std::string Konfigleser::gebeRauschenZielMAC(uint32_t rauschenid, uint32_t position) {
    XMLElement* faultyconn = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE3").FirstChildElement("FAULTYCONNECTION").ToElement();
    if (!faultyconn) {return "";}
    uint32_t zaehler = 1;
    while (faultyconn && zaehler != rauschenid) {
        faultyconn = faultyconn->NextSiblingElement("FAULTYCONNECTION");
        if (!faultyconn) {return "";}
        zaehler++;
    }
    XMLElement* togroup = faultyconn->FirstChildElement("TOGROUP");
    if (!togroup) {return "";}
    const char* gruppe = togroup->GetText();
    std::string gruppenname(gruppe);
    uint32_t gruppenid = gebeGruppennummer(gruppenname);
    std::string ausgabe(gebeMAC(gruppenid,position));
    return ausgabe;
}

Aenderungsmodus Konfigleser::gebeRauschenVerfahren(uint32_t rauschenid) {
    XMLElement* faultyconn = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE3").FirstChildElement("FAULTYCONNECTION").ToElement();
    if (!faultyconn) {return Aenderungsmodus::UNDEFINED;}
    uint32_t zaehler = 1;
    while (faultyconn && zaehler != rauschenid) {
        faultyconn = faultyconn->NextSiblingElement("FAULTYCONNECTION");
        if (!faultyconn) {return Aenderungsmodus::UNDEFINED;}
        zaehler++;
    }
    XMLElement* faultytype = faultyconn->FirstChildElement("FAULTYTYPE");
    if (!faultytype) {return Aenderungsmodus::UNDEFINED;} 
    const char* verfahren = faultytype->GetText();
    std::string fuervergleich(verfahren);
    Aenderungsmodus ausgabe;
    if (fuervergleich == "DELETE") {
        ausgabe = Aenderungsmodus::DELETE;
    } else {
        if (fuervergleich == "SHIFT") {
            ausgabe = Aenderungsmodus::SHIFT;
        } else {
            if (fuervergleich == "COPY") {
                ausgabe = Aenderungsmodus::COPY;
            } else {
                ausgabe = Aenderungsmodus::UNDEFINED;
            }
        }
    }
    return ausgabe;
}

Rauschenbetroffene Konfigleser::gebeRauschenBetroffene(uint32_t rauschenid) {
    XMLElement* faultyconn = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE3").FirstChildElement("FAULTYCONNECTION").ToElement();
    if (!faultyconn) {return Rauschenbetroffene::UNDEFINED;}
    uint32_t zaehler = 1;
    while (faultyconn && zaehler != rauschenid) {
        faultyconn = faultyconn->NextSiblingElement("FAULTYCONNECTION");
        if (!faultyconn) {return Rauschenbetroffene::UNDEFINED;}
        zaehler++;
    }
    XMLElement* concerning = faultyconn->FirstChildElement("CONCERNING");
    if (!concerning) {return Rauschenbetroffene::UNDEFINED;}
    const char* betroffene = concerning->GetText();
    std::string fuervergleich(betroffene);
    Rauschenbetroffene ausgabe;
    if (fuervergleich == "FROMGROUPTOGROUP") {
        ausgabe = Rauschenbetroffene::FROMGROUPTOGROUP;
    } else {
        if (fuervergleich == "FROMGROUPTOALL") {
            ausgabe = Rauschenbetroffene::FROMGROUPTOALL;
        } else {
            if (fuervergleich == "FROMALLTOGROUP") {
                ausgabe = Rauschenbetroffene::FROMALLTOGROUP;
            } else {
                if (fuervergleich == "FROMALLTOALL") {
                    ausgabe = Rauschenbetroffene::FROMALLTOALL;
                } else {
                    ausgabe = Rauschenbetroffene::UNDEFINED;
                }
            }
        }
    }
    return ausgabe;
}

uint32_t Konfigleser::gebeRauschenZeitverschiebung(uint32_t rauschenid) {
    XMLElement* faultyconn = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE3").FirstChildElement("FAULTYCONNECTION").ToElement();
    if (!faultyconn) {return 0;}
    uint32_t zaehler = 1;
    while (faultyconn && zaehler != rauschenid) {
        faultyconn = faultyconn->NextSiblingElement("FAULTYCONNECTION");
        if (!faultyconn) {return 0;}
        zaehler++;
    }
    XMLElement* faultytimeshift = faultyconn->FirstChildElement("FAULTYTIMESHIFT");
    if (!faultytimeshift) {return 0;}
    const char* wahrscheinlichkeit = faultytimeshift->GetText();
    uint32_t ausgabe = atoi(wahrscheinlichkeit);
    return ausgabe;
}


bool Konfigleser::ausfuehrenModul4() {
    XMLElement* ausfuehren = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE4").FirstChildElement("EXECUTE").ToElement();
    if (!ausfuehren) {return true;}
    const char* fuehreaus = ausfuehren->GetText();
    std::string inklusive(fuehreaus);
    bool ausgabe = (inklusive == "Y") || (inklusive == "YES") || (inklusive == "1");
    return ausgabe;
}

std::string Konfigleser::gebeQuellnameModul4() {
    XMLElement* pcapname = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE4").FirstChildElement("SOURCE").ToElement();
    if (!pcapname) {return "";}
    const char* pcapbezeichner = pcapname->GetText();
    std::string ausgabe(pcapbezeichner);
    return ausgabe;    
}

std::string Konfigleser::gebeKettenparameterdateiname() {
    XMLElement* csvname = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE4").FirstChildElement("FILENAME").ToElement();
    if (!csvname) {return "";}
    const char* csvbezeichner = csvname->GetText();
    std::string ausgabe(csvbezeichner);
    return ausgabe;    
}

uint32_t Konfigleser::gebeVerbindungGeschlossenNach() {
    XMLElement* geschlossennach = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE4").FirstChildElement("CONNECTIONASSUMECLOSEDAFTER").ToElement();
    if (!geschlossennach) {return 0;}
    const char* geschlossen = geschlossennach->GetText();
    uint32_t ausgabe = atoi(geschlossen);
    return ausgabe;
}

uint32_t Konfigleser::gebeBytesPayloadberechnung() {
    XMLElement* bytes = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE4").FirstChildElement("PAYLOADENTROPY").FirstChildElement("NUMFIRSTBYTES").ToElement();
    if (!bytes) {return 0;}
    const char* anzbytes = bytes->GetText();
    uint32_t ausgabe = atoi(anzbytes);
    return ausgabe;
}

uint32_t Konfigleser::gebePaketePayloadberechnung() {
    XMLElement* pakete = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE4").FirstChildElement("PAYLOADENTROPY").FirstChildElement("NUMPACKETS").ToElement();
    if (!pakete) {return 0;}
    const char* anzpakete = pakete->GetText();
    uint32_t ausgabe = atoi(anzpakete);
    return ausgabe;
}

uint32_t Konfigleser::gebeNanosekundenProIntervall() {
    XMLElement* nanosek = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE4").FirstChildElement("HURST").FirstChildElement("INTERVALNANOSECS").ToElement();
    if (!nanosek) {return 0;}
    const char* anzahlnanosek = nanosek->GetText();
    uint32_t ausgabe = atoi(anzahlnanosek);
    return ausgabe;
}

uint32_t Konfigleser::gebeMinimaleZahlIntervalle() {
    XMLElement* minint = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE4").FirstChildElement("HURST").FirstChildElement("MININTERVALS").ToElement();
    if (!minint) {return 0;}
    const char* anzahlminint = minint->GetText();
    uint32_t ausgabe = atoi(anzahlminint);
    return ausgabe;
}

uint32_t Konfigleser::gebeMaximaleZahlAufteilungen() {
    XMLElement* maxauft = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE4").FirstChildElement("HURST").FirstChildElement("MAXSPLITS").ToElement();
    if (!maxauft) {return 0;}
    const char* anzahlmaxauft = maxauft->GetText();
    uint32_t ausgabe = atoi(anzahlmaxauft);
    return ausgabe;    
}

bool Konfigleser::globaleHurstparameter() {
    XMLElement* ausfuehren = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE4").FirstChildElement("HURST").FirstChildElement("INCLUDEGLOBAL").ToElement();
    if (!ausfuehren) {return true;}
    const char* fuehreaus = ausfuehren->GetText();
    std::string inklusive(fuehreaus);
    bool ausgabe = (inklusive == "Y") || (inklusive == "YES") || (inklusive == "1");
    return ausgabe;
}

std::string Konfigleser::gebeHurstglobalDateiname() {
    XMLElement* txtname = xmlZugriff.FirstChildElement("CONFIG").FirstChildElement("MODULE4").FirstChildElement("HURST").FirstChildElement("FILENAME").ToElement();
    if (!txtname) {return "";}
    const char* txtbezeichner = txtname->GetText();
    std::string ausgabe(txtbezeichner);
    return ausgabe;    
}