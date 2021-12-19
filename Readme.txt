Dies ist die Readme.txt zur Abgabeversion der Implementierung des Tools für die Masterarbeit "Erzeugung von Netzwerkverkehr mit Anomalien".
Sie wurde von Heike Austermann im Studiengang Master Praktische Informatik (PO-Version 17) zum 15.10.2021 erstellt.

Für die Lauffähigkeit des Tools sind Voraussetzungen:
- eine aktuelle Linuxumgebung mit einem aktuellen C++-Compiler und für pcapstego aktuellem python
- PcapPlusPlus in einem Verzeichnis neben dem Tool-Verzeichnis
- ID2T installiert zum Aufruf per ./id2t
- für die Tests: Googletest in einem Verzeichnis neben dem Tool-Verzeichnis

Das Verzeichnis enthält diese Readme.txt, das PDF der Masterarbeit und dreizehn Unterverzeichnisse:
- aufruf für den Start des Tools
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


Theoretische Hintergründe sind im PDF der Masterarbeit zu finden, hier die beiden praktischen Referenzen:

Referenz zum Testverfahren:

@book{2013Langr,
    author = {Langr, Jeff},
    title = {Modern C++ Programming with Test-Driven Development: Code Better, Sleep Better},
    year = {2013},
    publisher = {Pragmatic Bookshelf},
    isbn = {1937785483}
}

Referenz zum Refactoring:

@book{2018Fowler,
    author = {Fowler, Martin},
    title = {Refactoring - Improving the Design of Existing Code},
    edition = 2,
    year = {2018},
    publisher = {Addison-Wesley},
    isbn = {978-0-13-475759-9}
}