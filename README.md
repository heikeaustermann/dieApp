Dies ist die Implementierung des Tools für die Masterarbeit "Erzeugung von Netzwerkverkehr mit Anomalien".
Die erste Version wurde von Heike Austermann zum 15.10.2021 erstellt.

Für die Lauffähigkeit des Tools sind Voraussetzungen:
- eine aktuelle Linuxumgebung mit einem aktuellen C++-Compiler und für pcapstego aktuellem python inklusive pandas und scapy
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


##### Aufruf und Nutzung des Tools:
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
./dieApp
```

Aufruf der Tests:
```
cd tests_modul[N]_[XYZXYZ]
make
./tester
```

Anmerkung: bei der Anwendung sollte berücksichtigt werden, dass die Bereinigungen die erzeugten und angepassten pcap-Dateien entfernen.

Auf Grund der Anforderungen in der Anwendungsanalyse zur Nutzung unter kontrollierten Bedingungen in einer wohlmeinenden Umgebung sind keine besonderen Maßnahmen zur Härtung oder Fehlerbehandlung getroffen worden. Das Tool läuft bei Eingabe zulässiger Parameter fehlerfrei.

##### Modul 0 Werkzeugkasten

Dieses Modul enthält die gesammelten global benötigten Funktionalitäten wie den Konfigleser, der das Auslesen der Konfigurationsdatei realisiert, die Fremdsoftware TinyXML-2, mehrere selbst erstellte Format-Umwandlungsbibliotheken, stringexec für das direkte Aufrufen von Linux-Befehlen analog der Befehlszeile sowie die Sammlung der Schlüsselwörter.

Bei Umsetzung des Konfiglesers wurde von einer Expertennutzung und wie in der Anforderungsanalyse bestimmt von einer wohlwollenden Umgebung ausgegangen. Das heißt der Konfigleser führt keine gesonderten Prüfungen der Datei und ihrer Inhalte aus und gibt stets 0, den leeren String oder "undefined“ als Rückgabe, wenn eine Angabe in der Konfigurationsdatei nicht definiert wurde.
Näheres zu TinyXML-2 findet sich im Abschnitt ”Übernommene Softwarekomponenten“.

Die Format-Umwandlungsbibliotheken bestehen aus hexbytes, das Zahlen in Zeichenketten mit den zugehörigen Hexadezimalzahlen umwandelt, sowie removespaces, das vom Konfigleser bei Bereinigung der Eingaben genutzt wird, um Leerzeichen zu entfernen.
Sowohl stringexec als auch removespaces hängen stark von der lokalen Implementierung ab und es wird daher zumindest für diese beiden Bibliotheken eine lokale Kompilierung empfohlen. (Aktuell ist eine vollständige lokale Kompilierung umgesetzt.)

##### Modul 1 Pcap-Erzeuger

Dieses Modul erzeugt eine pcap-Datei mit einem mitgegebenen Namen nach den in der Konfigurationsdatei ausgelesenen Parametern.

Die Objekte Kettendef und Abfolge sind reine Datencontainer. Kettendef enthält eine aus der Konfigurationsdatei ausgelesene Kettendefinition. Diese besteht aus den jeweiligen Gruppenzugehörigkeiten für Quelle und Ziel, dem umzusetzenden Verfahren und ggf. zeitlichen Einschränkungen bei ihrem Auftreten. Abfolge enthält eine tatsächliche Realisierung einer Kette mit all ihren bei Erzeugung zufällig bestimmten Parametern (inklusive den Adressen aus den Gruppen und aus dem Verfahren sich ergebenden Ports) und dem nächsten bestimmten Zeitstempel.

Die Bibliothek Intervallgeber stellt mehrere Möglichkeiten bereit, eine Zufallszahl zu erhalten. Es können eine exponentielle und eine uniforme Verteilung sowie zwei fraktale Verteilungen als Grundlage ausgewählt werden. Die fraktalen Verteilungen unterscheiden sich nur darin, dass die eine einen Maximalwert beachtet.

Die Objekte Zeitstempelgeber dienen zur Bestimmung des nächsten Zeitstempels und werden bei Nutzung je einer Abfolge zugeordnet. Ein Zeitstempelgeber ist entweder fraktal, dann erzeugt er eine fraktale, selbstähnliche Folge von Zeitstempeln an Hand der vorgegebenen Parameter, oder er ist nicht fraktal, dann erzeugt er eine regelmäßige Folge von Zeitstempeln mit zufälligen kleinen Abweichungen. Die beiden Arten von Zeitstempelgebern entsprechen den beiden grundsätzlich verschiedenen Verhaltensweisen von nutzerbeeinflusstem versus maschinenbeeinflusstem Datenverkehr.
Ein Objekt Zeitstempelgeber erzeugt den nächsten Zeitstempel in der Folge unter Nutzung der passenden Funktion des Intervallgebers, indem der gelieferte Wert als Intervall zum vorherigen Zeitstempel addiert wird.

Das Objekt Pcaperzeuger bildet die Steuerung des Moduls. Es liest die Konfiguration aus, füllt die Randbedingungen wie maximale Paketanzahl und Zeitraum sowie die internen Vektoren mit Kettendefinitionen. Weiterhin erstellt es für den Schreibvorgang die Vektoren mit Abfolgen, den zugehörigen Zeitstempelgebern und nächsten Zeitstempeln, bestimmt die nächste zu beachtende Abfolge, sammelt deren Eigenschaften zum Erstellen des zugehörigen Pakets in die pcap-Datei, erstellt den nächsten Zeitstempel und schreibt letztlich das so festgelegte Paket in die Zieldatei.

##### Modul 2 Pcap-Ergänzer

Dieses Modul ergänzt eine vorliegende pcap-Datei um Anomalien und speichert diese als ergänzte pcap-Datei.

Es besteht aus dem Objekt PcapErgaenzer. Bei Aufruf der ergaenze-Funktion werden die anzupassenden pcap-Dateien umkopiert, um die Originale zu erhalten, aus der Konfigurationsdatei die umzusetzende Funktionalität entnommen und diese schließlich aufgerufen.

##### Modul 3 Pcap-Änderer

Dieses Modul ändert eine vorliegende pcap-Datei, indem simuliertes Rauschen hinzugefügt wird und speichert diese als geänderte pcap-Datei.

Es besteht aus dem Modul PcapAenderer mit einem analogen äußeren Aufbau zu PcapErgaenzer mit Funktion aendere. Die umzusetzende Änderung ergibt sich aus den folgenden Dimensionen, die aus der Konfigurationsdatei übernommen werden: Es werden
- für alle Pakete oder nur für solche mit Adressen aus bestimmten Gruppen (je fur Quelle und Ziel getrennt festgelegt)
- mit bestimmter Wahrscheinlichkeit
- in einem festgelegten Zeitfenster
- Pakete gelöscht, verschieben oder gedoppelt (ggf. mit festgelegtem Intervall für Verzögerung).

Aktuell ist diese Funktionalität nur nutzbar, wenn die OSI-Schicht 2 betrachtet wird, da es sich bei den umgesetzten Rauschen-Auswirkungen um Probleme handelt, die tendenziell hardwarenäher und in dieser Schicht begründet sind. Das Tool speichert dabei die geschobenen Pakete zwischen und fügt diese einzeln beim Durchlauf immer an der korrekten neuen Stelle in der geänderten pcapDatei ein, so dass sich eine saubere Reihenfolge nach Zeitstempeln ergibt.

##### Modul 4 Parameter-Ausleser

Dieses Modul liest eine vorhandene pcap-Datei aus und erzeugt eine tabellarisch zusammengefasste Übersicht der einzelnen Verbindungen und/ oder eine Datei mit globalem Hurstparameter.

Das Objekt Kette dient als Container für eine ausgelesene Kette. Eine Kette ist bestimmt als die Menge aller Pakete in der pcap-Datei, die dieselbe Adressen mit denselben Ports aufweisen (Quelle und Ziel können naturlich vertauscht sein; die Pakete werden dann in einer Kette zusammengefasst aber für getrennte Richtungen gezählt). Wenn der Parameter "connection assume closed after" in der Konfigurationsdatei auf einen positiven Wert gesetzt ist, wird eine Kette in zwei Ketten aufgeteilt, sobald eine Pause zwischen einzelnen Paketen vorliegt, die größer oder gleich dem festgelegten Wert ist.
Der Begriff Kette wurde bewusst gewählt, da dieser Begriff in vorliegendem Kontext nicht mit einer eigenen Bedeutung belegt ist und somit nicht weitere Eigenschaften unterstellt werden, wie dies beispielsweise bei Verwendung des Begriffs ”Konversation“ wäre.

Die beiden Bibliotheken Entropieberechner und Hurstberechner stellen die Funktionen zur Bestimmung der Entropie für einen Hexzahl-gefüllten String und des Hurst-Parameters für eine Zahlenfolge in den beiden Verfahren klassisch nach Hurst und korrigiert nach Anis-LLoyd bereit. (Die Grundlage der Rechenverfahren ist im Hintergrundkapitel beschrieben.)
Die tabellarisch zusammengefasste Ubersicht mit den Kennzahlen enthält neben grundlegenen Eigenschaften bei vorheriger Berechnung die Hurst-Parameter für die Paketanzahl und den Leitungsdurchsatz sowie die Entropie der Payloads einer Kette. Die Beschränkung auf die grundlegenden Eigenschaften wie Quelle, Ziel, Zeitraum, Paketanzahl und Datenmenge wurde umgesetzt, da es bereits eine Vielzahl von Programmen zur Parametererstellung gibt.

##### Übernommene Softwarekomponenten

Zwei C++-Bibliotheken werden direkt im Programm genutzt. Dabei handelt es sich um TinyXML-2 zum Auslesen der Konfigurationsdatei und PcapPlusPlus für das Auslesen und Schreiben der pcap-Dateien.
Beide Bibliotheken sind open source, stehen auf github zur Verfügung und haben eine Lizenz, die eine Nutzung in diesem Kontext zulässt.
TinyXML-2 steht unter der ”zlib“-Lizenz, die auch eine kommerzielle Verwendung als Teil eines Programms zulässt, solange der Quellcode mit allen Hinweisen zum Ursprung unverändert erhalten bleibt oder Anderungen eindeutig gekennzeichnet werden.
PcapPlusPlus wird unter einer so betitelten ”Unlicense“ veröffentlicht, die keine Bedingungen stellt und explizit nur jede Gewährleistung ausschließt.

Die beiden Programme id2t und pcapstego werden über im Programm eingebundene Befehlszeilen über die Kommandozeile der Linux-Umgebung aufgerufen. Beide Programme können in Modul 2 aufgerufen werden und ergänzen ein pcap um Anomalien. Es werden die Standardinstallationsparameter vorausgesetzt. Die entsprechenden Strings für den Aufruf lassen sich ggf. im Quelltext anpassen.
Die Verwendung einer indirekten Einbindung über den Aufruf per Befehlszeile und kein direktes Einbinden als Kompenente ist so gewählt, weil beide Programme keine explizite API für einen Zugriff angeboten haben und zum zweiten relativ jung sind und sich damit potenziell noch stark ändern können. Eine Einbindung über einen indirekten Aufruf ist somit sowohl zukunftssicherer (der Befehl zum Aufruf ändert sich üblicherweise nicht mehr oder nur selten) als auch einfacher anzupassen (bei externem Aufruf wird nur genau diese eine Zeile angepasst).