## 1. Complete EBNF (Syntax Definition)

The Extended Backus-Naur Form (EBNF) defines the "grammar" or structure of *every* message sent to or from the Command Station.

```ebnf
(* A message is a single command or response,
   enclosed in angle brackets. *)
message     ::= "<" opcode ( " " parameter )* ">"

(* The opcode is ALWAYS a single character.
   It is case-sensitive
   (e.g., 'T' is a command, 't' is a response). *)
opcode      ::= CHARACTER

(* Parameters are separated by spaces. *)
parameter   ::= keyword | numeric | string

(* A keyword is a sequence of characters without spaces.
   It consists of letters (a-z, A-Z), digits (0-9), and/
   or the underscore (_).
   Keywords are NOT case-sensitive. *)
keyword     ::= ( LETTER | DIGIT | "_" )+

(* A number is a sequence of digits.
   It can optionally have a negative prefix (-).
   Decimals or floating-point numbers are not supported. *)
numeric     ::= [ "-" ] DIGIT+

(* A string is any sequence of characters
   enclosed in double-quotes (").
   It may contain spaces. *)
string      ::= '"' ( ANY_CHAR_EXCEPT_QUOTE )* '"'

````

-----

## 2\. Complete Command Table (Opcode Reference)

This is the complete list of all Opcodes (commands and responses), categorized by their function.

### A. Track Power & Status

| OPCODE | Type | Parameters | Description |
| :--- | :--- | :--- | :--- |
| **`1`** | Command | (none) | Turns Track Power (Track 0 / MAIN) ON. |
| **`0`** | Command | (none) | Turns Track Power (Track 0 / MAIN) OFF. |
| **`p`** | Command | `<track> <state>` | Sets the power status (`0`=OFF, `1`=ON) for a specific track (`0`=MAIN, `1`=PROG). |
| `p` | Response | `<track> <state>` | Response to a `<p>` command or a status change. |
| **`c`** | Command | (none) | Requests a detailed status report from the Command Station (version, power status, etc.). |
| `c` | Response | `<station-status>` | Sends the detailed status report (Format: `<p0 1><p1 0><v 5.1.0 (Git-....)...>`). |
| **`S`** | Command | (none) | Requests a full status dump of all known devices (turnouts, locos, etc.). |
| `S` | Response | `<S ...>` | Response to `<S>` (format details vary). |
| **`i`** | Command | (none) | Requests hardware and board information (used by EX-Installer). |
| `i` | Response | `<i ...>` | Sends hardware information. |
| **`Z`** | Command | (none) | Performs a software reset (reboot) of the Command Station. |

### B. Loco Control (Throttle)

| OPCODE | Type | Parameters | Description |
| :--- | :--- | :--- | :--- |
| **`t`** | Command | `<cab> <speed> <dir> <steps>` | **Main throttle command:** Controls loco (`cab` address), speed (`0-126`), direction (`0`=Rev, `1`=Fwd), and speed steps (`1`=14, `2`=28, `4`=128). |
| **`s`** | Command | `<cab> <speed> <dir>` | **Simple throttle command:** Uses the default speed step (usually 128). Speed (`0-126`), direction (`0`=Rev, `1`=Fwd). |
| **`f`** | Command | `<cab> <f_num> <state>` | Toggles a function (`0-28`) for a loco (`cab`) ON (`1`) or OFF (`0`). |
| **`F`** | Command | `<cab> <f_num> <state>` | Obsolete but supported. Identical to `<f>`. |
| **`l`** | Command | (none) | Lists all locos currently in the internal roster (memory). |
| `l` | Response | `<cab> <reg> <speedByte> <fMap>` | Response to `<l>`: Sends loco status (`speedByte` is DCC-encoded, `fMap` is a bitmask of functions). |
| **`X`** | Command | `<cab>` | Triggers Emergency Stop for a single loco (sets speed to -1). |
| **`K`** | Command | (none) | Sends an Emergency Stop signal to *all* locos. |
| **`E`** | Command | (none) | Resets the Emergency Stop status for *all* locos (sets speed to 0). |
| **`R`** | Command | `<cab>` | Deletes (removes) a loco from the internal memory (roster). |
| `R` | Response | `<cab>` | Confirms the deletion of the loco. |

### C. CV Programming (Service & Main Track)

| OPCODE | Type | Parameters | Description |
| :--- | :--- | :--- | :--- |
| **`R`** | Command | `<cv> <track>` | **Read CV:** Reads CV (`1-1024`) on the Programming Track (`1`=PROG) or Main Track (`0`=POM). |
| `r` | Response | `<cv> <value>` | Response to `<R>`: Sends the read CV value (`-1` or `-2` on error). |
| **`W`** | Command | `<cv> <value> <track>` | **Write CV (Byte):** Writes value (`0-255`) to CV (`1-1024`) on PROG (`1`) or POM (`0`). |
| `w` | Response | `<cv> <value>` | Response to `<W>` (from Prog track only): Confirms the written value. |
| **`B`** | Command | `<cv> <bit> <value> <track>` | **Write CV (Bit):** Writes a single bit value (`0` or `1`) to a bit position (`0-7`) within a CV. |
| `b` | Response | `<cv> <bit> <value>` | Response to `<B>` (from Prog track only). |
| **`P`** | Command | `<cv> <value>` | **Paged Mode Write:** Writes a CV value *directly* to the programming track (no acknowledgement). |
| **`M`** | Command | `<addr> <value>` | **Direct Mode Write:** Writes a value *directly* to a register address (`addr`). (Experts only) |

### D. Accessories (Turnouts, Signals, etc.)

| OPCODE | Type | Parameters | Description |
| :--- | :--- | :--- | :--- |
| **`T`** | Command | `<id> <state>` | Sets a Turnout with `id` to a state (`0`=Thrown/Closed, `1`=Straight/Open). |
| `H` | Response | `T <id> <state>` | Broadcast Message: Reports that Turnout `id` has changed its state (`0`/`1`). |
| **`Q`** | Command | `<id> <state>` | Sets a simple Accessory (signal) with `id` to a state (`0`=OFF, `1`=ON). |
| `H` | Response | `Q <id> <state>` | Broadcast Message: Reports that Accessory `id` has changed its state (`0`/`1`). |

### E. Sensors & I/O

| OPCODE | Type | Parameters | Description |
| :--- | :--- | :--- | :--- |
| **`Y`** | Command | `<id> <state>` | Manually sets the state of a VPIN (virtual pin) `id` to `state` (`0`/`1`). |
| `H` | Response | `Y <id> <state>` | Broadcast Message: Reports that Sensor/VPIN `id` has changed its state (`0`/`1`). |
| **`N`** | Command | `<vpin> <type> <state>` | Defines a VPIN (`vpin`) with a type (e.g., `INPUT`, `OUTPUT`) and sets its initial state (`state`). |
| **`U`** | Command | `<vpin>` | Removes (undefines) a VPIN (`vpin`). |

### F. JSON Commands (Lists & Configuration)

*(These commands use JSON for complex responses; the command itself remains simple.)*
| OPCODE | Type | Parameters | Description |
| :--- | :--- | :--- | :--- |
| **`J`** | Command | `<keyword>` | **JSON Query:** Requests a list or data in JSON format. |
| | | `T` | `<JT>` Requests the list of all defined Turnouts. |
| | | `A` | `<JA>` Requests the list of all defined Accessories. |
| | | `L` | `<JL>` Requests the list of all defined Locos (Roster). |
| | | `S` | `<JS>` Requests the list of all defined Sensors. |
| | | `O` | `<JO>` Requests the list of all defined Outputs. |
| | | `R` | `<JR>` Requests the list of all defined Routes (EX-RAIL). |
| | | `C` | `<JC>` Requests the list of all defined Current sensors. |
| `j` | Response | `<keyword> [JSON-data]` | Response to a `<J>` command (e.g., `<jT [...]>`). |
| **`=`** | Command | `<JSON-string>` | Sends a configuration command in JSON format (e.g., to define devices). |
| `=` | Response | `<JSON-string>` | Response/acknowledgement to a JSON configuration command. |

### G. Diagnostics & System

| OPCODE | Type | Parameters | Description |
| :--- | :--- | :--- | :--- |
| **`D`** | Command | `<keyword> [params...]` | **Diagnostic Command:** Performs various diagnostic actions. |
| | | `ACK` | `<D ACK>` Toggles ACK pulse testing for the programming track. |
| | | `WIFI [ON\|OFF]` | `<D WIFI ON>` Turns WiFi debugging ON/OFF. |
| | | `SERIAL [ON\|OFF]`| `<D SERIAL ON>` Turns Serial debugging ON/OFF. |
| | | `ETH [ON\|OFF]` | `<D ETH ON>` Turns Ethernet debugging ON/OFF. |
| | | `RAM` | `<D RAM>` Displays the amount of free memory. |
| | | `I2C` | `<D I2C>` Scans the I2C bus and lists found devices. |
| | | `EEPROM` | `<D EEPROM>` Dumps the contents of the EEPROM (Debug). |
| | | `CLEAR` | `<D CLEAR>` Clears all devices stored in EEPROM (turnouts, sensors, etc.). |
| | | `SHOW` | `<D SHOW>` Lists all VPINs and their assignments. |
| `D` | Response | `[Text]` | Response to a diagnostic command (format varies greatly, often human-readable text). |
| **`V`** | Command | (none) | Lists all defined variables and their values. |
| `V` | Response | `[Text]` | Response to `<V>`. |
| **`+`** | Command | (none) | Activates "Echo" mode (sends all received commands back). |
| **`-`** | Command | (none) | Deactivates "Echo" mode. |
| **`?`** | Command | (none) | Requests a list of all available commands (Opcodes) from the station. |
| `?` | Response | `[Text]` | Sends the list of implemented Opcodes. |
| **`h`, `help`** | Command | `[command]` | **Reserved for Help:** These opcodes, along with `?`, are reserved for invoking a help system. The `debug_cli` example provides a reference implementation. See the [Debug CLI Syntax](./DEBUG_CLI_SYNTAX.md) for details. |

### H. Error & Broadcast Responses

| OPCODE | Type | Parameters | Description |
| :--- | :--- | :--- | :--- |
| `X` | Response | (none) | **Error:** The last command sent was invalid, unknown, or syntactically incorrect. |
| `H` | Response | `<type> [...]` | **Broadcast:** A general message reporting a state change (e.g., turnout, sensor). See Sections D & E. |
| `*` | Response | `[Text]` | **Debug Message:** A general, human-readable debug or status message (e.g., `<* WiFi connected>`). |

```
```
