#include "parameterauspcapgeber.h"
#include "hurstberechner.h"
#include "../modul_0_werkzeugkasten/hexbytes.h"
#include "../modul_0_werkzeugkasten/konfigleser.h"
#include "../modul_0_werkzeugkasten/removeSpaces.h"
#include "Packet.h"
#include "EthLayer.h"
#include "IPv6Layer.h"
#include "TcpLayer.h"
#include "UdpLayer.h"
#include "PayloadLayer.h"
#include "SystemUtils.h"
#include <math.h>
#include <numeric>
#include <iostream>
#include <fstream>

ParameterAusPcapGeber::ParameterAusPcapGeber(std::string eingabeKonfigOrt, std::string eingabeDateiOrt) {
    dateiort = eingabeDateiOrt;
    pcapausgelesen = false;

    verbindunggeschlossennach=0;
    inklusiveLayer2=true;
    nanosekundenproIntervall=100000;
    minimaleIntervallbreite=10;
    maximaleAufteilungen=20;
    anzahlgelesenePakete = 0;
    groessepcap = 0;
    ersterZeitstempel = 0;
    letzterZeitstempel = 0;
    anzahlKetten = 0;
    bytesPayloadberechnung = 96;
    paketePayloadberechnung = 4;

    Konfigleser konfigleser(eingabeKonfigOrt);
    int fehlercode = konfigleser.gebeFehlercode();
    if (fehlercode != 0) {
        std::cout << "Fehlercode beim Auslesen Konfigdatei: " << std::to_string(fehlercode) << std::endl;
        std::cout << "Bitte Konfigdatei überprüfen: " << eingabeKonfigOrt << std::endl;
        std::cout << "Weitere Berechnungen mit Standardwerten." << std::endl;
    } else {
        verbindunggeschlossennach = konfigleser.gebeVerbindungGeschlossenNach();
        inklusiveLayer2 = konfigleser.gebeEinschliesslichLayer2();
        nanosekundenproIntervall = konfigleser.gebeNanosekundenProIntervall();
        minimaleIntervallbreite = konfigleser.gebeMinimaleZahlIntervalle();
        maximaleAufteilungen = konfigleser.gebeMaximaleZahlAufteilungen();
        bytesPayloadberechnung = konfigleser.gebeBytesPayloadberechnung();
        paketePayloadberechnung = konfigleser.gebePaketePayloadberechnung();
        uint32_t gruppenid = 1;
        uint32_t anzahlGruppen = konfigleser.gebeAnzahlGruppen();
        uint32_t position;
        std::string geleseneip;
        while (gruppenid < anzahlGruppen+1) {
            position = 1;
            geleseneip = konfigleser.gebeIP(gruppenid,position);
            while(geleseneip != "") {
                if (ipzugruppe.find(geleseneip) == ipzugruppe.end()) {
                    ipzugruppe.insert({geleseneip,konfigleser.gebeGruppenname(gruppenid)});
                }
                position++;
                geleseneip = konfigleser.gebeIP(gruppenid,position);
            }
            gruppenid++;
        }
    }
}

void ParameterAusPcapGeber::lesePcapAus() {
    pcpp::PcapFileReaderDevice leser(dateiort);
	leser.open();
    if (!leser.open()) {printf("Bitte Dateiangabe des pcap prüfen.\n");exit(1);}

    pcpp::RawPacket rawPacket;
    pcpp::Packet parsedPacket(&rawPacket);
    pcpp::EthLayer* ethernetLayer = parsedPacket.getLayerOfType<pcpp::EthLayer>();
    pcpp::IPv6Layer* ipv6layer = parsedPacket.getLayerOfType<pcpp::IPv6Layer>();
    pcpp::TcpLayer* tcplayer = parsedPacket.getLayerOfType<pcpp::TcpLayer>();
    pcpp::UdpLayer* udplayer = parsedPacket.getLayerOfType<pcpp::UdpLayer>();
    pcpp::Layer* lastlayer = parsedPacket.getLastLayer();
    bool gelesen = leser.getNextPacket(rawPacket);
    long long paketzeitstempel;
    uint32_t paketgroesse;
    std::string macs;
    std::string macd;
    uint16_t ethertype;
    std::string ips;
    std::string ipd;
    uint16_t ports;
    uint16_t portd;
    kettenliste.clear();
    familienliste.clear();
    anzahlKetten = 0;
    size_t pakethash;
    size_t pakethashr;
    uint8_t* payload;
    std::string lastlayerbeschreibung;
    std::string payloadstring;
    size_t payloadlaenge;
    uint32_t payloadspeicherlaenge;

    while (gelesen == true) {
        parsedPacket = &rawPacket;
        zeitstempel = rawPacket.getPacketTimeStamp();
        paketzeitstempel = zeitstempel.tv_sec*1000000+floor(zeitstempel.tv_nsec/1000);
        zeitstempelliste.insert(zeitstempelliste.end(),paketzeitstempel);
        ethernetLayer = parsedPacket.getLayerOfType<pcpp::EthLayer>();      
        if (ethernetLayer != NULL) {
            paketgroesse = ethernetLayer->getDataLen();
            paketgroessenliste.insert(paketgroessenliste.end(),paketgroesse);
            ethertype = pcpp::netToHost16(ethernetLayer->getEthHeader()->etherType);
            macs = RemoveSpaces::removeSpaces(ethernetLayer->getSourceMac().toString());
            macd = RemoveSpaces::removeSpaces(ethernetLayer->getDestMac().toString());
            ipv6layer = parsedPacket.getLayerOfType<pcpp::IPv6Layer>();
            if (ipv6layer != NULL) {
                ips = RemoveSpaces::removeSpaces(ipv6layer->getSrcIPv6Address().toString());
                ipd = RemoveSpaces::removeSpaces(ipv6layer->getDstIPv6Address().toString());
            } else {
                ips = "";
                ipd = "";
            }

            tcplayer = parsedPacket.getLayerOfType<pcpp::TcpLayer>();
            if (tcplayer != NULL) {
                ports = tcplayer->getSrcPort();
                portd = tcplayer->getDstPort();
            } else {
                udplayer = parsedPacket.getLayerOfType<pcpp::UdpLayer>();
                if (udplayer != NULL) {
                    ports = udplayer->getSrcPort();
                    portd = udplayer->getDstPort();
                } else {
                    ports = 0;
                    portd = 0;
                }
            }

            lastlayer = parsedPacket.getLastLayer();
            if (lastlayer != NULL) {
                payload = lastlayer->getData();
                if (payload != NULL) {
                    payloadlaenge = lastlayer->getDataLen();
                    lastlayerbeschreibung = lastlayer->toString();
                    if (lastlayerbeschreibung.find("Data") != std::string::npos) {
                        payloadspeicherlaenge = ((payloadlaenge+10)>bytesPayloadberechnung)? bytesPayloadberechnung:(payloadlaenge+10);
                        size_t pos = 10;
                        while (pos < payloadspeicherlaenge) {
                            payloadstring.append(Hexbytes::hexbyte(payload[pos]));
                            pos++;
                        }

                    }
                }
            }

            lesbarliste.insert(lesbarliste.end(),true);
        } else {
            paketgroessenliste.insert(paketgroessenliste.end(),0);
            ethertype = 0;
            macs = "";
            macd = "";
            ips = "";
            ipd = "";
            ports = 0;
            portd = 0;
            lesbarliste.insert(lesbarliste.end(),false);
        }

        pakethash =(inklusiveLayer2)? Kette::berechneHash(ethertype,macs,macd,ips,ipd,ports,portd):Kette::berechneHash(ips,ipd,ports,portd);
        pakethashr =(inklusiveLayer2)? Kette::berechneHash(ethertype,macd,macs,ipd,ips,portd,ports):Kette::berechneHash(ipd,ips,portd,ports);
        if (verbindunggeschlossennach == 0) {
            if (kettenliste.count(pakethash) == 1) {
                kettenliste.at(pakethash).at(0).ergaenzePakethin(paketzeitstempel,paketgroesse,payloadstring);
                familienliste.insert(familienliste.end(),{pakethash,0});
            } else {       
                if (kettenliste.count(pakethashr)==1) {
                    kettenliste.at(pakethashr).at(0).ergaenzePaketrueck(paketzeitstempel,paketgroesse,payloadstring);
                    familienliste.insert(familienliste.end(),{pakethashr,0});
                } else {
                    if (inklusiveLayer2) {
                        Kette kette(pakethash,pakethash,pakethashr,paketzeitstempel,paketgroesse,payloadstring,paketePayloadberechnung,ethertype,macs,macd,ips,ipd,ports,portd);
                        familie.clear(); familie.insert({0,kette});
                        kettenliste.insert({pakethash,familie});
                        anzahlKetten++;
                    } else {
                        Kette kette(pakethash,pakethash,pakethashr,paketzeitstempel,paketgroesse,payloadstring,paketePayloadberechnung,ips,ipd,ports,portd);
                        familie.clear(); familie.insert({0,kette});
                        kettenliste.insert({pakethash,familie});
                        anzahlKetten++;
                    }               
                    familienliste.insert(familienliste.end(),{pakethash,0});
                }
            }
        } else {
            if (kettenliste.count(pakethash) > 0) {
                if (kettenliste.at(pakethash).rbegin()-> second.gebeLetzterzeitstempel() < (paketzeitstempel - verbindunggeschlossennach)) {
                    if (inklusiveLayer2) {
                        Kette kette(pakethash,pakethash,pakethashr,paketzeitstempel,paketgroesse,payloadstring,paketePayloadberechnung,ethertype,macs,macd,ips,ipd,ports,portd);
                        kettenliste.at(pakethash).insert({paketzeitstempel,kette});
                        anzahlKetten++;
                    } else {
                        Kette kette(pakethash,pakethash,pakethashr,paketzeitstempel,paketgroesse,payloadstring,paketePayloadberechnung,ips,ipd,ports,portd);
                        kettenliste.at(pakethash).insert({paketzeitstempel,kette});
                        anzahlKetten++;
                    }               
                    familienliste.insert(familienliste.end(),{pakethash,paketzeitstempel});
                } else {
                    kettenliste.at(pakethash).rbegin()->second.ergaenzePakethin(paketzeitstempel,paketgroesse,payloadstring);
                    familienliste.insert(familienliste.end(),{pakethash,kettenliste.at(pakethash).end()->second.gebeErsterzeitstempel()});
                }
            } else {
                if (kettenliste.count(pakethashr) > 0) {
                    if (kettenliste.at(pakethashr).rbegin()->first < (paketzeitstempel -verbindunggeschlossennach)) {
                        if (inklusiveLayer2) {
                            Kette kette(pakethashr,pakethashr,pakethash,paketzeitstempel,paketgroesse,payloadstring,paketePayloadberechnung,ethertype,macs,macd,ips,ipd,ports,portd);
                            kettenliste.at(pakethashr).insert({paketzeitstempel,kette});
                            anzahlKetten++;
                        } else {
                            Kette kette(pakethashr,pakethashr,pakethash,paketzeitstempel,paketgroesse,payloadstring,paketePayloadberechnung,ips,ipd,ports,portd);
                            kettenliste.at(pakethashr).insert({paketzeitstempel,kette});
                            anzahlKetten++;
                        }               
                        familienliste.insert(familienliste.end(),{pakethashr,paketzeitstempel});
                    } else {
                    kettenliste.at(pakethashr).rbegin()->second.ergaenzePaketrueck(paketzeitstempel,paketgroesse,payloadstring);
                    familienliste.insert(familienliste.end(),{pakethashr,kettenliste.at(pakethashr).end()->second.gebeErsterzeitstempel()});
                    }
                } else {
                    if (inklusiveLayer2) {
                        Kette kette(pakethash,pakethash,pakethashr,paketzeitstempel,paketgroesse,payloadstring,paketePayloadberechnung,ethertype,macs,macd,ips,ipd,ports,portd);
                        familie.clear(); familie.insert({paketzeitstempel,kette});
                        kettenliste.insert({pakethash,familie});
                        anzahlKetten++;
                    } else {
                        Kette kette(pakethash,pakethash,pakethashr,paketzeitstempel,paketgroesse,payloadstring,paketePayloadberechnung,ips,ipd,ports,portd);
                        familie.clear(); familie.insert({paketzeitstempel,kette});
                        kettenliste.insert({pakethash,familie});
                        anzahlKetten++;
                    }               
                    familienliste.insert(familienliste.end(),{pakethash,paketzeitstempel});  
                }
            }
        }
        payloadstring.clear();
        gelesen = leser.getNextPacket(rawPacket);
    }
    anzahlgelesenePakete = zeitstempelliste.size();
    groessepcap = std::accumulate(paketgroessenliste.begin(),paketgroessenliste.end(),0l);
    zeitstempelliste.shrink_to_fit();

    leser.close();

    pcapausgelesen = true;
}

long long ParameterAusPcapGeber::gebeErstenZeitstempel() {
    if (!pcapausgelesen) {lesePcapAus();}
    return zeitstempelliste.front();
}

long long ParameterAusPcapGeber::gebeLetztenZeitstempel() {
    if (!pcapausgelesen) {lesePcapAus();}
    return zeitstempelliste.back();
}

unsigned long ParameterAusPcapGeber::gebeAnzahlPakete() {
    if (!pcapausgelesen) {lesePcapAus();}
    return anzahlgelesenePakete;
}

unsigned long ParameterAusPcapGeber::gebeGroessePcap() {
    if (!pcapausgelesen) {lesePcapAus();}
    return groessepcap;
}

unsigned long ParameterAusPcapGeber::gebeAnzahlKetten() {
    if (!pcapausgelesen) {lesePcapAus();}
    return anzahlKetten;
}

bool ParameterAusPcapGeber::schreibeAktuelleKettenAus(std::string eingabeDateiortKettenaktuell) {
    if (!pcapausgelesen) {lesePcapAus();}
    bool rueckgabe = false;
    std::string Dateiort = eingabeDateiortKettenaktuell.append(".csv");
    std::ofstream ausgabe(Dateiort);
    for (auto& familie : kettenliste) {
        for (auto& kette : familie.second) {
            ausgabe << kette.second.gebeWertezeile() << "\n";
            rueckgabe = true;
        }
    }
    ausgabe.close();
    return rueckgabe;
}

void ParameterAusPcapGeber::schreibeGlobaleHurstParameterAus(std::string eingabeDateiortHurstglobal) {
    if (!pcapausgelesen) {lesePcapAus();}
    std::string Dateiort = eingabeDateiortHurstglobal.append(".txt");
    std::ofstream ausgabe(Dateiort);
    ausgabe << "Datei: " << dateiort << "\n";
    ausgabe << "Hurst für Zwischenankunftszeiten: " << gebeHurstZwischenankunftszeiten(minimaleIntervallbreite,maximaleAufteilungen) << "\n";
    ausgabe << "Hurst für Zeitintervalle: " << gebeHurstZeitintervalle(nanosekundenproIntervall,minimaleIntervallbreite,maximaleAufteilungen) << "\n";
    ausgabe << "Hurst für Größenintervalle: " << gebeHurstGroessenintervalle(nanosekundenproIntervall,minimaleIntervallbreite,maximaleAufteilungen) << "\n"; 
    ausgabe.close();
}

bool ParameterAusPcapGeber::schreibeAllesAus(std::string eingabeDateiortKetten,std::string eingabeDateiortHurstglobal) {
    schreibeGlobaleHurstParameterAus(eingabeDateiortHurstglobal);
    ergaenzeKettenHurst(nanosekundenproIntervall,minimaleIntervallbreite,maximaleAufteilungen);
    ergaenzeKettenGruppennamen();
    bool rueckgabe = schreibeAktuelleKettenAus(eingabeDateiortKetten);
    return rueckgabe;
}

bool ParameterAusPcapGeber::ergaenzeKettenGruppennamen() {
    if (!pcapausgelesen) {lesePcapAus();}
    bool rueckgabe = false;
    std::string quellip;
    std::string quellgruppe = "";
    std::string zielip;
    std::string zielgruppe = "";
    for (auto& familie : kettenliste) {
        for (auto& kette : familie.second) {
            quellip = kette.second.gebeIPquelle();
            if (ipzugruppe.find(quellip) == ipzugruppe.end()) {
                quellgruppe = "";
                } else {
                    quellgruppe = ipzugruppe[quellip];
                }
            zielip = kette.second.gebeIPziel();
            if (ipzugruppe.find(zielip) == ipzugruppe.end()) {
                zielgruppe = "";
            } else {
                zielgruppe = ipzugruppe[zielip];
            }
            kette.second.setzeGruppen(quellgruppe,zielgruppe);
            rueckgabe = true;
        }
    }
    return rueckgabe;
}

bool ParameterAusPcapGeber::ergaenzeKettenHurst(long long nanosekundenintervall, uint32_t intervallbreite, uint32_t maximaleAufteilungen) {
    if (!pcapausgelesen) {lesePcapAus();}
    bool rueckgabe = false;
    std::vector<long long> zeitenliste;
    std::vector<uint32_t> groessenliste;
    double hurstAnzahl;
    double hurstGroesse;
    for (auto& familie : kettenliste) {
        for (auto& kette : familie.second) {
            for (size_t i=0; i<zeitstempelliste.size(); i++) {
                if ((familienliste[i].hash==familie.first)&&(familienliste[i].anfang==kette.first)) {
                    zeitenliste.insert(zeitenliste.end(),zeitstempelliste[i]);
                    groessenliste.insert(groessenliste.end(),paketgroessenliste[i]);
                }
            }
            std::vector<uint32_t> zeitenfuerhurst = erzeugeZaehlerliste(zeitenliste,nanosekundenintervall);
            std::vector<uint32_t> groessenfuerhurst = erzeugeGroessenliste(zeitenliste,groessenliste,nanosekundenintervall);
            hurstAnzahl = Hurstberechner::anisLloyd(intervallbreite,maximaleAufteilungen,zeitenfuerhurst);
            hurstGroesse = Hurstberechner::anisLloyd(intervallbreite,maximaleAufteilungen,groessenfuerhurst);
            kette.second.setzeHurst(hurstAnzahl,hurstGroesse);
            zeitenliste.clear();
            groessenliste.clear();
            rueckgabe = true;
        }
    }
    return rueckgabe;
}

double ParameterAusPcapGeber::gebeHurstZeitintervalle(long long nanosekundenintervall, uint32_t intervallbreite, uint32_t maximaleAufteilungen) {
    if (!pcapausgelesen) {lesePcapAus();}
    std::vector<uint32_t> zeitenfuerhurst = erzeugeZaehlerliste(zeitstempelliste,nanosekundenintervall);
    double hurst = Hurstberechner::anisLloyd(intervallbreite,maximaleAufteilungen,zeitenfuerhurst);
    return hurst;
}

double ParameterAusPcapGeber::gebeHurstGroessenintervalle(long long nanosekundenintervall, uint32_t intervallbreite, uint32_t maximaleAufteilungen) {
    if (!pcapausgelesen) {lesePcapAus();}
    std::vector<uint32_t> groessenfuerhurst = erzeugeGroessenliste(zeitstempelliste,paketgroessenliste,nanosekundenintervall);
    double hurst = Hurstberechner::anisLloyd(intervallbreite,maximaleAufteilungen,groessenfuerhurst);
    return hurst;
}

double ParameterAusPcapGeber::gebeHurstZwischenankunftszeiten(uint32_t intervallbreite, uint32_t maximaleAufteilungen) {
    if (!pcapausgelesen) {lesePcapAus();}
    std::vector<uint32_t> zeitenfuerhurst;
    uint32_t differenz;
    for (unsigned long i=0; i<zeitstempelliste.size()-1; i++) {
        differenz = zeitstempelliste[i+1]-zeitstempelliste[i];
        zeitenfuerhurst.insert(zeitenfuerhurst.end(),differenz);
    }
    double hurst = Hurstberechner::anisLloyd(intervallbreite,maximaleAufteilungen,zeitenfuerhurst);
    return hurst;
}

std::vector<uint32_t> ParameterAusPcapGeber::erzeugeZaehlerliste(std::vector<long long> zeitenliste, long long zeitintervall) {
    std::vector<uint32_t> ausgabe;
    uint32_t anzahl = 0;
    size_t anzahllistenelemente = zeitenliste.size();
    long long schranke = zeitenliste.front() + zeitintervall;
    for (unsigned long i=0; i<anzahllistenelemente; i++) {
        if (zeitenliste[i] < schranke) {
            anzahl++; 
        }
        else {
            ausgabe.insert(ausgabe.end(),anzahl);            
            anzahl = 0;
            schranke = schranke + zeitintervall;
            while (zeitenliste[i] == schranke || zeitenliste[i] > schranke) {
                ausgabe.insert(ausgabe.end(),anzahl);
                schranke = schranke + zeitintervall;
            }
            anzahl++;
        }
    }
    return ausgabe;
}

std::vector<uint32_t> ParameterAusPcapGeber::erzeugeGroessenliste(std::vector<long long> zeitenliste, std::vector<uint32_t> groessenliste, long long zeitintervall) {
    std::vector<uint32_t> ausgabe;
    uint32_t groesse = 0;
    size_t anzahllistenelemente = groessenliste.size();
    long long schranke = zeitenliste.front() + zeitintervall;
    for (unsigned long i=0; i<anzahllistenelemente; i++) {
        if (zeitenliste[i] < schranke) {
            groesse = groesse + groessenliste[i];
        }
        else {
            ausgabe.insert(ausgabe.end(),groesse);            
            groesse = 0;
            schranke = schranke + zeitintervall;
            while (zeitenliste[i] == schranke || zeitenliste[i] > schranke) {
                ausgabe.insert(ausgabe.end(),groesse);
                schranke = schranke + zeitintervall;
            }
            groesse = groesse + groessenliste[i];
        }
    }
    return ausgabe;
}
