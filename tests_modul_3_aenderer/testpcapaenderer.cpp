#include <gtest/gtest.h>
#include "../modul_3_aenderer/pcapaenderer.h"

std::string konfigOrt = "../tests_modul_0_werkzeugkasten/ressourcen/konfiguration.xml";
std::string quellOrt = "ressourcen/neuesPcap.pcap";
std::string zielOrt = "geaendertesPcap.pcap";

TEST(PcapAendererTest,unbekannterOrt) {
    PcapAenderer pcapaenderer("michgibtesnicht.xml",quellOrt,zielOrt);
    bool erzeugt = pcapaenderer.aenderePcap(1);
    EXPECT_EQ(erzeugt,false);
}

TEST(PcapErgaenzerTest, einfacherDurchlauf) {
    PcapAenderer pcapaenderer(konfigOrt,quellOrt,zielOrt);
    bool erzeugt = pcapaenderer.aenderePcap();
    EXPECT_EQ(erzeugt,true);
}