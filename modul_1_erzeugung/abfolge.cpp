#include "abfolge.h"

Abfolge::Abfolge(uint32_t eingabeKettenID, long long eingabeEnde, Erzeugungsmethode eingabeMethode, uint32_t eingabeSchrittnummer, bool eingabeInklusiveLayer2, pcpp::MacAddress eingabeMacquelle, pcpp::MacAddress eingabeMacziel, pcpp::IPv6Address eingeabeIpquelle, pcpp::IPv6Address eingabeIpziel, uint16_t eingabePortquelle, uint16_t eingabePortziel) {
    kettenID = eingabeKettenID;
    endzeitstempel = eingabeEnde;
    methode = eingabeMethode;
    schrittnummer =  eingabeSchrittnummer;
    inklusiveLayer2 = eingabeInklusiveLayer2;
    macquelle = eingabeMacquelle;
    macziel = eingabeMacziel;
    ipquelle = eingeabeIpquelle;
    ipziel = eingabeIpziel;
    portquelle = eingabePortquelle;
    portziel = eingabePortziel;
}

bool Abfolge::beendeteKette(long long naechsterzeitstempel) {
    bool rueckgabe = false;
    schrittnummer++;
    if (naechsterzeitstempel > endzeitstempel) {rueckgabe = true;}
    return rueckgabe;
}
