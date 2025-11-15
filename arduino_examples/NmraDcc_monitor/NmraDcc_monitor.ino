#include <NmraDcc.h>
#include "xDuinoRails_xTrainAPI.h"
#include "xDuinoRails_xTrainAPI_utils.h"

// This example requires the NmraDcc library.
// You can install it from the Arduino Library Manager.

NmraDcc Dcc;
ModelRail::CmdLinePrinter printer(Serial);

void notifyDccSpeed(uint16_t Addr, uint8_t Speed, uint8_t Dir) {
    ModelRail::LocoHandle loco = {Addr, ModelRail::Protocol::DCC, 0};
    ModelRail::Direction direction = (Dir == 1) ? ModelRail::Direction::FORWARD : ModelRail::Direction::REVERSE;
    printer.onLocoSpeedChanged(loco, Speed, direction, 128);
}

void notifyDccFunc(uint16_t Addr, uint8_t Func, uint8_t State) {
    ModelRail::LocoHandle loco = {Addr, ModelRail::Protocol::DCC, 0};
    printer.onLocoFunctionChanged(loco, Func, State);
}

void notifyDccAccTurnoutOutput(uint16_t Addr, uint8_t Output, uint8_t State) {
    printer.onTurnoutChanged(Addr, State, false);
}

void notifyDccClock(uint16_t Minutes, uint8_t Rate, uint8_t Hours, uint8_t Days) {
    // xTrainAPI does not have a clock event, so we will print a custom message.
    Serial.print("DCC Clock: ");
    Serial.print(Hours);
    Serial.print(":");
    Serial.print(Minutes);
    Serial.print(" Rate: ");
    Serial.println(Rate);
}

void notifyDccPower(uint8_t State) {
    printer.onTrackPowerChanged(State ? ModelRail::PowerState::ON : ModelRail::PowerState::OFF);
}

void setup() {
    Serial.begin(115220);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    Dcc.init(DCC_PIN, 0);
}

void loop() {
    Dcc.process();
}
