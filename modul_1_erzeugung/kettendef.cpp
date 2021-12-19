#include "kettendef.h"

Kettendef::Kettendef(long long eingabeZeitraumanfang, long long eingabeZeitraumende, uint32_t eingabeGewicht, Erzeugungsmethode eingabeMethode, uint32_t eingabeIntervall, uint32_t eingabeVergroesserungsfaktor, double eingabeVergroesserungswahrscheinlichkeit, std::vector<pcpp::IPv6Address> eingabeIpQuellen, std::vector<pcpp::IPv6Address> eingabeIpZiele, std::vector<pcpp::MacAddress> eingabeMacQuellen, std::vector<pcpp::MacAddress> eingabeMacZiele, uint32_t eingabeAnzahlQuellen, uint32_t eingabeAnzahlZiele) {
    zeitraumanfang = eingabeZeitraumanfang;
    zeitraumende = eingabeZeitraumende;
    gewicht = eingabeGewicht;
    methode = eingabeMethode;
    intervall = eingabeIntervall;
    vergroesserungsfaktor = eingabeVergroesserungsfaktor;
    vergroesserungswahrscheinlichkeit = eingabeVergroesserungswahrscheinlichkeit;
    ipQuellen = eingabeIpQuellen;
    ipZiele = eingabeIpZiele;
    macQuellen = eingabeMacQuellen;
    macZiele = eingabeMacZiele;
    anzahlQuellen = eingabeAnzahlQuellen;
    anzahlZiele = eingabeAnzahlZiele;
}