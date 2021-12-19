#include "kette.h"
#include "entropieberechner.h"
#include <functional>
#include <iostream>

Kette::Kette(std::size_t eingabekettenid, std::size_t eingabehashwert, std::size_t eingabehashwertrueck, long long eingabezeitstempel, uint64_t eingabegroesse, std::string eingabepayload, uint32_t eingabemaxpaketefuerentropie, uint16_t eingabeethertype, std::string eingabemacquelle, std::string eingabemacziel, std::string eingabeipquelle, std::string eingabeipziel, uint16_t eingabeportquelle, uint16_t eingabeportziel) {
    inklusiveLayer2 = true;
    initialisiereWerte(eingabekettenid,eingabehashwert,eingabehashwertrueck,eingabezeitstempel,eingabegroesse,eingabepayload,eingabemaxpaketefuerentropie,eingabeethertype,eingabemacquelle,eingabemacziel,eingabeipquelle,eingabeipziel,eingabeportquelle,eingabeportziel);
}

Kette::Kette(std::size_t eingabekettenid, std::size_t eingabehashwert, std::size_t eingabehashwertrueck, long long eingabezeitstempel, uint64_t eingabegroesse, std::string eingabepayload, uint32_t eingabemaxpaketefuerentropie, std::string eingabeipquelle, std::string eingabeipziel, uint16_t eingabeportquelle, uint16_t eingabeportziel) {
    inklusiveLayer2 = false;
    initialisiereWerte(eingabekettenid,eingabehashwert,eingabehashwertrueck,eingabezeitstempel,eingabegroesse,eingabepayload,eingabemaxpaketefuerentropie,0,"","",eingabeipquelle,eingabeipziel,eingabeportquelle,eingabeportziel);
}

void Kette::initialisiereWerte(std::size_t eingabekettenid, std::size_t eingabehashwert, std::size_t eingabehashwertrueck, long long eingabezeitstempel, uint64_t eingabegroesse, std::string eingabepayload, uint32_t eingabemaxpaketefuerentropie, uint16_t eingabeethertype, std::string eingabemacquelle, std::string eingabemacziel, std::string eingabeipquelle, std::string eingabeipziel, uint16_t eingabeportquelle, uint16_t eingabeportziel) {
    kettenid = eingabekettenid;
    gruppengesetzt = false;
    entropieberechnet = false;
    hurstergaenzt = false;
    hashwert = eingabehashwert;
    hashwertrueck = eingabehashwertrueck;
    ersterzeitstempel = eingabezeitstempel;
    letzterzeitstempel = eingabezeitstempel;
    anzahlpaketehin = 1;
    anzahlpaketerueck = 0;
    groessepaketehin = eingabegroesse;
    groessepaketerueck = 0;
    ethertype = eingabeethertype;
    macquelle = eingabemacquelle;
    macziel = eingabemacziel;
    ipquelle = eingabeipquelle;
    ipziel = eingabeipziel;
    gruppequelle = "";
    gruppeziel = "";
    portquelle = eingabeportquelle;
    portziel = eingabeportziel;
    hurstAnzahl = 0;
    hurstGroesse = 0;
    if (eingabemaxpaketefuerentropie == 0) {
        entropieberechnet = true;
    } else {
        maxpaketefuerentropie = eingabemaxpaketefuerentropie;
    }
    if (eingabepayload.empty()) {
        anzpaketefuerentropie = 0;
    } else {
        anzpaketefuerentropie = 1;
        bytesfuerentropie = eingabepayload;
    }
}

long long Kette::gebeErsterzeitstempel() {
    return ersterzeitstempel;
}

long long Kette::gebeLetzterzeitstempel() {
    return letzterzeitstempel;
}

void Kette::ergaenzePakethin(long long eingabeZeitstempel, uint32_t eingabeGroesse, std::string eingabepayload) {
    anzahlpaketehin++;
    letzterzeitstempel = eingabeZeitstempel;
    groessepaketehin += eingabeGroesse;
    if (!entropieberechnet) {berechneEntropie(eingabepayload);}
}

void Kette::ergaenzePaketrueck(long long eingabeZeitstempel, uint32_t eingabeGroesse, std::string eingabepayload) {
    anzahlpaketerueck++;
    letzterzeitstempel = eingabeZeitstempel;
    groessepaketerueck += eingabeGroesse;
    if (!entropieberechnet) {berechneEntropie(eingabepayload);}
}

void Kette::berechneEntropie(std::string eingabepayload) {
    if (!eingabepayload.empty()) {
        bytesfuerentropie.append(eingabepayload);
        anzpaketefuerentropie++;
        if (anzpaketefuerentropie == maxpaketefuerentropie) {
            entropie = Entropieberechner::berechne(bytesfuerentropie);
            entropieberechnet = true;
            bytesfuerentropie.clear();
        }
    }
}

std::string Kette::gebeIPquelle() {
    return ipquelle;
}

std::string Kette::gebeIPziel() {
    return ipziel;
}

void Kette::setzeGruppen(std::string eingabeGruppequelle, std::string eingabeGruppeziel) {
    gruppequelle = eingabeGruppequelle;
    gruppeziel = eingabeGruppeziel;
    gruppengesetzt = true;
}

void Kette::setzeHurst(double eingabeHurstAnzahl, double eingabeHurstGroesse) {
    hurstAnzahl = eingabeHurstAnzahl;
    hurstGroesse = eingabeHurstGroesse;
    hurstergaenzt = true;
}

std::string Kette::gebeWertezeile() {
    std::string grundlage = std::to_string(ersterzeitstempel).append(";").append(std::to_string(letzterzeitstempel));
    if (gruppengesetzt) {grundlage.append(";").append(gruppequelle).append(";").append(gruppeziel);}
    if (inklusiveLayer2) {grundlage.append(";").append(std::to_string(ethertype)).append(";").append(macquelle).append(";").append(macziel);}
    grundlage.append(";").append(ipquelle).append(";").append(ipziel).append(";").append(std::to_string(portquelle)).append(";").append(std::to_string(portziel));
    grundlage.append(";").append(std::to_string(anzahlpaketehin)).append(";").append(std::to_string(anzahlpaketerueck)).append(";").append(std::to_string(groessepaketehin)).append(";").append(std::to_string(groessepaketerueck));
    grundlage.append(";").append(std::to_string(entropie));
    if (hurstergaenzt) {grundlage.append(";").append(std::to_string(hurstAnzahl)).append(";").append(std::to_string(hurstGroesse));}
    return grundlage;
}

size_t Kette::berechneHash(uint16_t ethertype,std::string macquelle, std::string macziel, std::string ipquelle, std::string ipziel, uint16_t portquelle, uint16_t portziel) {
    std::string grundlage = std::to_string((int)ethertype).append(macquelle).append(macziel).append(ipquelle).append(ipziel).append(std::to_string((int)portquelle)).append(std::to_string((int)portziel));
    size_t hashrueckgabe = std::hash<std::string>{}(grundlage);
    return hashrueckgabe;
}

size_t Kette::berechneHash(std::string ipquelle, std::string ipziel, uint16_t portquelle, uint16_t portziel) {
    std::string grundlage = ipquelle.append(ipziel).append(std::to_string((int)portquelle)).append(std::to_string((int)portziel));
    size_t hashrueckgabe = std::hash<std::string>{}(grundlage);
    return hashrueckgabe;
}