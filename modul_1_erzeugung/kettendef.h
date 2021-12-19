#include <stdint.h>
#include <vector>
#include <PcapFileDevice.h>
#include "../modul_0_werkzeugkasten/schluesselwoerter.h"

#ifndef KETTENDEF_H
#define KETTENDEF_H

struct Kettendef {
    Kettendef(long long zeitraumanfang, long long zeitraumende, uint32_t gewicht, Erzeugungsmethode methode, uint32_t intervall, uint32_t vergroesserungsfaktor, double vergroesserungswahrscheinlichkeit, std::vector<pcpp::IPv6Address> ipQuellen, std::vector<pcpp::IPv6Address> ipZiele, std::vector<pcpp::MacAddress> macQuellen, std::vector<pcpp::MacAddress> macZiele, uint32_t anzahlQuellen, uint32_t anzahlZiele);

    long long zeitraumanfang;
    long long zeitraumende;
    uint32_t gewicht;
    Erzeugungsmethode methode;
    uint32_t intervall;
    uint32_t vergroesserungsfaktor;
    double vergroesserungswahrscheinlichkeit;
    std::vector<pcpp::IPv6Address> ipQuellen;
    std::vector<pcpp::IPv6Address> ipZiele;
    std::vector<pcpp::MacAddress> macQuellen;
    std::vector<pcpp::MacAddress> macZiele;
    uint32_t anzahlQuellen;
    uint32_t anzahlZiele;
};

#endif