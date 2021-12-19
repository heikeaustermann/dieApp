#include "zeitstempelgeber.h"
#include "intervallgeber.h"
#include <random>
#include <time.h>

Zeitstempelgeber::Zeitstempelgeber(uint64_t eingabeAnfangszeitpunkt, uint32_t eingabeBasisintervall, uint32_t eingabeVariabilitaet)
{
    fraktal = false;
    letzterZeitpunkt = eingabeAnfangszeitpunkt;
    basisintervall = (eingabeBasisintervall<1)?1:eingabeBasisintervall;
    variabilitaet = (eingabeVariabilitaet = 0)? 1:eingabeVariabilitaet;
}

Zeitstempelgeber::Zeitstempelgeber(uint64_t eingabeAnfangszeitpunkt, uint32_t eingabeBasisintervall, uint32_t eingabeVergroesserungsfaktor, double eingabeVergroesserungswahrscheinlichkeit)
{
    fraktal = true;
    letzterZeitpunkt = eingabeAnfangszeitpunkt;
    basisintervall = (eingabeBasisintervall<1)?1:eingabeBasisintervall;
    vergroesserungsfaktor = (eingabeVergroesserungsfaktor<2)?2:eingabeVergroesserungsfaktor;
    vergroesserungswahrscheinlichkeit = eingabeVergroesserungswahrscheinlichkeit;
}

uint64_t Zeitstempelgeber::gebeZeitstempel() {
    if (fraktal) {
        uint32_t intervall = Intervallgeber::fraktal(basisintervall, vergroesserungsfaktor, vergroesserungswahrscheinlichkeit);
        letzterZeitpunkt = letzterZeitpunkt + intervall;
        return letzterZeitpunkt;
    } else {
        letzterZeitpunkt = letzterZeitpunkt + basisintervall;
        uint64_t rueckgabe = letzterZeitpunkt+Intervallgeber::uniform(variabilitaet);
        return rueckgabe;
    }
}

uint64_t Zeitstempelgeber::gebeZeitstempel(uint32_t eingabeRuestzeit) {
    if (fraktal) {
        uint32_t intervall = Intervallgeber::fraktal(basisintervall, vergroesserungsfaktor, vergroesserungswahrscheinlichkeit);
        letzterZeitpunkt = letzterZeitpunkt + eingabeRuestzeit + intervall;
        return letzterZeitpunkt;
    } else {
        letzterZeitpunkt = letzterZeitpunkt + basisintervall + eingabeRuestzeit;
        uint64_t rueckgabe = letzterZeitpunkt+Intervallgeber::uniform(variabilitaet);
        return rueckgabe;
    }
}