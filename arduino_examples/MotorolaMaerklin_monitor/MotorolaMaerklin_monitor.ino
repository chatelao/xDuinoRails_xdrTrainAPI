#include <MotorolaMaerklin.h>
#include "xDuinoRails_xTrainAPI.h"
#include "xDuinoRails_xTrainAPI_utils.h"

// This example requires the laserlight/MotorolaMaerklin library.
// You can install it from the Arduino Library Manager.

MotorolaMaerklin Mm(2); // Use pin 2 for the DCC signal
ModelRail::CmdLinePrinter printer(Serial);

/*
 * @brief This is a placeholder callback function.
 * The MotorolaMaerklin library is not available in the current environment,
 * so this function serves as a template for how to map Maerklin events
 * to the xTrainAPI. The actual function signature and event mapping will
 * depend on the library's implementation.
 */
void notifyMmEvent(uint8_t address, uint8_t command, uint8_t data) {
    ModelRail::LocoHandle loco = {address, ModelRail::Protocol::MM_II, 0};

    // Example mapping:
    switch (command) {
        case 0x00: // Speed and direction
            {
                ModelRail::Direction direction = (data & 0x80) ? ModelRail::Direction::FORWARD : ModelRail::Direction::REVERSE;
                uint8_t speed = data & 0x7F;
                printer.onLocoSpeedChange(loco, speed, direction, 14);
            }
            break;
        case 0x01: // Function on/off
            {
                bool isActive = (data & 0x01);
                printer.onLocoFunctionChanged(loco, 0, isActive); // F0
            }
            break;
        // Add more mappings for other commands as needed.
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    Mm.setCallback(notifyMmEvent);
}

void loop() {
    Mm.loop();
}
