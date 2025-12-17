Here is the complete **Data Dictionary** for the `UnifiedTrainAPI` as a GitHub-flavored Markdown table. You can copy this block directly into your `README.md` or wiki page.

# xTrainAPI Data Dictionary

This table describes all fields, parameters, and data types used in the `UnifiedTrainAPI`.

| Category | Field / Parameter | Data Type | Description | Valid Range / Values | Notes |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Core** | `Protocol` | `enum` | List of all supported protocols. | `DCC`, `MM_I`, `MM_II`, `MFX`, `SELECTRIX`, `SX2`, `LOCONET`, `BIDIB`, `XPRESSNET`, `CAN_GENERIC` | Determines the driver to be used. |
| **Core** | `mobAddress` | `uint16_t` | The digital address of the decoder. | `1` - `10239` (DCC)<br>`1` - `255` (MM)<br>`1` - `127` (SX) | Is ignored if `mfxUid` is set. |
| **Core** | `mobMfxUid` | `uint32_t` | Unique Hardware ID (UID) for mfx/BiDiB. | `0` = Inactive<br>`>0` = Valid UID | Takes precedence over the address (routing priority). |
| **Core** | `direction` | `enum` | Logical travel direction. | `REVERSE` (0)<br>`FORWARD` (1)<br>`UNKNOWN` (2) | Used for commands and feedback. |
| **Traffic** | `mobSpeedPercent` | `float` | Target speed of the locomotive. | `0.0` (Stop) to `100.0` (Vmax) | Must be converted by the driver into speed steps (14/28/128). |
| **Traffic** | `mobSpeedSteps` | `int` | Resolution of the speed steps for the command. | `14`, `28`, `128` | Important for correct DCC packet generation. |
| **Traffic** | `mobFuncX` | `int` | Index of the function key. | `0` (Light)<br>`1` - `32767` (Aux) | RCN-227 defines up to F68, `int` is future-proof. |
| **Traffic** | `mobFuncActive` | `bool` | Status of a binary function (switch). | `true` = ON<br>`false` = OFF | Standard for F-keys. |
| **Traffic** | `mobFuncAnalog0to1` | `float` | Value of an analog function (pressure/intensity). | `0.0` to `1.0` | For `onLocoFunctionAnalogValue` (e.g., WiThrottle). |
| **Traffic** | `mobIsAcquired` | `bool` | Status of slot management (dispatching). | `true` = Acquired by controller<br>`false` = Released | Relevant for LocoNet/XpressNet arbitration. |
| **Traffic** | `mobOwnerId` | `string` | ID of the device controlling the locomotive. | e.g., "Throttle \#54", "JMRI" | Debug info. |
| **Traffic** | `mobConsist` | `enum` | Method of multiple traction. | `ADVANCED_DCC` (CV19)<br>`UNIVERSAL_HOST` (Software)<br>`MU_LOCONET` | Determines the type of linking/addressing. |
| **Traffic** | `mobInverted` | `bool` | Orientation in the consist. | `true` = Slave runs in opposite direction<br>`false` = Same direction | For `onConsistLink`. |
| **Accessory** | `statAddress` | `uint16_t` | Address of the accessory decoder. | Protocol-dependent (usually `1`-`2048`) | For turnouts and signals. |
| **Accessory** | `statIsThrown` | `bool` | State of a standard turnout. | `true` = Thrown/Red<br>`false` = Straight/Green | Basic function. |
| **Accessory** | `statIsFeedback` | `bool` | Source of the event. | `true` = Actual feedback (RailCom)<br>`false` = Command echo | Important for automation validation (safety). |
| **Accessory** | `statAspect` | `uint8_t` | Signal aspect. | `0` to `255` | For complex signals (Hp0, Hp1, Sh1...). |
| **Accessory** | `statAnalog0to1` | `float` | Analog accessory value (servo/dimmer). | `0.0` to `1.0` | Normalized float. |
| **Accessory** | `statError` | `uint8_t` | Diagnostic error code. | `1`=OpenLoad<br>`2`=Overload/Short<br>`3`=Stall | Hardware diagnostics (BiDiB/RailCom Port Status). |
| **Accessory** | `statErrorDesc` | `string` | Readable error description. | Text | For UI display. |
| **System** | `PowerState` | `enum` | Global power status. | `OFF` (0)<br>`ON` (1)<br>`EMERGENCY_STOP` (2)<br>`SHORT_CIRCUIT` (3) | Distinguishes between logical emergency stop and physical short circuit. |
| **System** | `modelTimeUnix` | `int64_t` | Current model time (RCN-211). | Unix Timestamp (seconds since 1970) | Contains year, month, day, time for DCC Time/Date packets. |
| **System** | `factor` | `float` | Time acceleration factor. | e.g., `1.0` (Real-time), `4.0` (Fast) | Sync for handheld controller clocks (LocoNet/XpressNet). |
| **System** | `nodeUid` | `string` | Unique Hardware ID of a bus node. | e.g., "05:00:AB:C1" | Uniquely identifies boosters/detectors in the bus tree. |
| **System** | `productName` | `string` | Product name of the hardware. | e.g., "GBM16T", "Z21" | UI display of the topology. |
| **System** | `booster` | `bool` | Power supply capability. | `true` = Is a booster<br>`false` = Logic only | Filter for UI (Power Management). |
| **System** | `message` | `string` | System-wide text message. | Text | Chat, logs, or LISSY text. |
| **Telemetry** | `sensorId` | `uint32_t` | ID of a feedback sensor/block. | System-dependent 32-bit ID | Maps to S88 port, LNet sensor, BiDiB input. |
| **Telemetry** | `DecoderOrientation`| `enum` | Locomotive orientation on the track (RCN-217). | `NORMAL` (Chimney first)<br>`INVERTED` (Tender first)<br>`UNKNOWN` | Critical for shuttle train control and automation. |
| **Telemetry** | `ExternalState` | `enum` | External influence. | `FREE_RUN`<br>`STOPPED_BY_ABC`<br>`STOPPED_BY_DC`<br>`STOPPED_BY_HLU` | Indicates if the track is braking the loco (despite throttle > 0). |
| **Telemetry** | `TelemetryType` | `enum` | Type of the measured value. | `SPEED`, `LOAD`, `VOLTAGE`, `CURRENT`, `FUEL`, `TEMP`, `QOS`, `ODOMETER` | Defines the unit of `value`. |
| **Telemetry** | `value` (Telemetry)| `float` | The measured value. | KM/H, %, Volt, Ampere, °C | Depends on `TelemetryType`. |
| **Telemetry** | `appId` | `uint8_t` | RailCom application ID. | `0` to `255` | For proprietary raw data (manufacturer-specific). |
| **Telemetry** | `data` (Raw) | `vector<u8>`| Raw data payload. | Byte Array | For RailCom Raw or Config Blocks. |
| **Config** | `cvNumber` | `int` | Index of the configuration variable. | `1` to `1024` (DCC)<br>`1` to `65535` (Index) | Address in the decoder's memory. |
| **Config** | `bankIndex` | `uint8_t` | Sound bank for SUSI. | `0` to `3` | Usually mapped via CV 1024/897. |
| **Config** | `susiIndex` | `uint8_t` | Parameter within the SUSI bank. | `0` to `255` | The "CV" in the SUSI module. |
| **Config** | `domain` | `string` | Type of the mass data block. | "ICON", "FIRMWARE", "MFX\_CONFIG" | Context for `onConfigBlockLoaded`. |
| **Config** | `operation` | `string` | Name of the ongoing process. | e.g., "Reading CV List" | For progress bars/UI. |
| **Config** | `percent` | `float` | Progress. | `0.0` to `100.0` | UI feedback. |
| **Config** | `success` | `bool` | Result of an operation. | `true` = ACK received<br>`false` = Timeout/NACK | Success check for CV reads. |
