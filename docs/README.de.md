# xTrainAPI - Einheitliche Modellbahnsteuerung

Willkommen beim xTrainAPI-Projekt! Dieses Repository enthält eine C++-basierte API zur Steuerung von digitalen Modelleisenbahnen und deren Zubehör.

## Zusammenfassung

xTrainAPI ist eine einheitliche Schnittstelle, die entwickelt wurde, um verschiedene digitale Modellbahnsysteme über eine einzige, konsistente API zu steuern. Sie vereinfacht die Entwicklung von Steuerungssoftware, indem sie die Komplexität einzelner Protokolle wie DCC und Märklin Motorola abstrahiert.

Die API ist in C++ als abstrakte Schnittstelle (`IUnifiedModelTrainListener`) definiert und dient als „Single Source of Truth“. Alle anderen Artefakte, einschließlich XML-Schemata, der OpenAPI-Spezifikation und Hilfsklassen, werden von dieser zentralen C++-Header-Datei abgeleitet. Dieses Repository enthält die Kern-API, die Dokumentation und eine Reihe von Beispielen, die Ihnen den Einstieg erleichtern.

## Dokumentation

Unsere API-Dokumentation wird automatisch generiert und auf GitHub Pages gehostet.

*   **Doxygen (C++ API):** Eine detaillierte Dokumentation der C++-Schnittstelle, Klassen und Methoden finden Sie hier:
    [Doxygen-Dokumentation](https://chatelao.github.io/xDuinoRails_xTrainAPI/doxygen)

*   **Swagger (REST API):** Die Spezifikation für unsere REST-Schnittstelle, einschließlich interaktiver Endpunkte, ist hier verfügbar:
    [Swagger UI](https://chatelao.github.io/xDuinoRails_xTrainAPI/swagger)

*   **Web-Tool:** Eine einfache Webanwendung zur Interaktion mit der xTrainAPI ist hier verfügbar:
    [Web-Tool](https://chatelao.github.io/xDuinoRails_xTrainAPI/webtool)

## Kernkonzepte

Die API ist in C++ als abstrakte Schnittstelle (`IUnifiedModelTrainListener`) definiert und dient als „Single Source of Truth“. Alle anderen Artefakte, wie die XML-Schemata und die OpenAPI-Spezifikation, werden von dieser C++-Header-Datei abgeleitet.

Für ein tieferes Verständnis der in der API verwendeten Begriffe und Datenfelder werfen Sie bitte einen Blick auf unser [Glossar](XTTRAIN_GLOSSARY.de.md) und das [Datenwörterbuch](DATA_DICTIONARY.de.md).

## Repository-Struktur

Das Repository ist wie folgt organisiert:

-   `/src`: Enthält die C++-API-Hauptdefinition und Hilfsklassen (einschließlich `yxml`).
-   `/docs`: Enthält zusätzliche Dokumentation, wie z. B. die CLI-Syntax.
-   `/examples`: Arduino-Sketches, die die Verwendung der API demonstrieren.
-   `/json`: JSON-Schemata für die API-Ereignisse.
-   `/swagger`: Die OpenAPI 3.0-Spezifikation für die REST-API.
-   `/webtool`: Ein webbasiertes Tool zur Interaktion mit der API.
-   `/xml`: XML-Schemata und Beispiele.

## Erste Schritte

Um die xTrainAPI in Ihren eigenen Arduino-Projekten zu verwenden, folgen Sie diesen Schritten:

1.  **Installation der Bibliothek:**
    *   Laden Sie das Repository als ZIP-Datei herunter.
    *   Gehen Sie in der Arduino IDE zu **Sketch** -> **Bibliothek einbinden** -> **.ZIP-Bibliothek hinzufügen...** und wählen Sie die heruntergeladene Datei aus.
    *   Alternativ können Sie die Bibliothek manuell installieren, indem Sie den Repository-Ordner in Ihren Arduino-`libraries`-Ordner kopieren.

2.  **Schnittstelle implementieren:** Erstellen Sie in Ihrem Sketch eine neue Klasse, die von `IUnifiedModelTrainListener` erbt, und implementieren Sie die Methoden für die Ereignisse, die Sie behandeln möchten.
3.  **Ereignisse verarbeiten:** In Ihrer `loop()`-Funktion können Sie eine der Hilfsklassen (wie `CmdLineParser` oder `XmlParser`) verwenden, um eingehende Daten zu parsen und die entsprechenden Ereignisse in Ihrer Listener-Klasse auszulösen.

## Ausführen der Beispiele

Das `/examples`-Verzeichnis enthält mehrere Sketches, die die Funktionalität der API demonstrieren. Um sie auszuführen, müssen Sie die Arduino IDE oder `arduino-cli` installiert haben.

1.  Öffnen Sie einen der Beispiel-Sketches (z. B. `examples/debug_cli/debug_cli.ino`) in der Arduino IDE.
2.  Wählen Sie Ihr Board und Ihren Port aus.
3.  Laden Sie den Sketch auf Ihren Arduino.
4.  Öffnen Sie den Seriellen Monitor, um mit dem Beispiel zu interagieren.

## Mitwirken

Wir freuen uns über Beiträge! Bitte lesen Sie unsere `AGENTS.md`, bevor Sie Änderungen am Code vornehmen.
