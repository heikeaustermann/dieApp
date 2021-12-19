#include <gtest/gtest.h>
#include "../modul_2_ergaenzer/pcapergaenzer.h"
#include "../modul_0_werkzeugkasten/stringexec.h"

std::string konfigOrt = "../tests_modul_0_werkzeugkasten/ressourcen/konfiguration_alternativ.xml";
std::string quellDateiverzeichnis = "ressourcen/";
std::string quellOrt = "neuesPcap.pcap";
std::string zielOrt = "ergaenztesPcap.pcap";

TEST(PcapErgaenzerTest,unbekannterOrt) {
    PcapErgaenzer pcapergaenzer("michgibtesnicht.xml",quellOrt,zielOrt);
    bool erzeugt = pcapergaenzer.ergaenzePcap(1);
    EXPECT_EQ(erzeugt,false);
}

TEST(PcapErgaenzerTest, einfacherDurchlauf) {
    std::string befehl("cp ");
    befehl.append(quellDateiverzeichnis).append(quellOrt).append(" .");
    const char *zeile = befehl.c_str();
    Stringexec::exec(zeile);
    PcapErgaenzer pcapergaenzer(konfigOrt,quellOrt,zielOrt);
    bool erzeugt = pcapergaenzer.ergaenzeAlle();
    EXPECT_EQ(erzeugt,true);
}