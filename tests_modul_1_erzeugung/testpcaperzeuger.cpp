#include <gtest/gtest.h>
#include "../modul_1_erzeugung/pcaperzeuger.h"

std::string konfigOrt = "../tests_modul_0_werkzeugkasten/ressourcen/konfiguration.xml";
std::string zielOrt = "neuesPcap.pcap";

TEST(PcapErzeugerTest,unbekannterOrt) {
    PcapErzeuger pcaperzeuger("michgibtesnicht.xml",zielOrt);
    bool erzeugt = pcaperzeuger.schreibePcap();
    EXPECT_EQ(erzeugt,false);
}

TEST(PcapErzeugerTest,Durchlauf) {
    PcapErzeuger pcaperzeuger(konfigOrt, zielOrt);
    bool erzeugt = pcaperzeuger.schreibePcap();
    EXPECT_EQ(erzeugt,true);
}