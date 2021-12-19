# include "kette.h"

# include <string.h>
# include <list>
# include <map>

# include <PcapFileDevice.h>

# ifndef PARAMETERAUSPCAPGEBER_H
# define PARAMETERAUSPCAPGEBER_H

class ParameterAusPcapGeber {
    public:
    ParameterAusPcapGeber(std::string eingabeKonfigort, std::string eingabeDateiort);
    long long gebeErstenZeitstempel();
    long long gebeLetztenZeitstempel();
    unsigned long gebeAnzahlPakete();
    unsigned long gebeGroessePcap();
    unsigned long gebeAnzahlKetten();
    bool schreibeAktuelleKettenAus(std::string eingabeDateiortKettenaktuell);
    void schreibeGlobaleHurstParameterAus(std::string eingabeDateiortHurstglobal);
    bool schreibeAllesAus(std::string eingabeDateiortKetten,std::string eingabeDateiortHurstglobal);
    bool ergaenzeKettenGruppennamen();
    bool ergaenzeKettenHurst(long long nanosekundenintervall, uint32_t intervallbreite, uint32_t maximaleAufteilungen);
    double gebeHurstZeitintervalle(long long nanosekundenintervall, uint32_t intervallbreite, uint32_t maximaleAufteilungen);
    double gebeHurstGroessenintervalle(long long nanosekundenintervall, uint32_t intervallbreite, uint32_t maximaleAufteilungen);
    double gebeHurstZwischenankunftszeiten(uint32_t intervallbreite, uint32_t maximaleAufteilungen);

    private:
    std::string dateiort;
    std::string konfigort;
    bool pcapausgelesen;
    void lesePcapAus();
    std::vector<uint32_t> erzeugeZaehlerliste(std::vector<long long> zeitstempelliste, long long zeitintervall);
    std::vector<uint32_t> erzeugeGroessenliste(std::vector<long long> zeitstempelliste, std::vector<uint32_t> groessenliste, long long zeitintervall);

    uint32_t verbindunggeschlossennach;
    bool inklusiveLayer2;
    uint32_t nanosekundenproIntervall;
    uint32_t minimaleIntervallbreite;
    uint32_t maximaleAufteilungen;
    uint32_t bytesPayloadberechnung;
    uint32_t paketePayloadberechnung;

    struct timespec zeitstempel;
    unsigned long anzahlgelesenePakete;
    unsigned long groessepcap;
    long long ersterZeitstempel;
    long long letzterZeitstempel;
    std::vector<bool> lesbarliste;

    std::vector<long long> zeitstempelliste;
    std::vector<uint32_t> paketgroessenliste;
    struct Familie {size_t hash; long long anfang;};
    std::vector<Familie> familienliste; // Zuordnung des nten Pakets zu einer Familie
    std::map<long long, Kette> familie; // eine Familie besteht aus Ketten mit gleichem Hashwert im Minimalabstand verbindunggeschlossennach falls letzteres !=0
    std::map<size_t,std::map<long long,Kette>> kettenliste; // die Kettenliste besteht aus Hashes und zugehörigen Familien
    unsigned long anzahlKetten;
    std::map<std::string,std::string> ipzugruppe; // die MAC-Adresse wird ignoriert, falls gewünscht könnte diese aber immer später per append in den String übernommen werden

};

# endif