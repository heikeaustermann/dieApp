# include "../modul_4_parameter/parameterauspcapgeber.h"
# include <gtest/gtest.h>
# include <math.h>

std::string konfigort = "../tests_modul_0_werkzeugkasten/ressourcen/konfiguration.xml";
std::string konfigortohneL2 = "../tests_modul_0_werkzeugkasten/ressourcen/konfiguration_alternativ.xml";

/* 
Einzeln auskommentieren für den entsprechenden Test.
Zugriff auf die Konfigurationsdatei ergibt wegen nicht abgefangener Mehrfachabfragen Fehler.
TEST(ParameterAusPcapGeberTest, gebeEigenschaften) {
    ParameterAusPcapGeber parameterAusPcapGeber("ressourcen/pcap5054.pcap",konfigort);
    long long zeitstempel = parameterAusPcapGeber.gebeErstenZeitstempel();
    long long zeitstempel2 = parameterAusPcapGeber.gebeLetztenZeitstempel();
    unsigned long anzahl = parameterAusPcapGeber.gebeAnzahlPakete();
    unsigned long groesse = parameterAusPcapGeber.gebeGroessePcap();
    EXPECT_EQ(zeitstempel,1619872968012345);
    EXPECT_EQ(zeitstempel2,1620473410082681);
    EXPECT_EQ(anzahl,5054);
    EXPECT_EQ(groesse,171836);
}
*/

TEST(ParameterAusPcapGeberTest, ergaenzeHurst) {
    ParameterAusPcapGeber parameterAusPcapGeber(konfigort,"ressourcen/neuesPcap.pcap");
    parameterAusPcapGeber.ergaenzeKettenHurst(100000,10,20);
    parameterAusPcapGeber.ergaenzeKettenGruppennamen();
    bool schreiben = parameterAusPcapGeber.schreibeAktuelleKettenAus("ketten");
    parameterAusPcapGeber.schreibeGlobaleHurstParameterAus("hurstglobal");
    unsigned long anzahlk = parameterAusPcapGeber.gebeAnzahlKetten();
    EXPECT_EQ(anzahlk,6);
    EXPECT_EQ(schreiben,true);
}

TEST(ParameterAusPcapGeberTest, ergaenzeHurstohneL2) {
    ParameterAusPcapGeber parameterAusPcapGeber(konfigortohneL2,"ressourcen/neuesPcap.pcap");
    parameterAusPcapGeber.ergaenzeKettenHurst(100000,10,20);
    parameterAusPcapGeber.ergaenzeKettenGruppennamen();
    bool schreiben = parameterAusPcapGeber.schreibeAktuelleKettenAus("kettenohneL2");
    parameterAusPcapGeber.schreibeGlobaleHurstParameterAus("hurstglobalohneL2");
    EXPECT_EQ(schreiben,true);
}