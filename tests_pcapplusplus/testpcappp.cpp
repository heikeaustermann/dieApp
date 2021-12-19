#include <gtest/gtest.h>
#include <stdlib.h>
#include <stdio.h>
#include "SystemUtils.h"
#include "Packet.h"
#include "EthLayer.h"
#include "VlanLayer.h"
#include "IPv4Layer.h"
#include "TcpLayer.h"
#include "HttpLayer.h"
#include "UdpLayer.h"
#include "DnsLayer.h"
#include "PcapFileDevice.h"


TEST(PcapPPTest,PaketerzeugungMitZeitstempel){
	long long uebergebenerZeitstempel = 1620473684012345; // May  8, 2021 13:34:44.012345000 CEST
	bool zeitgesetzt = false;
	int maxNumber = 3;

	// angepasst aus Tutorial Packet Creation
	// https://pcapplusplus.github.io/docs/tutorials/packet-crafting#packet-creation

	// create a new Ethernet layer
	pcpp::EthLayer newEthernetLayer(pcpp::MacAddress("00:50:43:11:22:33"), pcpp::MacAddress("aa:bb:cc:dd:ee"));

	// create a new IPv4 layer
	pcpp::IPv4Layer newIPLayer(pcpp::IPv4Address(std::string("192.168.1.1")), pcpp::IPv4Address(std::string("10.0.0.1")));
	newIPLayer.getIPv4Header()->ipId = pcpp::hostToNet16(2000);
	newIPLayer.getIPv4Header()->timeToLive = 64;

	// create a new UDP layer
	pcpp::UdpLayer newUdpLayer(12345, 53);

	// create a new DNS layer
	pcpp::DnsLayer newDnsLayer;
	newDnsLayer.addQuery("www.ebay.com", pcpp::DNS_TYPE_A, pcpp::DNS_CLASS_IN);

	// create a packet with initial capacity of 100 bytes (will grow automatically if needed)
	pcpp::Packet newPacket(100);

	// add all the layers we created
	newPacket.addLayer(&newEthernetLayer);
	newPacket.addLayer(&newIPLayer);
	newPacket.addLayer(&newUdpLayer);
	newPacket.addLayer(&newDnsLayer);
	
	// compute all calculated fields
	newPacket.computeCalculateFields();

	// setze den Zeitstempel
	struct timeval zeitstempel;
	zeitstempel.tv_sec = uebergebenerZeitstempel/1000000;
	zeitstempel.tv_usec = uebergebenerZeitstempel%1000000;
	zeitgesetzt = newPacket.getRawPacket()->setPacketTimeStamp(zeitstempel);

	// compute all calculated fields
	newPacket.computeCalculateFields();

	// open a new pcap file
	pcpp::PcapFileWriterDevice writer("PaketerzeugungMitZeitstempel.pcap");
	writer.open();

	for (int i=0; i<maxNumber; i++) {
		// write the new packet to a pcap file
		writer.writePacket(*(newPacket.getRawPacket()));

		// Spielerei zur Zeitanpassung
		zeitstempel.tv_sec++;
		zeitgesetzt = zeitgesetzt && newPacket.getRawPacket()->setPacketTimeStamp(zeitstempel);
		newPacket.computeCalculateFields();
	}

	// close the file
	writer.close();

    // Zeiten ohne Fehlermeldung gesetzt
    EXPECT_EQ(zeitgesetzt,true); 
}

TEST(PcapPPTest,NGPaketerzeugungMitZeitstempel){
	long long uebergebenerZeitstempel = 1620473684012345; // May  8, 2021 13:34:44.012345000 CEST
	bool zeitgesetzt = false;
	int maxNumber = 3;

	// angepasst aus Tutorial Packet Creation
	// https://pcapplusplus.github.io/docs/tutorials/packet-crafting#packet-creation

	// create a new Ethernet layer
	pcpp::EthLayer newEthernetLayer(pcpp::MacAddress("00:50:43:11:22:33"), pcpp::MacAddress("aa:bb:cc:dd:ee"));

	// create a new IPv4 layer
	pcpp::IPv4Layer newIPLayer(pcpp::IPv4Address(std::string("192.168.1.1")), pcpp::IPv4Address(std::string("10.0.0.1")));
	newIPLayer.getIPv4Header()->ipId = pcpp::hostToNet16(2000);
	newIPLayer.getIPv4Header()->timeToLive = 64;

	// create a new UDP layer
	pcpp::UdpLayer newUdpLayer(12345, 53);

	// create a new DNS layer
	pcpp::DnsLayer newDnsLayer;
	newDnsLayer.addQuery("www.ebay.com", pcpp::DNS_TYPE_A, pcpp::DNS_CLASS_IN);

	// create a packet with initial capacity of 100 bytes (will grow automatically if needed)
	pcpp::Packet newPacket(100);

	// add all the layers we created
	newPacket.addLayer(&newEthernetLayer);
	newPacket.addLayer(&newIPLayer);
	newPacket.addLayer(&newUdpLayer);
	newPacket.addLayer(&newDnsLayer);
	
	// compute all calculated fields
	newPacket.computeCalculateFields();

	// setze den Zeitstempel
	struct timeval zeitstempel;
	zeitstempel.tv_sec = uebergebenerZeitstempel/1000000;
	zeitstempel.tv_usec = uebergebenerZeitstempel%1000000;
	zeitgesetzt = newPacket.getRawPacket()->setPacketTimeStamp(zeitstempel);

	// compute all calculated fields
	newPacket.computeCalculateFields();

	// open a new pcap file
	pcpp::PcapNgFileWriterDevice writer("NGPaketerzeugungMitZeitstempel.pcapng");
	writer.open();

	for (int i=0; i<maxNumber; i++) {
		// write the new packet to a pcap file
		writer.writePacket(*(newPacket.getRawPacket()));

		// Spielerei zur Zeitanpassung
		zeitstempel.tv_sec++;
		zeitgesetzt = zeitgesetzt && newPacket.getRawPacket()->setPacketTimeStamp(zeitstempel);
		newPacket.computeCalculateFields();
	}

	// close the file
	writer.close();

    // Zeiten ohne Fehlermeldung gesetzt
    EXPECT_EQ(zeitgesetzt,true); 
}