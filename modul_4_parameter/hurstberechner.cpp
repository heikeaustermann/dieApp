# include "hurstberechner.h"
# include <math.h>
# include <stdio.h>

double Hurstberechner::klassisch(uint32_t minimaleIntervallbreite, uint32_t maximaleAufteilungen, std::vector<uint32_t> werte) {
    uint32_t anzahl = werte.size();
    if (anzahl == 0) {return 0;}
    if (anzahl < minimaleIntervallbreite) {return 0;}

    std::vector<double> reskaliertesReservoirFolge;
    double zwischenterm = 0;

    for (uint32_t zaehler = 0; ((anzahl/pow(2,zaehler)) > minimaleIntervallbreite) && (zaehler < maximaleAufteilungen); zaehler++) {
        zwischenterm = Hurstberechner::berechneReskaliertesReservoirFolgenelement(pow(2,zaehler),werte);
        reskaliertesReservoirFolge.insert(reskaliertesReservoirFolge.end(),zwischenterm);
    }

    double hurst = Hurstberechner::berechneSteigungfuerLogaufLog2mitkleinstenQuadraten(reskaliertesReservoirFolge, anzahl);

    return hurst;
}

double Hurstberechner::anisLloyd(uint32_t minimaleIntervallbreite, uint32_t maximaleAufteilungen, std::vector<uint32_t> werte) {
    uint32_t anzahl = werte.size();
    if (anzahl == 0) {return 0;}
    if (anzahl < minimaleIntervallbreite) {return 0;}

    std::vector<double> reskaliertesReservoirFolge;
    double zwischenterm = 0;

    for (uint32_t zaehler = 0; ((anzahl/pow(2,zaehler)) > minimaleIntervallbreite) && (zaehler < maximaleAufteilungen); zaehler++) {
        zwischenterm = Hurstberechner::berechneReskaliertesReservoirFolgenelement(pow(2,zaehler),werte);
        reskaliertesReservoirFolge.insert(reskaliertesReservoirFolge.end(),zwischenterm);
    }

    double hurst = Hurstberechner::berechneKorrekturnachAnisLloyd(reskaliertesReservoirFolge, anzahl);

    return hurst;
}

double Hurstberechner::berechneMittelwert(std::vector<uint32_t> werte) {
    uint32_t anzahl = werte.size();
    if (anzahl == 0) {return 0;}

    double mittelwert = 0;
    uint32_t zaehler = 0;
    while (zaehler < anzahl) {
        mittelwert = mittelwert + werte[zaehler];
        zaehler++;
    }
    mittelwert = mittelwert / anzahl;
    return mittelwert;
}

double Hurstberechner::berechneReservoir(std::vector<uint32_t> werte) {
    uint32_t anzahl = werte.size();
    if (anzahl == 0) {return 0;}

    double mittelwert = berechneMittelwert(werte);
    double akkumulierteAbweichung = werte[0] - mittelwert;
    double min = akkumulierteAbweichung;
    double max = akkumulierteAbweichung;

    uint32_t zaehler = 1;
    while (zaehler < anzahl) {
        akkumulierteAbweichung = akkumulierteAbweichung + werte[zaehler] - mittelwert;
        if (akkumulierteAbweichung < min) {min = akkumulierteAbweichung;}
        if (akkumulierteAbweichung > max) {max = akkumulierteAbweichung;}
        zaehler++;
    }

    double reservoir = max - min;
    return reservoir;
}

double Hurstberechner::berechneStandardabweichung(std::vector<uint32_t> werte) {
    uint32_t anzahl = werte.size();
    if (anzahl == 0) {return 0;}

    double mittelwert = berechneMittelwert(werte);

    double standardabweichung = 0;
    double quadrat = 0;
    for (uint32_t zaehler = 0; zaehler < anzahl; zaehler++) {
        quadrat = (werte[zaehler] - mittelwert)*(werte[zaehler] - mittelwert);
        standardabweichung = standardabweichung + quadrat/anzahl;
    }
    standardabweichung = sqrt(standardabweichung);
    return standardabweichung;
}

double Hurstberechner::berechneReskaliertesReservoir(std::vector<uint32_t> werte) {
    uint32_t anzahl = werte.size();
    if (anzahl == 0) {return 0;}  

    double reservoir = berechneReservoir(werte);
    double standardabweichung = berechneStandardabweichung(werte);

    if (standardabweichung == 0) {return 0;}

    double reskaliertesReservoir = reservoir / standardabweichung;
    return reskaliertesReservoir;
}

double Hurstberechner::berechneReskaliertesReservoirFolgenelement(uint32_t aufteilungen, std::vector<uint32_t> werte) {
    if (werte.size() == 0) {return 0;} 
    if (aufteilungen<1) {return 0;}

    uint32_t intervallbreite = floor(werte.size()/aufteilungen);
    if (intervallbreite < 1) {return 0;}

    uint32_t doppelte = werte.size() - aufteilungen*intervallbreite;

    uint32_t zaehlerdoppelte = doppelte - floor(doppelte/aufteilungen)*aufteilungen;
    uint32_t anzahl = intervallbreite + floor(doppelte/aufteilungen) + 1;
    uint32_t zwischenanzahl = anzahl;

    std::vector<uint32_t> zwischenwerte;

    double zwischenelement = 0;
    double zwischensumme = 0;

    for (uint32_t i=0; i<aufteilungen; i++) {
        if (i==zaehlerdoppelte) {zwischenanzahl--;}
        for (uint32_t j=0; j<zwischenanzahl; j++) {
            zwischenwerte.insert(zwischenwerte.end(),werte[i*anzahl+j]);
        }
        zwischenelement = berechneReskaliertesReservoir(zwischenwerte);
        zwischensumme = zwischensumme + zwischenelement;
        zwischenwerte.clear();
    }

    double element = zwischensumme / aufteilungen;

    return element;
}

double Hurstberechner::berechneSteigungfuerLogaufLog2mitkleinstenQuadraten(std::vector<double> werte, uint32_t werteanzahl) {
    int anzahl = werte.size();
    if (anzahl < 2) {return 0;}

    double zwischenterm = 0;
    std::vector<double> plotY;
    double summeY = 0;
    std::vector<double> plotX;
    double summeX = 0;
    for (int i=anzahl; i>0; i--) {
        zwischenterm = werte[i-1];
        plotY.insert(plotY.end(),log(zwischenterm));
        summeY += log(zwischenterm);
        plotX.insert(plotX.end(),log(werteanzahl / pow(2,i)));
        summeX += log(werteanzahl / pow(2,i));
    } 

    plotY.shrink_to_fit();
    plotX.shrink_to_fit();
    double mittelwertX = summeX / anzahl;
    double mittelwertY = summeY / anzahl;

    double zaehler = 0;
    double nenner = 0;
    for (int i=0; i<anzahl; i++) {
        zaehler += (plotX[i] - mittelwertX)*(plotY[i] - mittelwertY);
        nenner += (plotX[i] - mittelwertX)*(plotX[i] - mittelwertX);
    }

    double steigung = zaehler / nenner;
    return steigung;
}

double Hurstberechner::berechneKorrekturnachAnisLloyd(std::vector<double> werte, uint32_t werteanzahl) {
    int anzahl = werte.size();
    if (anzahl < 2) {return 0;}

    double vergleich;
    uint32_t zwischen;
    std::vector<double> korrekturvektor;
    for (int i=0; i<anzahl; i++) {
        vergleich =(double) werteanzahl / pow(2,i);
        zwischen = 0;
        if (vergleich<340) {
            for (int j=0;j<vergleich;j++) {
                zwischen = zwischen + sqrt(vergleich - j)/j;
            }
            zwischen = zwischen * gamma((vergleich-1)/2)/(sqrt(M_PI)*gamma(vergleich/2));
        } else {
            for (int j=0;j<vergleich;j++) {
                zwischen = zwischen + sqrt(vergleich - j)/j;
            }
            zwischen = zwischen/(sqrt(vergleich * M_PI / 2));
        }
        korrekturvektor.insert(korrekturvektor.end(),zwischen);
    }

    double zwischenterm = 0;
    std::vector<double> plotY;
    double summeY = 0;
    std::vector<double> plotX;
    double summeX = 0;

    for (int i=anzahl-1; i>-1; i--) {
        zwischenterm = werte[i]-korrekturvektor[i];
        plotY.insert(plotY.end(),zwischenterm);
        summeY += zwischenterm;
        plotX.insert(plotX.end(),werteanzahl / pow(2,i));
        summeX += werteanzahl / pow(2,i);
    } 

    plotY.shrink_to_fit();
    plotX.shrink_to_fit();
    double mittelwertX = summeX / anzahl;
    double mittelwertY = summeY / anzahl;

    double zaehler = 0;
    double nenner = 0;
    for (int i=0; i<anzahl; i++) {
        zaehler += (plotX[i] - mittelwertX)*(plotY[i] -mittelwertY);
        nenner += (plotX[i] - mittelwertX)*(plotX[i] - mittelwertX);
    }

    double steigung = 0.5 + (zaehler / nenner);
    return steigung;
}