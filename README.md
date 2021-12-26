Dies ist die Implementierung des Tools für die Masterarbeit "Erzeugung von Netzwerkverkehr mit Anomalien".
Die erste Version wurde von Heike Austermann im Studiengang Master Praktische Informatik (PO-Version 17) zum 15.10.2021 erstellt.

Für die Lauffähigkeit des Tools sind Voraussetzungen:
- eine aktuelle Linuxumgebung mit einem aktuellen C++-Compiler und für pcapstego aktuellem python
- PcapPlusPlus in einem Verzeichnis neben dem Tool-Verzeichnis
- ID2T installiert zum Aufruf per ./id2t
- für die Tests: Googletest in einem Verzeichnis neben dem Tool-Verzeichnis

Das Verzeichnis enthält diese README.md, eine License.txt und vierzehn Unterverzeichnisse:
- aufruf für den Start des Tools
- ressourcen für global genutzte Makefile-Einstellungen
- modul[N]_[XYZXYZ] für den Quellcode zu den fünf Modulen
- tests_modul[N]_[XYZXYZ] für die Tests zu den fünf Modulen, teilweise mit einem Unterverzeichnis und weiteren benötigten Ressourcen
- tests_pcapplusplus für die Tests zur korrekten Einbindung von PcapPlusPlus
- tests_tinyxml2 für die Tests zur korrekten Einbindung von Tiny-XML2


Aufruf des Tools:
cd aufruf
make
./dieApp


Aufruf der Tests:
cd tests_modul[N]_[XYZXYZ]
make
./tester
