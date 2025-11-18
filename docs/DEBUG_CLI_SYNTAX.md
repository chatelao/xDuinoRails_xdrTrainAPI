# Debug CLI Syntax

This document describes the command-line interface (CLI) for the `debug_cli.ino` example sketch. This interface is intended for interactive debugging and testing of the xDuinoRails_xTrainAPI library.

## Command Format

All commands must be enclosed in angle brackets `< >` or be one of the recognized help commands. The serial interface reads commands terminated by a newline character.

-   **Standard Commands:** `<COMMAND ...>`
-   **Help Commands:** `h`, `?`, `help`

## Help Command

The help command provides information about available commands and their syntax.

| Command | Description | Example |
| :--- | :--- | :--- |
| `h`, `?`, `help` | Displays a general list of all available commands. | `h` |
| `h <COMMAND>` | Displays detailed help for a specific command. | `h THROTTLE` |

**Note:** The `<?>` command is aligned with the DCC-EX syntax for listing available commands. The `h` and `help` commands are additional aliases provided in this debug sketch for user convenience.

## Supported Commands

The following table lists the commands supported by the `debug_cli` sketch. It shows both the extended, self-descriptive syntax and the corresponding DCC-EX syntax.

| Command | Description | Parameters | Example | DCC-EX Syntax |
| :--- | :--- | :--- | :--- | :--- |
| `POWER` | Controls track power. | `state` ("ON" or "OFF") | `<POWER state="ON">` | `<1>` / `<0>` |
| `THROTTLE` | Controls a locomotive's speed and direction. | `cab`, `speed` (0-100%), `direction` (0/1), `steps` (optional) | `<THROTTLE cab="3" speed="50" direction="1">` | `<t cab speed dir>` |
| `FUNCTION` | Controls a locomotive's functions. | `cab`, `function` (0-28), `state` (0/1) | `<FUNCTION cab="3" function="0" state="1">` | `<f cab func state>` |
| `TURNOUT` | Controls a turnout. | `id`, `state` (0/1) | `<TURNOUT id="10" state="1">` | `<T id state>` |
| `SIGNAL` | Controls a signal. | `id`, `aspect` | `<SIGNAL id="5" aspect="2">` | `<S id aspect>` |
| `MECH_SYNC`| Sends a mechanical synchronization event. | `cab`, `type`, `value` | `<MECH_SYNC cab="3" type="CAM_PULSE" value="1">` | `<m cab type value>`|
