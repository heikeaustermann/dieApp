#include <string>
#include "tinyxml2.h"
#include "schluesselwoerter.h"

/* 
    Konfigleser führt keine inhaltlichen Prüfungen durch.
    Wenn zu einem Aufruf kein Eintrag vorliegt, wird undefined, leer oder 0 ausgegeben.
*/

# ifndef KONFIGLESER_H
# define KONFIGLESER_H

class Konfigleser {
    public:
    Konfigleser(std::string dateiname);
    int gebeFehlercode();

    // mögliche sinnvolle Erweiterung
    // uint32_t gebePcapversion();
    // uint32_t gebeIPVersion();
    bool gebeEinschliesslichLayer2();

    uint32_t gebeAnzahlGruppen();
    std::string gebeGruppenname(uint32_t gruppenid);
    uint32_t gebeGruppennummer(std::string gruppenname);
    std::string gebeMAC(uint32_t gruppenid, uint32_t quellid);
    std::string gebeIP(uint32_t gruppenid, uint32_t quellid);

    uint32_t gebeMaxPakete();
    uint32_t gebeMaxKetten();
    long long gebeTimestamp();
    long long gebeEndTimestamp();
    uint32_t gebeIntervall();

    bool ausfuehrenModul1();
    std::string gebeNameModul1();
    uint32_t gebeAnzahlKetten();
    uint32_t gebeGewicht(uint32_t kettenid);
    uint32_t gebeStartverzoegerung(uint32_t kettenid);
    uint32_t gebeVerfruehtesende(uint32_t kettenid);
    Erzeugungsmethode gebeMethode(uint32_t kettenid);
    uint32_t gebeIntervall(uint32_t kettenid);
    uint32_t gebeVergroesserungsfaktor(uint32_t kettenid);
    double gebeVergroesserungswahrscheinlichkeit(uint32_t kettenid);
    uint32_t gebeQuellGruppenID(uint32_t kettenid);
    uint32_t gebeZielGruppenID(uint32_t kettenid);

    bool ausfuehrenModul2();
    std::string gebeNameModul2();
    std::string gebeQuellnameModul2();
    uint32_t gebeAnzahlAnomalien();
    Ergaenzungsmodus gebeAnomaliemethode(uint32_t anoid);
    std::string gebeAnomaliezusatzinfo(uint32_t anoid);

    bool ausfuehrenModul3();
    std::string gebeNameModul3();
    std::string gebeQuellnameModul3();
    uint32_t gebeAnzahlRauschen();
    long long gebeRauschenTimestamp(uint32_t rauschenid);
    long long gebeRauschenEndTimestamp(uint32_t rauschenid);
    double gebeRauschenWahrscheinlichkeit(uint32_t rauschenid);
    std::string gebeRauschenQuellMAC(uint32_t rauschenid, uint32_t position);
    std::string gebeRauschenZielMAC(uint32_t rauschenid, uint32_t position);
    Aenderungsmodus gebeRauschenVerfahren(uint32_t rauschenid);
    Rauschenbetroffene gebeRauschenBetroffene(uint32_t rauschenid);
    uint32_t gebeRauschenZeitverschiebung(uint32_t rauschenid);

    bool ausfuehrenModul4();
    std::string gebeQuellnameModul4();
    std::string gebeKettenparameterdateiname();
    uint32_t gebeVerbindungGeschlossenNach();
    uint32_t gebeBytesPayloadberechnung();
    uint32_t gebePaketePayloadberechnung();
    uint32_t gebeNanosekundenProIntervall();
    uint32_t gebeMinimaleZahlIntervalle();
    uint32_t gebeMaximaleZahlAufteilungen();
    bool globaleHurstparameter();
    std::string gebeHurstglobalDateiname();

    private:
    std::string dateiOrt;
    int fehlercode;
    tinyxml2::XMLDocument xmlKonfig;
    tinyxml2::XMLHandle xmlZugriff;
};

#endif