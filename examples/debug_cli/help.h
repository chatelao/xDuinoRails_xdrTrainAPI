#pragma once
#include <Arduino.h>

const char HELP_MAIN[] PROGMEM = R"=====(
Available commands:
  POWER      - Control track power
  THROTTLE   - Control a locomotive's speed and direction
  FUNCTION   - Control a locomotive's functions
  TURNOUT    - Control a turnout
  SIGNAL     - Control a signal
  MECH_SYNC  - Send a mechanical synchronization event
  h, help, ? - Print this help message

For detailed help on a specific command, type 'h <command>' or 'help <command>'.
Example: h THROTTLE
)=====";

const char HELP_POWER[] PROGMEM = R"=====(
POWER: Controls the main track power.

Syntax: <POWER state="<STATE>">
  <STATE> - The desired power state. Can be "ON" or "OFF".

Legacy Syntax: <1> (ON) or <0> (OFF)

Examples:
  <POWER state="ON">  - Turns track power on.
  <POWER state="OFF"> - Turns track power off.
  <1>                 - Turns track power on (legacy).
)=====";

const char HELP_THROTTLE[] PROGMEM = R"=====(
THROTTLE: Sets the speed and direction of a locomotive.

Syntax: <THROTTLE cab="<CAB>" speed="<SPEED>" direction="<DIR>" steps="<STEPS>">
  <CAB>     - The locomotive's DCC address.
  <SPEED>   - Speed as a percentage (0-100).
  <DIR>     - Direction: 1 for FORWARD, 0 for REVERSE.
  <STEPS>   - (Optional) The number of speed steps for the decoder (e.g., 28, 128). Defaults to 128.

Legacy Syntax: <t <CAB> <SPEED> <DIR>>
  <SPEED> in legacy syntax is an absolute value based on speed steps, not a percentage.

Examples:
  <THROTTLE cab="3" speed="50" direction="1"> - Sets loco 3 to 50% speed, forward.
  <t 3 64 1>                              - Sets loco 3 to speed step 64, forward (legacy).
)=====";

const char HELP_FUNCTION[] PROGMEM = R"=====(
FUNCTION: Activates or deactivates a locomotive function.

Syntax: <FUNCTION cab="<CAB>" function="<FUNC>" state="<STATE>">
  <CAB>     - The locomotive's DCC address.
  <FUNC>    - The function number to control (e.g., 0 for headlight).
  <STATE>   - The desired state: 1 for ON, 0 for OFF.

Legacy Syntax: <f <CAB> <FUNC> <STATE>>

Examples:
  <FUNCTION cab="3" function="0" state="1"> - Turns on the headlight (F0) for loco 3.
  <f 3 5 1>                               - Activates function 5 for loco 3 (legacy).
)=====";

const char HELP_TURNOUT[] PROGMEM = R"=====(
TURNOUT: Changes the state of a turnout.

Syntax: <TURNOUT id="<ID>" state="<STATE>">
  <ID>      - The accessory decoder address of the turnout.
  <STATE>   - The desired state: 1 for THROWN/DIVERGING, 0 for CLOSED/STRAIGHT.

Legacy Syntax: <T <ID> <STATE>>

Examples:
  <TURNOUT id="10" state="1"> - Throws turnout 10.
  <T 10 0>                   - Closes turnout 10 (legacy).
)=====";

const char HELP_SIGNAL[] PROGMEM = R"=====(
SIGNAL: Sets the aspect of a signal.

Syntax: <SIGNAL id="<ID>" aspect="<ASPECT>">
  <ID>      - The accessory decoder address of the signal.
  <ASPECT>  - The numeric ID of the desired aspect.

Legacy Syntax: <S <ID> <ASPECT>>

Examples:
  <SIGNAL id="5" aspect="2"> - Sets signal 5 to aspect 2 (e.g., 'clear').
  <S 5 0>                    - Sets signal 5 to aspect 0 (e.g., 'stop', legacy).
)=====";

const char HELP_MECH_SYNC[] PROGMEM = R"=====(
MECH_SYNC: Sends a mechanical synchronization event, typically for sound decoders.

Syntax: <MECH_SYNC cab="<CAB>" type="<TYPE>" value="<VALUE>">
  <CAB>     - The locomotive's DCC address.
  <TYPE>    - The type of synchronization event.
              Valid types: CAM_PULSE, CYLINDER_CYCLE, GEAR_CHANGE_UP,
                           GEAR_CHANGE_DOWN, BRAKE_SQUEAL_START, DOOR_MOVEMENT.
  <VALUE>   - An optional integer value associated with the event.

Legacy Syntax: <m <CAB> <TYPE_ID> <VALUE>>
  <TYPE_ID> - A numeric representation of the sync type.

Example:
  <MECH_SYNC cab="3" type="CAM_PULSE" value="1">
)=====";

void printHelp(const char* helpString) {
    Serial.println(FPSTR(helpString));
}

void showHelp(const String& command) {
    if (command.isEmpty()) {
        printHelp(HELP_MAIN);
    } else if (command.equalsIgnoreCase("POWER")) {
        printHelp(HELP_POWER);
    } else if (command.equalsIgnoreCase("THROTTLE")) {
        printHelp(HELP_THROTTLE);
    } else if (command.equalsIgnoreCase("FUNCTION")) {
        printHelp(HELP_FUNCTION);
    } else if (command.equalsIgnoreCase("TURNOUT")) {
        printHelp(HELP_TURNOUT);
    } else if (command.equalsIgnoreCase("SIGNAL")) {
        printHelp(HELP_SIGNAL);
    } else if (command.equalsIgnoreCase("MECH_SYNC")) {
        printHelp(HELP_MECH_SYNC);
    } else {
        Serial.print("Unknown command: ");
        Serial.println(command);
        printHelp(HELP_MAIN);
    }
}
