#include <stdint.h>
#include <string>
#include <EthLayer.h>
#include <IPv6Layer.h>

#include "../modul_0_werkzeugkasten/schluesselwoerter.h"

#ifndef ABFOLGE_H
#define ABFOLGE_H

struct Abfolge {
    public:
    Abfolge(uint32_t kettenID, long long endzeitstempel, Erzeugungsmethode methode, uint32_t schrittnummer, bool inklusiveLayer2, pcpp::MacAddress macquelle, pcpp::MacAddress macziel, pcpp::IPv6Address ipquelle, pcpp::IPv6Address ipziel, uint16_t portquelle, uint16_t portziel);
    
    bool beendeteKette(long long naechsterzeitstempel);

    uint32_t kettenID;
    long long endzeitstempel;
    Erzeugungsmethode methode;
    uint32_t schrittnummer;
    bool inklusiveLayer2;
    pcpp::MacAddress macquelle="";
    pcpp::MacAddress macziel="";
    pcpp::IPv6Address ipquelle;
    pcpp::IPv6Address ipziel;
    uint16_t portquelle;
    uint16_t portziel;
};

#endif