# Konzept: Zustandsgesteuertes Event-Scripting mit PAWN

Dieses Dokument beschreibt ein Konzept zur Integration der Skriptsprache PAWN in das xTrainAPI-Ökosystem, um benutzerdefinierte Automatisierung und zustandsbehaftete Ereignisbehandlung zu ermöglichen.

## 1. Ziele

*   **Benutzererweiterbarkeit:** Ermöglichen, dass Endbenutzer benutzerdefinierte Logik und Automatisierungsskripte schreiben können, ohne die Kern-C++-Firmware ändern zu müssen.
*   **Zustandsbehaftete Logik:** Die Erstellung von Skripten ermöglichen, die sich den Zustand zwischen Ereignissen merken können (z. B. "wenn ein Zug Sensor A passiert, dann Sensor B, stelle Weiche C").
*   **Entkopplung:** Trennung der benutzerdefinierten Logik von der Kern-API-Implementierung, um die Wartbarkeit zu verbessern.
*   **Ressourceneffizienz:** Verwendung einer Skriptsprache mit geringem Ressourcenbedarf, die für ressourcenbeschränkte Mikrocontroller wie Arduino geeignet ist.

## 2. Kernkonzept

Die Kernidee besteht darin, die PAWN Virtual Machine (VM) in die Haupt-Arduino-Firmware einzubetten. Diese VM führt vorkompilierten PAWN-Bytecode (`.amx`-Dateien) aus, der auf einem vom Mikrocontroller zugänglichen Dateisystem (z. B. einer SD-Karte oder SPIFFS) gespeichert ist.

Eine "Binding"-Schicht wird eine Teilmenge der xTrainAPI-Funktionen für die PAWN-Skriptumgebung verfügbar machen. Wenn ein Ereignis in der xTrainAPI auftritt, wird es an eine öffentliche Funktion im laufenden PAWN-Skript weitergeleitet. Das Skript kann dann zurück in den nativen C++-Code rufen, um Aktionen auszulösen.

## 3. Architektur

### 3.1. PAWN Virtual Machine (VM)

Die PAWN-VM ist ein leichtgewichtiger Interpreter, der den kompilierten Bytecode ausführt. Sie wird als C++-Bibliothek in die Haupt-Firmware integriert. Die VM ist für das Laden und Ausführen des Bytecodes verantwortlich.

### 3.2. Bytecode-Speicherung

Kompilierte PAWN-Skripte (`.amx`-Dateien) werden auf einem Dateisystem gespeichert. Die Firmware erwartet eine bestimmte Datei, wie z. B. `main.amx`, im Stammverzeichnis des Dateisystems, die beim Start geladen wird.

### 3.3. Native Funktionsbindung

Eine C++-Bindungsschicht wird erstellt, um Kernfunktionalitäten der xTrainAPI für die PAWN-Umgebung bereitzustellen. Dies sind "native" Funktionen, die von PAWN-Skripten aufgerufen werden können.

**Beispiel C++ (Bindung):**

```cpp
// Diese Funktion wird von PAWN aufrufbar sein
cell AMX_NATIVE_CALL set_loco_speed(AMX* amx, cell* params) {
    int locoId = params[1];
    int speed = params[2];
    // Aufruf der eigentlichen xTrainAPI-Funktion
    // IUnifiedModelTrainListener::onLocoSpeedChange(locoId, speed);
    return 0;
}
```

### 3.4. Ereignisbehandlung

xTrainAPI-Ereignisse werden an das PAWN-Skript weitergegeben, indem öffentliche Funktionen aufgerufen werden, die im Skript definiert sind. Das Skript muss diese Funktionen implementieren, um auf die Ereignisse zu reagieren.

**Beispiel PAWN (Skript):**

```pawn
// Diese öffentliche Funktion wird von der Firmware aufgerufen, wenn ein Lok-Geschwindigkeitsereignis auftritt
public OnLocoSpeedChanged(locoId, speed) {
    // Benutzerlogik hier...
    printf("Lok %d Geschwindigkeit ist jetzt %d", locoId, speed);
}
```

## 4. Benutzer-Workflow

1.  **Skript schreiben:** Der Benutzer schreibt seine Logik in einer `.pwn`-Datei unter Verwendung der PAWN-Sprache.
2.  **Kompilieren:** Der Benutzer kompiliert die `.pwn`-Datei mit dem Standard-PAWN-Compiler (`pawncc`) auf seinem Computer. Dies erzeugt eine `.amx`-Datei.
3.  **Hochladen:** Der Benutzer überträgt die kompilierte `main.amx`-Datei in das Stammverzeichnis der SD-Karte oder des Dateisystems des Mikrocontrollers.
4.  **Ausführen:** Beim Start initialisiert die Arduino-Firmware die xTrainAPI, bindet das Dateisystem ein, lädt `main.amx` in die PAWN-VM und beginnt mit der Ausführung.

## 5. "Hallo Welt"-Beispiel

Dieses Beispiel zeigt ein einfaches Skript, das eine Nachricht auf der seriellen Konsole ausgibt, wenn die Geschwindigkeit einer Lokomotive geändert wird.

**`main.pwn`:**

```pawn
#include <xtrain_api> // Eine hypothetische Include-Datei, die die nativen Funktionen definiert

main() {
    printf("PAWN-Skript für xTrainAPI geladen.");
}

public OnLocoSpeedChanged(locoId, speed) {
    printf("Hallo von PAWN! Lok %d Geschwindigkeit auf %d geändert.", locoId, speed);

    // Beispiel für einen Rückruf zu einer nativen Funktion
    if (speed > 100) {
        SetSignalAspect(1, 2); // Signal 1 auf Aspekt 2 setzen (z. B. "Gefahr")
    }
}
```

Dieses Konzept bietet eine flexible und leistungsstarke Möglichkeit, die Funktionalität der xTrainAPI zu erweitern, ohne dass Benutzer C++-Experten sein oder die Firmware für jede Logikänderung neu flashen müssen.
