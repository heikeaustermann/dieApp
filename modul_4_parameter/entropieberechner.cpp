# include "entropieberechner.h"
# include <math.h>

double Entropieberechner::berechne(std::string eingabeHexString) {
    int laenge = eingabeHexString.length();
    if (laenge < 2) {return 0;}
    int gesamtzahl = floor(laenge/2);
    std::string teilstring;
    std::map<std::string,int> anzahlen;
    for (int i = 0; i<gesamtzahl; i++) {
        teilstring = eingabeHexString.substr(2*i,2);
        if (anzahlen.find(teilstring) == anzahlen.end()) {
            anzahlen.insert(anzahlen.end(),{teilstring,1});
            } else {
                anzahlen[teilstring]++;
            }
    }
    double rueckgabe = 0;
    double zwischen = 0;
    for (auto& anzahl : anzahlen) {
        zwischen = (double) anzahl.second / gesamtzahl;
        rueckgabe += zwischen * log2(zwischen) / 8;
    }
    rueckgabe *= -1;
    return rueckgabe;
}