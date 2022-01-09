Dies ist die Implementierung des Tools für die Masterarbeit "Erzeugung von Netzwerkverkehr mit Anomalien".
Die erste Version wurde von Heike Austermann zum 15.10.2021 erstellt.

Für die Lauffähigkeit des Tools sind Voraussetzungen:
- eine aktuelle Linuxumgebung mit einem aktuellen C++-Compiler und für pcapstego aktuellem python inklusive pandas
- PcapPlusPlus in einem Verzeichnis neben dem Tool-Verzeichnis
- ID2T installiert zum Aufruf per ./id2t
- für die Tests: Googletest in einem Verzeichnis neben dem Tool-Verzeichnis

Das Verzeichnis enthält diese README.md, eine LICENSE.txt mit der MIT-Lizenz und vierzehn Unterverzeichnisse:
- aufruf für den Start des Tools
- ressourcen für global genutzte Makefile-Einstellungen
- modul[N]_[XYZXYZ] für den Quellcode zu den fünf Modulen
- tests_modul[N]_[XYZXYZ] für die Tests zu den fünf Modulen, teilweise mit einem Unterverzeichnis und weiteren benötigten Ressourcen
- tests_pcapplusplus für die Tests zur korrekten Einbindung von PcapPlusPlus
- tests_tinyxml2 für die Tests zur korrekten Einbindung von Tiny-XML2


Aufruf des Tools:
```
cd aufruf
make
./dieApp
```

Im Ordner aufruf finden sich jetzt die erzeugten Dateien.

Um alle neu erzeugten Dateien zu löschen, neu zu kompilieren und die App erneut auszuführen:
```
make clean
make
./dieApp
```

Um nur die erzeugten Dateien zu löschen und die App erneut auszuführen:
```
make new
make
./dieApp
```

Aufruf der Tests:
```
cd tests_modul[N]_[XYZXYZ]
make
./tester
```