#ifndef ZEITSTEMPELGEBER_H
#define ZEITSTEMPELGEBER_H

#include <stdint.h>

class Zeitstempelgeber {
    public:
    Zeitstempelgeber(uint64_t eingabeAnfangszeitpunkt, uint32_t eingabeBasisintervall, uint32_t eingabeVariabilitaet);
    Zeitstempelgeber(uint64_t eingabeAnfangszeitpunkt, uint32_t eingabeBasisintervall, uint32_t eingabeVergroesserungsfaktor, double eingabeVergroesserungswahrscheinlichkeit);
    
    uint64_t gebeZeitstempel();
    uint64_t gebeZeitstempel(uint32_t eingabeRuestzeit);

    private:
    bool fraktal;

    uint32_t variabilitaet;

    uint32_t basisintervall;
    uint32_t vergroesserungsfaktor;
    double vergroesserungswahrscheinlichkeit;
    
    uint64_t letzterZeitpunkt;
};

#endif