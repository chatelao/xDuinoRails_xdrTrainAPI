#include "xDuinoRails_xTrainAPI.h"
#include "xDuinoRails_xTrainAPI_utils.h"

#define TEST_SET 1

ModelRail::CmdLinePrinter printer(Serial);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

#if TEST_SET == 1
  // Test Set 1
  printer.onTrackPowerChanged(ModelRail::PowerState::ON);
  ModelRail::LocoHandle loco1 = {123, ModelRail::Protocol::DCC, 0};
  printer.onLocoSpeedChanged(loco1, 100, ModelRail::Direction::FORWARD, 128);
  printer.onLocoFunctionChanged(loco1, 1, true);
  printer.onTurnoutChanged(456, true, false);
  printer.onSignalAspectChanged(789, 3, false);
  printer.onTrackPowerChanged(ModelRail::PowerState::OFF);
#elif TEST_SET == 2
  // Test Set 2
  printer.onTrackPowerChanged(ModelRail::PowerState::ON);
  ModelRail::LocoHandle loco2 = {3, ModelRail::Protocol::DCC, 0};
  printer.onLocoSpeedChanged(loco2, 50, ModelRail::Direction::REVERSE, 128);
  printer.onLocoFunctionChanged(loco2, 2, true);
  printer.onLocoFunctionChanged(loco2, 3, true);
  printer.onTurnoutChanged(10, false, false);
  printer.onSignalAspectChanged(11, 1, false);
  printer.onLocoSpeedChanged(loco2, 0, ModelRail::Direction::REVERSE, 128);
  printer.onTrackPowerChanged(ModelRail::PowerState::OFF);
#endif
}

void loop() {
  // Nothing to do here
}
