#include <gtest/gtest.h>
#include <string>
#include "../modul_0_werkzeugkasten/konfigleser.h"

std::string dateiort = "ressourcen/konfiguration.xml";
std::string dateiortalternative = "ressourcen/konfiguration_alternativ.xml";

TEST(KonfigleserTest,Einlesetest){
    Konfigleser konfigleser(dateiort);
    int errorcode = konfigleser.gebeFehlercode();
    EXPECT_EQ(errorcode,0); 
}

TEST(KonfigleserTest,ParameterohneLayer2) {
    Konfigleser konfigleser(dateiortalternative);
    bool inklusiveLayer2 = konfigleser.gebeEinschliesslichLayer2();
    EXPECT_EQ(inklusiveLayer2,false);
}


TEST(KonfigleserTest,Anzahlgruppentest){
    Konfigleser konfigleser(dateiort);
    uint32_t anz = konfigleser.gebeAnzahlGruppen();
    EXPECT_EQ(anz,3);
}

TEST(KonfigleserTest,Gruppennametest){
    Konfigleser konfigleser(dateiort);
    std::string grpname = konfigleser.gebeGruppenname(2);
    EXPECT_EQ(grpname,"externalspecial");
}

TEST(KonfigleserTest,Gruppennummertest) {
    Konfigleser konfigleser(dateiort);
    uint32_t nummer = konfigleser.gebeGruppennummer("external");
    EXPECT_EQ(nummer,3);
}

TEST(KonfigleserTest,ErsteQuellMACgebertest){
    Konfigleser konfigleser(dateiort);
    std::string mac = konfigleser.gebeMAC(1,1);
    EXPECT_EQ(mac,"aa:bb:cc:dd:ee:ff"); 
}

TEST(KonfigleserTest,EinsZweiZielMACgebertest){
    Konfigleser konfigleser(dateiort);
    std::string mac = konfigleser.gebeMAC(3,2);
    EXPECT_EQ(mac,"aa:11:bb:22:cc:33"); 
}

TEST(KonfigleserTest,ErsteQuellIPgebertest){
    Konfigleser konfigleser(dateiort);
    std::string ip = konfigleser.gebeIP(1,2);
    EXPECT_EQ(ip,"1:2:3:4::5"); 
}

TEST(KonfigleserTest,EinsZweiZielIPgebertest){
    Konfigleser konfigleser(dateiort);
    std::string ip = konfigleser.gebeIP(3,1);
    EXPECT_EQ(ip,"1::cdef"); 
}


TEST(KonfigleserTest,AusfuehrenModul1test) {
    Konfigleser konfigleser(dateiort);
    bool ausfuehren = konfigleser.ausfuehrenModul1();
    EXPECT_EQ(ausfuehren,true);
}

TEST(KonfigleserTest,NameModul1test) {
    Konfigleser konfigleser(dateiort);
    std::string namepcap = konfigleser.gebeNameModul1();
    EXPECT_EQ(namepcap,"neuesPcap");    
}

TEST(KonfigleserTest,Paketanzahlgebertest){
    Konfigleser konfigleser(dateiort);
    uint32_t pakete = konfigleser.gebeMaxPakete();
    EXPECT_EQ(pakete,10000); 
}

TEST(KonfigleserTest,Zeitstempelgebertest){
    Konfigleser konfigleser(dateiort);
    long long zeitstempel = konfigleser.gebeTimestamp();
    EXPECT_EQ(zeitstempel,1619872968012345); 
}

TEST(KonfigleserTest,Endzeitstempelgebertest){
    Konfigleser konfigleser(dateiort);
    long long zeitstempel = konfigleser.gebeEndTimestamp();
    EXPECT_EQ(zeitstempel,1620473684012345); 
}

TEST(KonfigleserTest,Kettenintervallgebertest){
    Konfigleser konfigleser(dateiort);
    uint32_t intervall = konfigleser.gebeIntervall();
    EXPECT_EQ(intervall,100); 
}

TEST(KonfigleserTest,Maxkettengebertest){
    Konfigleser konfigleser(dateiort);
    uint32_t anz = konfigleser.gebeMaxKetten();
    EXPECT_EQ(anz,99); 
}

TEST(KonfigleserTest,Anzahlkettengebertest){
    Konfigleser konfigleser(dateiort);
    uint32_t anz = konfigleser.gebeAnzahlKetten();
    EXPECT_EQ(anz,3); 
}

TEST(KonfigleserTest,Gewichtsgebertest){
    Konfigleser konfigleser(dateiort);
    uint32_t gew = konfigleser.gebeGewicht(3);
    EXPECT_EQ(gew,3); 
}

TEST(KonfigleserTest,Fruehesendegebertest){
    Konfigleser konfigleser(dateiort);
    uint32_t zeit = konfigleser.gebeVerfruehtesende(3);
    EXPECT_EQ(zeit,876543); 
}

TEST(KonfigleserTest,Spaeteranfanggebertest){
    Konfigleser konfigleser(dateiort);
    uint32_t zeit = konfigleser.gebeStartverzoegerung(3);
    EXPECT_EQ(zeit,1000); 
}

TEST(KonfigleserTest,Methodegebertest){
    Konfigleser konfigleser(dateiort);
    Erzeugungsmethode meth1 = konfigleser.gebeMethode(1);
    Erzeugungsmethode meth2 = konfigleser.gebeMethode(2);
    EXPECT_EQ(meth1,Erzeugungsmethode::APPDATA); 
    EXPECT_EQ(meth2,Erzeugungsmethode::DNS);
}

TEST(KonfigleserTest,ErstesQuellIntervallgebertest){
    Konfigleser konfigleser(dateiort);
    uint32_t intervall = konfigleser.gebeIntervall(1);
    uint32_t intervall2 = konfigleser.gebeIntervall(2);
    EXPECT_EQ(intervall,10000); 
    EXPECT_EQ(intervall2,300000);
}

TEST(KonfigleserTest,Vergroesserungsfaktorgebertest){
    Konfigleser konfigleser(dateiort);
    uint32_t fak1 = konfigleser.gebeVergroesserungsfaktor(1);
    uint32_t fak2 = konfigleser.gebeVergroesserungsfaktor(3);
    EXPECT_EQ(fak1,10); 
    EXPECT_EQ(fak2,100);
}

TEST(KonfigleserTest,Vergroesserungswahrscheinlichkeitgebertest){
    Konfigleser konfigleser(dateiort);
    double wahrscheinlichkeit = konfigleser.gebeVergroesserungswahrscheinlichkeit(1);
    EXPECT_EQ(wahrscheinlichkeit,0.5); 
}

TEST(KonfigleserTest,Gruppengebertest){
    Konfigleser konfigleser(dateiort);
    uint32_t grp1 = konfigleser.gebeZielGruppenID(1);
    uint32_t grp2 = konfigleser.gebeQuellGruppenID(3);
    EXPECT_EQ(grp1,3); 
    EXPECT_EQ(grp2,2);
}


TEST(KonfigleserTest,AusfuehrenModul2test) {
    Konfigleser konfigleser(dateiortalternative);
    bool ausfuehren = konfigleser.ausfuehrenModul2();
    EXPECT_EQ(ausfuehren,true);
}

TEST(KonfigleserTest,NameModul2test) {
    Konfigleser konfigleser(dateiort);
    std::string namepcap = konfigleser.gebeNameModul2();
    EXPECT_EQ(namepcap,"ergaenztesPcap");    
}

TEST(KonfigleserTest,QuellnameModul2test) {
    Konfigleser konfigleser(dateiort);
    std::string namepcap = konfigleser.gebeQuellnameModul2();
    EXPECT_EQ(namepcap,"neuesPcap");    
}

TEST(KonfigleserTest,Anomalienanzahlgebertest){
    Konfigleser konfigleser(dateiort);
    uint32_t anzanos = konfigleser.gebeAnzahlAnomalien();
    EXPECT_EQ(anzanos,1); 
}

TEST(KonfigleserTest,Anomalienmethodengebertest){
    Konfigleser konfigleser(dateiort);
    Ergaenzungsmodus ergaenzungsmodus = konfigleser.gebeAnomaliemethode(1);
    EXPECT_EQ(ergaenzungsmodus,Ergaenzungsmodus::PCAPSTEGO); 
}

TEST(KonfigleserTest,Infostringgebertest){
    Konfigleser konfigleser(dateiort);
    std::string info = konfigleser.gebeAnomaliezusatzinfo(1);
    EXPECT_EQ(info,"ressourcen/attacks.txt"); 
}


TEST(KonfigleserTest,AusfuehrenModul3test) {
    Konfigleser konfigleser(dateiort);
    bool ausfuehren = konfigleser.ausfuehrenModul3();
    EXPECT_EQ(ausfuehren,true);
}

TEST(KonfigleserTest,NameModul3test) {
    Konfigleser konfigleser(dateiort);
    std::string namepcap = konfigleser.gebeNameModul3();
    EXPECT_EQ(namepcap,"geaendertesPcap");    
}

TEST(KonfigleserTest,QuellnameModul3test) {
    Konfigleser konfigleser(dateiortalternative);
    std::string namepcap = konfigleser.gebeQuellnameModul3();
    EXPECT_EQ(namepcap,"ergaenztesPcap");    
}

TEST(KonfigleserTest,Rauschenanzahlgebertest) {
    Konfigleser konfigleser(dateiort);
    uint32_t anzrauschen = konfigleser.gebeAnzahlRauschen();
    EXPECT_EQ(anzrauschen,3); 
}

TEST(KonfigleserTest,Rauschenzeitstempelgebertest){
    Konfigleser konfigleser(dateiort);
    long long zeitstempel = konfigleser.gebeRauschenTimestamp(2);
    EXPECT_EQ(zeitstempel,1619872969117191); 
}

TEST(KonfigleserTest,Rauschenendzeitstempelgebertest){
    Konfigleser konfigleser(dateiort);
    long long zeitstempel = konfigleser.gebeRauschenEndTimestamp(1);
    EXPECT_EQ(zeitstempel,1620473684012345); 
}

TEST(KonfigleserTest,Rauschenwahrscheinlichkeitgebertest){
    Konfigleser konfigleser(dateiort);
    double wahrscheinlichkeit = konfigleser.gebeRauschenWahrscheinlichkeit(1);
    EXPECT_EQ(wahrscheinlichkeit,0.9); 
}

TEST(KonfigleserTest,RauschenQuellMACgebertest){
    Konfigleser konfigleser(dateiort);
    std::string mac = konfigleser.gebeRauschenQuellMAC(2,3);
    EXPECT_EQ(mac,"44:44:22:12:34:88"); 
}

TEST(KonfigleserTest,RauschenZielMACgebertest){
    Konfigleser konfigleser(dateiort);
    std::string mac = konfigleser.gebeRauschenZielMAC(1,1);
    EXPECT_EQ(mac,"00:11:22:33:44:00"); 
}

TEST(KonfigleserTest,Rauschenverfahrengebertest){
    Konfigleser konfigleser(dateiort);
    Aenderungsmodus verf = konfigleser.gebeRauschenVerfahren(2);
    EXPECT_EQ(verf,Aenderungsmodus::COPY); 
}

TEST(KonfigleserTest,Rauschenbetroffenegebertest){
    Konfigleser konfigleser(dateiort);
    Rauschenbetroffene betr = konfigleser.gebeRauschenBetroffene(2);
    EXPECT_EQ(betr,Rauschenbetroffene::FROMGROUPTOGROUP); 
}

TEST(KonfigleserTest,Rauschenzeitverschiebungsgebertest){
    Konfigleser konfigleser(dateiort);
    uint32_t zeitversch = konfigleser.gebeRauschenZeitverschiebung(1);
    EXPECT_EQ(zeitversch,100); 
}


TEST(KonfigleserTest,AusfuehrenModul4test) {
    Konfigleser konfigleser(dateiort);
    bool ausfuehren = konfigleser.ausfuehrenModul4();
    EXPECT_EQ(ausfuehren,true);
}

TEST(KonfigleserTest,QuellnameModul4test) {
    Konfigleser konfigleser(dateiortalternative);
    std::string namepcap = konfigleser.gebeQuellnameModul4();
    EXPECT_EQ(namepcap,"geaendertesPcap");    
}

TEST(KonfigleserTest,Dateinameparametergebertest) {
    Konfigleser konfigleser(dateiort);
    std::string parameterort = konfigleser.gebeKettenparameterdateiname();
    EXPECT_EQ(parameterort,"kettenparameter");
}

TEST(KonfigleserTest,Verbindunggeschlossennachgebertest) {
    Konfigleser konfigleser(dateiortalternative);
    uint32_t geschlossennach = konfigleser.gebeVerbindungGeschlossenNach();
    EXPECT_EQ(geschlossennach,100000000);
}

TEST(KonfigleserTest,Payloadbytesgebertest) {
    Konfigleser konfigleser(dateiortalternative);
    uint32_t bytes = konfigleser.gebeBytesPayloadberechnung();
    EXPECT_EQ(bytes,96);
}

TEST(KonfigleserTest,Payloadpaketegebertest) {
    Konfigleser konfigleser(dateiortalternative);
    uint32_t pakete = konfigleser.gebePaketePayloadberechnung();
    EXPECT_EQ(pakete,4);
}

TEST(KonfigleserTest,Nanosekundenintervallegebertest) {
    Konfigleser konfigleser(dateiort);
    uint32_t nanosekunden = konfigleser.gebeNanosekundenProIntervall();
    EXPECT_EQ(nanosekunden,100000);
}

TEST(KonfigleserTest,Minimalezahlintervallegebertest) {
    Konfigleser konfigleser(dateiort);
    uint32_t intervallanzahl = konfigleser.gebeMinimaleZahlIntervalle();
    EXPECT_EQ(intervallanzahl,10);
}

TEST(KonfigleserTest,Maximalezahlaufteilungengebertest) {
    Konfigleser konfigleser(dateiort);
    uint32_t intervallanzahl = konfigleser.gebeMaximaleZahlAufteilungen();
    EXPECT_EQ(intervallanzahl,20);
}

TEST(KonfigleserTest,GlobaleHurstberechnentest) {
    Konfigleser konfigleser(dateiort);
    bool inklusive = konfigleser.globaleHurstparameter();
    EXPECT_EQ(inklusive,true);
}

TEST(KonfigleserTest,GlobalHurstDateinamengebertest) {
    Konfigleser konfigleser(dateiort);
    std::string txtdateiort = konfigleser.gebeHurstglobalDateiname();
    EXPECT_EQ(txtdateiort,"hurstglobal");
}