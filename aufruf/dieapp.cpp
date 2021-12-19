# include "../modul_0_werkzeugkasten/konfigleser.h"
# include "../modul_1_erzeugung/pcaperzeuger.h"
# include "../modul_2_ergaenzer/pcapergaenzer.h"
# include "../modul_3_aenderer/pcapaenderer.h"
# include "../modul_4_parameter/parameterauspcapgeber.h"

int main() {

    const std::string konfigOrt = "ressourcen/konfiguration.xml";

    Konfigleser konfigleser(konfigOrt);
    if (konfigleser.gebeFehlercode() != 0) {return 0;}

    std::string pcapname;
    std::string quellname;
    std::string kettenparameterdateiname;
    std::string hurstglobalparameterdateiname;

    bool durchlauf;

    durchlauf = konfigleser.ausfuehrenModul1();
    if (durchlauf) {
        pcapname = konfigleser.gebeNameModul1().append(".pcap");
        PcapErzeuger erzeuger(konfigOrt,pcapname);
        durchlauf = erzeuger.schreibePcap();
    }

    durchlauf = konfigleser.ausfuehrenModul2();
    if (durchlauf) {
        pcapname = konfigleser.gebeNameModul2().append(".pcap");
        quellname = konfigleser.gebeQuellnameModul2().append(".pcap");
        PcapErgaenzer ergaenzer(konfigOrt,quellname,pcapname);
        durchlauf = ergaenzer.ergaenzeAlle();
    }

    durchlauf = konfigleser.ausfuehrenModul3();
    if (durchlauf) {
        pcapname = konfigleser.gebeNameModul3().append(".pcap");
        quellname = konfigleser.gebeQuellnameModul3().append(".pcap");
        PcapAenderer aenderer(konfigOrt,quellname,pcapname);
        durchlauf = aenderer.aenderePcap();
    }

    durchlauf = konfigleser.ausfuehrenModul4();
    if (durchlauf) {
        quellname = konfigleser.gebeQuellnameModul4().append(".pcap");
        ParameterAusPcapGeber parametergeber(konfigOrt,quellname);
        kettenparameterdateiname = konfigleser.gebeKettenparameterdateiname();
        hurstglobalparameterdateiname = konfigleser.gebeHurstglobalDateiname();
        durchlauf = parametergeber.schreibeAllesAus(kettenparameterdateiname,hurstglobalparameterdateiname);
    }

}