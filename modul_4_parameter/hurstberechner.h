# include <vector>
# include <cstdint>

# ifndef HURSTBERECHNER_H
# define HURSTBERECHNER_H

class Hurstberechner{
    public:
    static double klassisch(uint32_t minimaleIntervallbreite, uint32_t maximaleAufteilungen, std::vector<uint32_t> werte);
    static double anisLloyd(uint32_t minimaleIntervallbreite, uint32_t maximaleAufteilungen, std::vector<uint32_t> werte);

    private:
    static double berechneMittelwert(std::vector<uint32_t> werte);
    static double berechneReservoir(std::vector<uint32_t> werte);
    static double berechneStandardabweichung(std::vector<uint32_t> werte);
    static double berechneReskaliertesReservoir(std::vector<uint32_t> werte);
    static double berechneReskaliertesReservoirFolgenelement(uint32_t aufteilungen, std::vector<uint32_t> werte);
    static double berechneSteigungfuerLogaufLog2mitkleinstenQuadraten(std::vector<double> werte, uint32_t werteanzahl);
    static double berechneKorrekturnachAnisLloyd(std::vector<double> werte, uint32_t werteanzahl);
};

# endif