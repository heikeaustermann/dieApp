#include <string>
#include <vector>
#include <Packet.h>
#include <PcapFileDevice.h>
#include <IPv6Layer.h>

#include "kettendef.h"
#include "abfolge.h"
#include "zeitstempelgeber.h"

#ifndef PCAPERZEUGER_H
#define PCAPERZEUGER_H

class PcapErzeuger{
    public:
    PcapErzeuger(std::string eingabeKonfigOrt, std::string eingabeZieldatei);
    bool leseKonfig();
    bool schreibePcap();

    private:
    void initialeBefuellung();
    void befuelle(uint32_t abfolgenummer);
    bool erstellePaket(uint32_t abfolgenummer);
    bool schreibeTCPPaket();
    bool schreibeUDPPaket();

    std::string konfigOrt;
    bool konfiggelesen;

    uint32_t maximalAnzahlPakete;
    long long startzeitpunkt;
    long long endzeitpunkt;
    uint32_t intervallzwischenKetten;
    uint32_t maximalAnzahlKetten;
    uint32_t anzahlDefKetten;
    bool inklusiveLayer2;

    std::vector<Kettendef> ketten;

    std::vector<long long> kettenzeiten;
    std::vector<Abfolge> kettenabfolgen;
    std::vector<Zeitstempelgeber> kettenzeitstempelgeber;

    long long aktuelleZeit;

    pcpp::PcapFileWriterDevice schreiber;
    struct timeval zeitstempel;
    pcpp::MacAddress mac1;
    pcpp::MacAddress mac2;
    pcpp::IPv6Address ip1;
    pcpp::IPv6Address ip2;
    /** siehe Anmerkung 
    uint8_t flowlabel0;
    uint8_t flowlabel1;
    uint8_t flowlabel2;
    **/
    uint16_t port1;
    uint16_t port2;
    uint32_t sequno;
    uint32_t ackno;
    bool tcpack;
    bool tcpsyn;
    bool tcpfin;
    bool tcppsh;
    std::string payload;

    /** Anmerkung zu auskommentierten flowlabel-Einträgen:
     * wird ein Flow Label genutzt, dann zum Beispiel folgende Bausteine
     * 
     * flowlabel0 = (uint8_t) rand()%16;
     * flowlabel1 = (uint8_t) rand()%256;
     * flowlabel2 = (uint8_t) rand()%256;
     * 
     * newIPv6Layer.getIPv6Header()->flowLabel[0] = flowlabel0;
     * newIPv6Layer.getIPv6Header()->flowLabel[1] = flowlabel1;
     * newIPv6Layer.getIPv6Header()->flowLabel[2] = flowlabel2;
     * 
     * Achtung: die Einträge ebenfalls in den Kettenabfolgen ergänzen
     * und die genaue Definition mit einer "Richtung" beachten
    **/
};

#endif