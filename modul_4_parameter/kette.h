#include <string>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#ifndef KETTE_H
#define KETTE_H

struct Kette {
    public:
    Kette(std::size_t kettenid, std::size_t hashwert, std::size_t hashwertrueck, long long zeitstempel, uint64_t groesse, std::string payload, uint32_t maxpaketefuerentropie, uint16_t ethertype,std::string macquelle, std::string macziel, std::string ipquelle, std::string ipziel, uint16_t portquelle, uint16_t portziel);
    Kette(std::size_t kettenid, std::size_t hashwert, std::size_t hashwertrueck, long long zeitstempel, uint64_t groesse, std::string payload, uint32_t maxpaketefuerentropie, std::string ipquelle, std::string ipziel, uint16_t portquelle, uint16_t portziel);
    void setzeGruppen(std::string gruppequelle, std::string gruppeziel);
    void setzeHurst(double hurstAnzahl,double hurstGroesse);
    std::string gebeWertezeile();
    
    static size_t berechneHash(uint16_t ethertype,std::string macquelle, std::string macziel, std::string ipquelle, std::string ipziel, uint16_t portquelle, uint16_t portziel);
    static size_t berechneHash(std::string ipquelle, std::string ipziel, uint16_t portquelle, uint16_t portziel);

    long long gebeErsterzeitstempel();
    long long gebeLetzterzeitstempel();
    std::string gebeIPquelle();
    std::string gebeIPziel();
    void ergaenzePakethin(long long zeitstempel, uint32_t groesse, std::string payload);
    void ergaenzePaketrueck(long long zeitstempel, uint32_t groesse, std::string payload);
    
    private:
    void initialisiereWerte(std::size_t kettenid, std::size_t hashwert, std::size_t hashwertrueck, long long zeitstempel, uint64_t groesse, std::string payload, uint32_t maxpaketefuerentropie, uint16_t ethertype,std::string macquelle, std::string macziel, std::string ipquelle, std::string ipziel, uint16_t portquelle, uint16_t portziel);
    void berechneEntropie(std::string payload);
    std::size_t kettenid;
    bool inklusiveLayer2;
    bool gruppengesetzt;
    bool entropieberechnet;
    bool hurstergaenzt;
    std::size_t hashwert;
    std::size_t hashwertrueck;
    long long ersterzeitstempel;
    long long letzterzeitstempel;
    uint32_t anzahlpaketehin;
    uint32_t anzahlpaketerueck;
    uint64_t groessepaketehin;
    uint64_t groessepaketerueck;
    uint16_t ethertype=0;
    std::string macquelle="";
    std::string macziel="";
    std::string ipquelle;
    std::string ipziel;
    std::string gruppequelle;
    std::string gruppeziel;
    uint16_t portquelle;
    uint16_t portziel;
    uint32_t maxpaketefuerentropie;
    uint32_t anzpaketefuerentropie;
    std::string bytesfuerentropie;
    double entropie;
    double hurstAnzahl;
    double hurstGroesse;
};

#endif