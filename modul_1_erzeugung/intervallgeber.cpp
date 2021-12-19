# include "intervallgeber.h"
# include <math.h>

uint32_t Intervallgeber::exponentiell(uint32_t basiswert, uint32_t maximalerWert){
    if (maximalerWert<basiswert) {return maximalerWert;}
    uint32_t zufall = rand()%maximalerWert;
    zufall = floor(-log(zufall/maximalerWert)/basiswert);
    zufall =(zufall < 1)?1:zufall;
    return zufall;
}

uint32_t Intervallgeber::fraktal(uint32_t basiswert, uint32_t vergroesserungsfaktor, double vergroesserungswahrscheinlichkeit) {
    uint32_t maxVorletztesIntervall = floor(UINT32_MAX/vergroesserungsfaktor);
    uint32_t intervall = fraktalmaximalerwert(basiswert,maxVorletztesIntervall,vergroesserungsfaktor,vergroesserungswahrscheinlichkeit);
    return intervall;
}

uint32_t Intervallgeber::fraktalmaximalerwert(uint32_t basiswert, uint32_t maximalerWert, uint32_t vergroesserungsfaktor, double vergroesserungswahrscheinlichkeit) {
    uint32_t intervall = basiswert;
    uint32_t maxVorletztesIntervall = floor(maximalerWert/vergroesserungsfaktor);
    double vergleich =(double) (rand()%1000)/1000;
    bool weiter = (vergleich<vergroesserungswahrscheinlichkeit)?true:false;
    while ((weiter == true) && (intervall <= maxVorletztesIntervall))
    {
        intervall = intervall * vergroesserungsfaktor;
        vergleich =(double) (rand()%1000)/1000;
        weiter = (vergleich<vergroesserungswahrscheinlichkeit)?true:false;
    }
    intervall = rand()%intervall + 1;
    return intervall;
}

uint32_t Intervallgeber::uniform(uint32_t maximalerWert) {
    uint32_t rueckgabe = rand()%maximalerWert;
    rueckgabe =+1;
    return rueckgabe;
}