#include <string>
#include <vector>
#include <set>
#include <cstdlib>

#ifndef PCAPAENDERER_H
#define PCAPAENDERER_H


class PcapAenderer{
    public:
    PcapAenderer(std::string eingabeKonfigOrt, std::string eingabeQuelldatei, std::string eingabeZieldatei);
    bool aenderePcap(uint32_t position);
    bool aenderePcap();

    private:
    uint32_t anzahlPositionen;
    std::string konfigOrt;
    std::string quelldatei;
    std::string zieldatei;
    std::string zwischenspeicherdatei;
    std::string kopierspeicherdatei;
    std::string zwischenquelldatei;
    std::string zwischenzieldatei;
    std::string befehl;

    long long starttime;
    long long endtime;
    double rauschenWahrscheinlichkeit;
    uint32_t rauschenZeitverschiebung;
    bool fromall;
    bool toall;
    uint32_t anzahlQuellen;
    uint32_t anzahlZiele;
    std::vector<std::string> macQuellen;
    std::vector<std::string> macZiele;
    bool keeppacket;
    bool shiftpacket;
    struct timeval zeitstempel;
    std::multiset<long long> zwischenzeiten;
    
};

#endif