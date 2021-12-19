#include <string>

#ifndef PCAPERGAENZER_H
#define PCAPERGAENZER_H

class PcapErgaenzer{
    public:
    PcapErgaenzer(std::string eingabeKonfigOrt, std::string eingabeQuelldatei, std::string eingabeZieldatei);
    bool ergaenzePcap(uint32_t position);
    bool ergaenzeAlle();

    private:
    uint32_t anzahlPositionen;
    std::string konfigOrt;
    std::string quelldatei;
    std::string zieldatei;
    std::string zwischenspeicherdatei;
    std::string kopierspeicherdatei;
    std::string zwischenquelldatei;
    std::string zwischenzieldatei;

};

#endif