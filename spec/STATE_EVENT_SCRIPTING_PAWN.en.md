# Concept: State Event Scripting with PAWN

This document outlines a concept for integrating the PAWN scripting language into the xTrainAPI ecosystem to allow for user-defined automation and stateful event handling.

## 1. Goals

*   **User Extensibility:** Allow end-users to write custom logic and automation scripts without modifying the core C++ firmware.
*   **Stateful Logic:** Enable the creation of scripts that can remember state between events (e.g., "if a train passes sensor A, then sensor B, throw switch C").
*   **Decoupling:** Separate user-defined logic from the core API implementation, improving maintainability.
*   **Resource Efficiency:** Use a scripting language with a small footprint suitable for resource-constrained microcontrollers like Arduino.

## 2. Core Concept

The core idea is to embed the PAWN virtual machine (VM) into the main Arduino firmware. This VM will execute pre-compiled PAWN bytecode (`.amx` files) stored on a filesystem accessible by the microcontroller (e.g., an SD card or SPIFFS).

A "binding" layer will expose a subset of the xTrainAPI functions to the PAWN scripting environment. When an event occurs in the xTrainAPI, it will be forwarded to a public function within the running PAWN script. The script can then call back into the native C++ code to trigger actions.

## 3. Architecture

### 3.1. PAWN Virtual Machine (VM)

The PAWN VM is a lightweight interpreter that runs the compiled bytecode. It will be integrated as a C++ library into the main firmware. The VM will be responsible for loading the bytecode and executing it.

### 3.2. Bytecode Storage

Compiled PAWN scripts (`.amx` files) will be stored on a filesystem. The firmware will expect a specific file, such as `main.amx`, to be present at the root of the filesystem to be loaded on startup.

### 3.3. Native Function Binding

A C++ binding layer will be created to expose core xTrainAPI functionalities to the PAWN environment. These will be "native" functions that PAWN scripts can call.

**Example C++ (Binding):**

```cpp
// This function will be callable from PAWN
cell AMX_NATIVE_CALL set_loco_speed(AMX* amx, cell* params) {
    int locoId = params[1];
    int speed = params[2];
    // Call the actual xTrainAPI function
    // IUnifiedModelTrainListener::onLocoSpeedChange(locoId, speed);
    return 0;
}
```

### 3.4. Event Handling

xTrainAPI events will be passed to the PAWN script by calling public functions defined within the script. The script must implement these functions to react to the events.

**Example PAWN (Script):**

```pawn
// This public function is called by the firmware when a loco speed event occurs
public OnLocoSpeedChanged(locoId, speed) {
    // User logic here...
    printf("Loco %d speed is now %d", locoId, speed);
}
```

## 4. User Workflow

1.  **Write Script:** The user writes their logic in a `.pwn` file using the PAWN language.
2.  **Compile:** The user compiles the `.pwn` file using the standard PAWN compiler (`pawncc`) on their computer. This generates a `.amx` file.
3.  **Upload:** The user transfers the compiled `main.amx` file to the root of the microcontroller's SD card or filesystem.
4.  **Execute:** On boot, the Arduino firmware initializes the xTrainAPI, mounts the filesystem, loads `main.amx` into the PAWN VM, and begins execution.

## 5. "Hello World" Example

This example demonstrates a simple script that prints a message to the serial console when a locomotive's speed is changed.

**`main.pwn`:**

```pawn
#include <xtrain_api> // A hypothetical include file defining the native functions

main() {
    printf("PAWN Script for xTrainAPI loaded.");
}

public OnLocoSpeedChanged(locoId, speed) {
    printf("Hello from PAWN! Loco %d speed changed to %d.", locoId, speed);

    // Example of calling back to a native function
    if (speed > 100) {
        SetSignalAspect(1, 2); // Set signal 1 to aspect 2 (e.g., "danger")
    }
}
```

This concept provides a flexible and powerful way to extend the functionality of the xTrainAPI without requiring users to be C++ experts or re-flash the firmware for every logic change.
