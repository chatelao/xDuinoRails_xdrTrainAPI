/*
 * xDuinoRails_xTrainAPI_print_monitor
 *
 * This sketch demonstrates how to implement the IUnifiedModelTrainListener to
 * receive events from the xDuinoRails_xTrainAPI and print them to the serial
 * port in a DCC-EX like command format.
 *
 * This sketch is designed to be used for end-to-end testing with the
 * debug_cli example. It will generate a command stream that can be piped
 * to the debug_cli sketch.
 *
 * To test with a different test set, change the TEST_SET macro.
 *
 */

#include "xDuinoRails_xTrainAPI.h"

#define TEST_SET 1

class PrintMonitor : public ModelRail::IUnifiedModelTrainListener {
public:
    void onLocoSpeedChanged(const ModelRail::LocoHandle& loco, float speedPercent, ModelRail::Direction direction, int speedSteps) override {
        Serial.print("<t ");
        Serial.print(loco.address);
        Serial.print(" ");
        Serial.print((int)speedPercent);
        Serial.print(" ");
        Serial.print(direction == ModelRail::Direction::FORWARD ? 1 : 0);
        Serial.println(">");
    }

    void onLocoFunctionChanged(const ModelRail::LocoHandle& loco, int fIndex, bool isActive) override {
        Serial.print("<f ");
        Serial.print(loco.address);
        Serial.print(" ");
        Serial.print(fIndex);
        Serial.print(" ");
        Serial.print(isActive ? 1 : 0);
        Serial.println(">");
    }

    void onTurnoutChanged(uint16_t address, bool isThrown, bool isFeedback) override {
        Serial.print("<T ");
        Serial.print(address);
        Serial.print(" ");
        Serial.print(isThrown ? 1 : 0);
        Serial.println(">");
    }

    void onSignalAspectChanged(uint16_t address, uint8_t aspectId, bool isFeedback) override {
        Serial.print("<S ");
        Serial.print(address);
        Serial.print(" ");
        Serial.print(aspectId);
        Serial.println(">");
    }

    void onTrackPowerChanged(ModelRail::PowerState state) override {
        Serial.print("<");
        Serial.print(state == ModelRail::PowerState::ON ? 1 : 0);
        Serial.println(">");
    }

    // Implement other pure virtual functions from the interface
    void onLocoFunctionAnalogValue(const ModelRail::LocoHandle& loco, int fIndex, uint8_t value) override {}
    void onLocoDispatchStateChanged(const ModelRail::LocoHandle& loco, bool isAcquired, std::string ownerId) override {}
    void onConsistLink(const ModelRail::LocoHandle& master, const ModelRail::LocoHandle& slave, ModelRail::ConsistType type, bool inverted) override {}
    void onConsistUnlink(const ModelRail::LocoHandle& slave) override {}
    void onAccessoryAnalogValue(uint16_t address, float value0to1) override {}
    void onAccessoryError(uint16_t address, uint8_t errorId, std::string errorMsg) override {}
    void onSensorStateChanged(uint32_t sensorId, bool isActive) override {}
    void onFastClockUpdated(int64_t modelTimeUnix, float factor) override {}
    void onHardwareNodeAttached(std::string nodeUid, std::string productName, bool booster) override {}
    void onHardwareNodeLost(std::string nodeUid) override {}
    void onSystemMessage(std::string source, std::string message) override {}
    void onLocoDetectedOnBlock(uint32_t sensorId, const ModelRail::LocoHandle& loco, ModelRail::DecoderOrientation orientation) override {}
    void onLocoTelemetryData(const ModelRail::LocoHandle& loco, ModelRail::TelemetryType type, float value) override {}
    void onLocoExternalStateChanged(const ModelRail::LocoHandle& loco, ModelRail::ExternalState state) override {}
    void onLocoRailComRawData(const ModelRail::LocoHandle& loco, uint8_t appId, const std::vector<uint8_t>& data) override {}
    void onNewLocoDiscovered(const ModelRail::LocoHandle& loco, const std::string& name, const std::string& icon) override {}
    void onCvReadResult(const ModelRail::LocoHandle& loco, int cvNumber, uint8_t value, bool success) override {}
    void onSusiConfigRead(const ModelRail::LocoHandle& loco, uint8_t bankIndex, uint8_t susiIndex, uint8_t value) override {}
    void onConfigBlockLoaded(const ModelRail::LocoHandle& loco, std::string domain, const std::vector<uint8_t>& data) override {}
    void onProgressUpdate(std::string operation, float percent) override {}
};

PrintMonitor printMonitor;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

#if TEST_SET == 1
  // Test Set 1
  printMonitor.onTrackPowerChanged(ModelRail::PowerState::ON);
  ModelRail::LocoHandle loco1 = {123, ModelRail::Protocol::DCC, 0};
  printMonitor.onLocoSpeedChanged(loco1, 100, ModelRail::Direction::FORWARD, 128);
  printMonitor.onLocoFunctionChanged(loco1, 1, true);
  printMonitor.onTurnoutChanged(456, true, false);
  printMonitor.onSignalAspectChanged(789, 3, false);
  printMonitor.onTrackPowerChanged(ModelRail::PowerState::OFF);
#elif TEST_SET == 2
  // Test Set 2
  printMonitor.onTrackPowerChanged(ModelRail::PowerState::ON);
  ModelRail::LocoHandle loco2 = {3, ModelRail::Protocol::DCC, 0};
  printMonitor.onLocoSpeedChanged(loco2, 50, ModelRail::Direction::REVERSE, 128);
  printMonitor.onLocoFunctionChanged(loco2, 2, true);
  printMonitor.onLocoFunctionChanged(loco2, 3, true);
  printMonitor.onTurnoutChanged(10, false, false);
  printMonitor.onSignalAspectChanged(11, 1, false);
  printMonitor.onLocoSpeedChanged(loco2, 0, ModelRail::Direction::REVERSE, 128);
  printMonitor.onTrackPowerChanged(ModelRail::PowerState::OFF);
#endif
}

void loop() {
  // Nothing to do here
}
