# xTrainAPI XML Message Format

This document describes the XML format used for asynchronous communication and test data with the `xTrainAPI`. The format is derived directly from the `IUnifiedModelTrainListener` C++ interface.

## 1. Basic Structure

All messages are encapsulated within a root `<xTrainEvents>` element. Each individual message is represented by an `<event>` element with a `type` attribute that corresponds to a method in the C++ listener interface (e.g., `onLocoSpeedChange`).

```xml
<?xml version="1.0" encoding="UTF-8"?>
<xTrainEvents>
    <event type="EVENT_TYPE" timestamp="ISO_8601_TIMESTAMP">
        <!-- Event-specific data elements -->
    </event>
    ...
</xTrainEvents>
```

**Key Attributes:**

*   `type`: The name of the API event being represented.
*   `timestamp`: An ISO 8601 formatted timestamp (e.g., `2024-07-29T10:00:01Z`) indicating when the event was generated.

## 2. Event Payloads

The payload of each event is represented by one or more child elements that group related parameters from the corresponding C++ function.

### Example 1: `onLocoSpeedChange`

*   **C++ Signature:** `onLocoSpeedChange(const LocoHandle& loco, float speedPercent, Direction direction, int speedSteps)`
*   **XML Representation:**

    ```xml
    <event type="onLocoSpeedChange" timestamp="2024-07-29T10:00:01Z">
        <loco address="3" protocol="DCC" mfxUid="0" />
        <speed speedPercent="50.5" direction="FORWARD" speedSteps="128" />
    </event>
    ```

    *   The `LocoHandle` struct is mapped to the `<loco>` element.
    *   Speed-related parameters are grouped into the `<speed>` element.

### Example 2: `onTurnoutChanged`

*   **C++ Signature:** `onTurnoutChanged(uint16_t address, bool isThrown, bool isFeedback)`
*   **XML Representation:**

    ```xml
    <event type="onTurnoutChanged" timestamp="2024-07-29T10:03:10Z">
        <turnout address="15" isThrown="true" isFeedback="true" />
    </event>
    ```

    *   All parameters are mapped as attributes of the `<turnout>` element.

## 3. Data Mapping

| C++ Type / Concept | XML Element/Attribute | Description |
| :--- | :--- | :--- |
| `LocoHandle` struct | `<loco>` element | Contains `address`, `protocol`, and optional `mfxUid` attributes. |
| Enums (`Direction`, `Protocol`, etc.) | String Representation | Enum values are represented as uppercase strings (e.g., `FORWARD`, `DCC`). |
| Primitives (`int`, `float`, `bool`) | Attributes | Most primitive types are mapped to attributes on the relevant element. |
| `std::string` | Attributes | String parameters are also mapped to attributes. |

## 4. File Structure

This directory contains the following files:

*   `all_messages.xml`: A single XML file containing an example of every event type.
*   `on[EventName].xml`: Individual XML files, each containing a single event. These are useful for targeted testing.
*   `xTrainEvents.xsd`: An XML Schema Definition (XSD) file that defines the structure of the XML messages.

## 5. XSD Validation

All XML files in this directory are validated against the `xTrainEvents.xsd` schema. You can use a command-line tool like `xmllint` to validate your own XML files:

```bash
xmllint --noout --schema xTrainEvents.xsd your_messages.xml
```

## 6. Usage

This XML format can be used for:

*   **Test Data:** The `all_messages.xml` file or any of the individual event files can be parsed by a test harness to simulate a sequence of hardware events and verify the system's logic.
*   **Asynchronous Interface:** An external application can generate these XML messages and send them to the control system via a message queue (e.g., MQTT, ZeroMQ) or a network socket.
*   **Logging:** The system can serialize internal events into this format for detailed diagnostic logging.
