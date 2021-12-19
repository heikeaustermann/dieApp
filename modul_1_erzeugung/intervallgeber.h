#include <stdint.h>

# ifndef INTERVALLGEBER_H
# define INTERVALLGEBER_H

class  Intervallgeber
{
public:
     static uint32_t exponentiell(uint32_t basiswert, uint32_t maximalerWert);
     static uint32_t fraktal(uint32_t basiswert, uint32_t vergroesserungsfaktor, double vergroesserungswahrscheinlichkeit);
     static uint32_t fraktalmaximalerwert(uint32_t basiswert, uint32_t maximalerWert, uint32_t vergroesserungsfaktor, double vergroesserungswahrscheinlichkeit);
     static uint32_t uniform(uint32_t maximalerWert);
};

# endif