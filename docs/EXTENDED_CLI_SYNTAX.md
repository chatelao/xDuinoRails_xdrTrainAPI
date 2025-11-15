# Extended CLI Syntax

This document defines the extended command-line interface (CLI) syntax for the xDuinoRails_xTrainAPI. This syntax is designed to be more self-descriptive and easier to read than the legacy DCC-EX single-character commands. It also includes commands for all events supported by the `IUnifiedModelTrainListener` interface.

The extended syntax follows a consistent format: `<COMMAND param1="value1" param2="value2" ...>`.

**Note on Command Formats:** The `CmdLineParser` class is designed to accept both the legacy single-character commands and the extended commands at all times. The `CmdLinePrinter` class, however, can be configured to output either the legacy or extended format. This is controlled by the `USE_EXTENDED_CLI_SYNTAX` compile-time flag. By default, the extended syntax is used for output. To use the legacy syntax for output, set this flag to `0` during compilation.

## A. Track Power & Status

| Command | Legacy | Parameters | Description | Example |
| :--- | :--- | :--- | :--- | :--- |
| `POWER` | `<1>`/`<0>` | `state` ("ON" or "OFF") | Turns track power on or off. | `<POWER state="ON">` |
| `SET_POWER` | `<p>` | `track` (0=MAIN, 1=PROG), `state` (0=OFF, 1=ON) | Sets the power for a specific track. | `<SET_POWER track="1" state="1">` |
| `STATUS` | `<c>` | (none) | Requests a detailed status report. | `<STATUS>` |
| `FULL_STATUS` | `<S>` | (none) | Requests a full status dump of all devices. | `<FULL_STATUS>` |
| `HARDWARE_INFO` | `<i>` | (none) | Requests hardware and board information. | `<HARDWARE_INFO>` |
| `REBOOT` | `<Z>` | (none) | Reboots the command station. | `<REBOOT>` |

## B. Loco Control (Throttle)

| Command | Legacy | Parameters | Description | Example |
| :--- | :--- | :--- | :--- | :--- |
| `THROTTLE` | `<t>` | `cab`, `speed` (0-126), `direction` (0=REV, 1=FWD), `steps` (14, 28, 128) | Main throttle command. | `<THROTTLE cab="123" speed="100" direction="1" steps="128">` |
| `SIMPLE_THROTTLE` | `<s>` | `cab`, `speed` (0-126), `direction` (0=REV, 1=FWD) | Simple throttle command. | `<SIMPLE_THROTTLE cab="123" speed="100" direction="1">` |
| `FUNCTION` | `<f>`/`<F>` | `cab`, `function` (0-28), `state` (0=OFF, 1=ON) | Toggles a locomotive function. | `<FUNCTION cab="123" function="1" state="1">` |
| `LIST_LOCOS` | `<l>` | (none) | Lists all locos in the roster. | `<LIST_LOCOS>` |
| `EMERGENCY_STOP_LOCO` | `<X>` | `cab` | Emergency stop for a single loco. | `<EMERGENCY_STOP_LOCO cab="123">` |
| `EMERGENCY_STOP_ALL` | `<K>` | (none) | Emergency stop for all locos. | `<EMERGENCY_STOP_ALL>` |
| `RESET_EMERGENCY_STOP_ALL` | `<E>` | (none) | Resets emergency stop for all locos. | `<RESET_EMERGENCY_STOP_ALL>` |
| `DELETE_LOCO` | `<R>` | `cab` | Deletes a loco from the roster. | `<DELETE_LOCO cab="123">` |

## C. CV Programming

| Command | Legacy | Parameters | Description | Example |
| :--- | :--- | :--- | :--- | :--- |
| `READ_CV` | `<R>` | `cv`, `track` (0=POM, 1=PROG) | Reads a CV. | `<READ_CV cv="1" track="1">` |
| `WRITE_CV_BYTE` | `<W>` | `cv`, `value`, `track` (0=POM, 1=PROG) | Writes a byte to a CV. | `<WRITE_CV_BYTE cv="1" value="123" track="1">` |
| `WRITE_CV_BIT` | `<B>` | `cv`, `bit`, `value`, `track` (0=POM, 1=PROG) | Writes a bit to a CV. | `<WRITE_CV_BIT cv="1" bit="7" value="1" track="1">` |
| `PAGED_MODE_WRITE` | `<P>` | `cv`, `value` | Paged mode CV write. | `<PAGED_MODE_WRITE cv="1" value="123">` |
| `DIRECT_MODE_WRITE` | `<M>` | `address`, `value` | Direct mode register write. | `<DIRECT_MODE_WRITE address="1024" value="123">` |

## D. Accessories

| Command | Legacy | Parameters | Description | Example |
| :--- | :--- | :--- | :--- | :--- |
| `TURNOUT` | `<T>` | `id`, `state` (0=THROWN, 1=STRAIGHT) | Sets a turnout's state. | `<TURNOUT id="456" state="1">` |
| `ACCESSORY` | `<Q>` | `id`, `state` (0=OFF, 1=ON) | Sets an accessory's state. | `<ACCESSORY id="789" state="1">` |

## E. Sensors & I/O

| Command | Legacy | Parameters | Description | Example |
| :--- | :--- | :--- | :--- | :--- |
| `VPIN` | `<Y>` | `id`, `state` (0/1) | Manually sets a VPIN's state. | `<VPIN id="1" state="1">` |
| `DEFINE_VPIN` | `<N>` | `vpin`, `type`, `state` (0/1) | Defines a VPIN. | `<DEFINE_VPIN vpin="1" type="INPUT" state="0">` |
| `UNDEFINE_VPIN` | `<U>` | `vpin` | Undefines a VPIN. | `<UNDEFINE_VPIN vpin="1">` |

## F. JSON Commands

| Command | Legacy | Parameters | Description | Example |
| :--- | :--- | :--- | :--- | :--- |
| `JSON_QUERY` | `<J>` | `type` ("T", "A", "L", "S", "O", "R", "C") | Requests data in JSON format. | `<JSON_QUERY type="T">` |
| `JSON_CONFIG` | `<=>` | `json` | Sends a configuration command in JSON. | `<JSON_CONFIG json="{...}">` |

## G. Diagnostics & System

| Command | Legacy | Parameters | Description | Example |
| :--- | :--- | :--- | :--- | :--- |
| `DIAGNOSTIC` | `<D>` | `command`, `params` | Performs various diagnostic actions. | `<DIAGNOSTIC command="RAM">` |
| `LIST_VARIABLES` | `<V>` | (none) | Lists all defined variables. | `<LIST_VARIABLES>` |
| `ECHO` | `<+>`/`<->` | `state` ("ON" or "OFF") | Activates or deactivates echo mode. | `<ECHO state="ON">` |
| `LIST_COMMANDS` | `<?>` | (none) | Requests a list of available commands. | `<LIST_COMMANDS>` |

## H. Proposed New Commands

These commands are proposed to cover events in the `IUnifiedModelTrainListener` interface that are not covered by the legacy DCC-EX syntax.

| Command | Description | Parameters | Example |
| :--- | :--- | :--- | :--- |
| `LOCO_FUNCTION_ANALOG` | Sets an analog value for a loco function. | `cab`, `function`, `value` | `<LOCO_FUNCTION_ANALOG cab="123" function="8" value="255">` |
| `DISPATCH` | Changes the dispatch state of a loco. | `cab`, `state` ("ACQUIRED" or "RELEASED"), `owner` | `<DISPATCH cab="123" state="ACQUIRED" owner="user1">` |
| `CONSIST_LINK` | Links two locos in a consist. | `master`, `slave`, `type`, `inverted` (true/false) | `<CONSIST_LINK master="123" slave="456" type="ADVANCED" inverted="false">` |
| `CONSIST_UNLINK` | Unlinks a loco from a consist. | `slave` | `<CONSIST_UNLINK slave="456">` |
| `ACCESSORY_ANALOG` | Sets an analog value for an accessory. | `address`, `value` (0.0-1.0) | `<ACCESSORY_ANALOG address="789" value="0.75">` |
| `ACCESSORY_ERROR` | Reports an accessory error. | `address`, `errorId`, `errorMessage` | `<ACCESSORY_ERROR address="789" errorId="1" errorMessage="Overload">` |
| `SENSOR` | Sets the state of a sensor. | `id`, `state` ("ACTIVE" or "INACTIVE") | `<SENSOR id="101" state="ACTIVE">` |
| `FAST_CLOCK` | Updates the fast clock. | `time` (Unix timestamp), `factor` | `<FAST_CLOCK time="1678886400" factor="4.0">` |
| `HARDWARE_NODE_ATTACHED` | Reports a new hardware node. | `uid`, `productName`, `booster` (true/false) | `<HARDWARE_NODE_ATTACHED uid="abc-123" productName="xPressNet" booster="false">` |
| `HARDWARE_NODE_LOST` | Reports a lost hardware node. | `uid` | `<HARDWARE_NODE_LOST uid="abc-123">` |
| `SYSTEM_MESSAGE` | Sends a system message. | `source`, `message` | `<SYSTEM_MESSAGE source="WebServer" message="User logged in">` |
| `LOCO_DETECTED_ON_BLOCK` | Reports a loco detected on a block. | `sensorId`, `cab`, `orientation` ("FORWARD" or "REVERSE") | `<LOCO_DETECTED_ON_BLOCK sensorId="202" cab="123" orientation="FORWARD">` |
| `LOCO_TELEMETRY` | Reports telemetry data for a loco. | `cab`, `type`, `value` | `<LOCO_TELEMETRY cab="123" type="SPEED" value="50.5">` |
| `LOCO_EXTERNAL_STATE` | Changes the external state of a loco. | `cab`, `state` | `<LOCO_EXTERNAL_STATE cab="123" state="PARKED">` |
| `LOCO_RAILCOM_RAW` | Sends raw RailCom data. | `cab`, `appId`, `data` (hex string) | `<LOCO_RAILCOM_RAW cab="123" appId="1" data="A0B1C2">` |
| `NEW_LOCO_DISCOVERED` | Reports a newly discovered loco. | `cab`, `name`, `icon` | `<NEW_LOCO_DISCOVERED cab="123" name="Big Boy" icon="steam_loco.png">` |
| `CV_READ_RESULT` | Reports the result of a CV read. | `cab`, `cv`, `value`, `success` (true/false) | `<CV_READ_RESULT cab="123" cv="1" value="10" success="true">` |
| `SUSI_CONFIG_READ` | Reports the result of a SUSI config read. | `cab`, `bank`, `index`, `value` | `<SUSI_CONFIG_READ cab="123" bank="1" index="2" value="3">` |
| `CONFIG_BLOCK_LOADED` | Reports that a config block has been loaded. | `cab`, `domain`, `data` (hex string) | `<CONFIG_BLOCK_LOADED cab="123" domain="sound" data="DEADBEEF">` |
| `PROGRESS_UPDATE` | Reports the progress of an operation. | `operation`, `percent` | `<PROGRESS_UPDATE operation="Firmware Update" percent="75.5">` |
| `MECHANICAL_SYNC_EVENT` | Reports a mechanical sync event. | `cab`, `type`, `value` | `<MECHANICAL_SYNC_EVENT cab="123" type="CHUFF" value="1">` |
