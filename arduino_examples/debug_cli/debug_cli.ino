/*
 * xDuinoRails_xTrainAPI_debug_cli
 *
 * This sketch provides a simple serial command line interface (CLI) to interact
 * with the xDuinoRails_xTrainAPI. It is designed to be used for end-to-end
 * testing with the print_monitor example.
 *
 * This sketch will read commands from the serial port and log the API calls
 * that it would make to the serial port.
 *
 */

#include "xDuinoRails_xTrainAPI.h"

// Dummy implementation of the xTrainAPI for demonstration purposes
class MyTrainAPI : public ModelRail::IUnifiedModelTrainListener {
public:
    void onLocoSpeedChanged(const ModelRail::LocoHandle& loco, float speedPercent, ModelRail::Direction direction, int speedSteps) override {
        Serial.print("onLocoSpeedChanged(loco.address=");
        Serial.print(loco.address);
        Serial.print(", speedPercent=");
        Serial.print(speedPercent);
        Serial.print(", direction=");
        Serial.print(direction == ModelRail::Direction::FORWARD ? "FORWARD" : "REVERSE");
        Serial.println(")");
    }

    void onLocoFunctionChanged(const ModelRail::LocoHandle& loco, int fIndex, bool isActive) override {
        Serial.print("onLocoFunctionChanged(loco.address=");
        Serial.print(loco.address);
        Serial.print(", fIndex=");
        Serial.print(fIndex);
        Serial.print(", isActive=");
        Serial.print(isActive ? "true" : "false");
        Serial.println(")");
    }

    void onTurnoutChanged(uint16_t address, bool isThrown, bool isFeedback) override {
        Serial.print("onTurnoutChanged(address=");
        Serial.print(address);
        Serial.print(", isThrown=");
        Serial.print(isThrown ? "true" : "false");
        Serial.println(")");
    }

    void onSignalAspectChanged(uint16_t address, uint8_t aspectId, bool isFeedback) override {
        Serial.print("onSignalAspectChanged(address=");
        Serial.print(address);
        Serial.print(", aspectId=");
        Serial.print(aspectId);
        Serial.println(")");
    }

    void onTrackPowerChanged(ModelRail::PowerState state) override {
        Serial.print("onTrackPowerChanged(state=");
        Serial.print(state == ModelRail::PowerState::ON ? "ON" : "OFF");
        Serial.println(")");
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

MyTrainAPI trainAPI;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("xDuinoRails_xTrainAPI Debug CLI");
  Serial.println("Ready to receive commands...");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('>');
    if (command.startsWith("<")) {
      command = command.substring(1);
      parseCommand(command);
    }
  }
}

void parseCommand(String command) {
  char cmdChar = command.charAt(0);
  String params = command.substring(1);
  params.trim();

  switch (cmdChar) {
    case '1':
      trainAPI.onTrackPowerChanged(ModelRail::PowerState::ON);
      break;
    case '0':
      trainAPI.onTrackPowerChanged(ModelRail::PowerState::OFF);
      break;
    case 't':
      {
        int firstSpace = params.indexOf(' ');
        int secondSpace = params.indexOf(' ', firstSpace + 1);
        if (firstSpace != -1 && secondSpace != -1) {
          uint16_t cab = params.substring(0, firstSpace).toInt();
          int speed = params.substring(firstSpace + 1, secondSpace).toInt();
          int dir = params.substring(secondSpace + 1).toInt();

          ModelRail::LocoHandle loco = {cab, ModelRail::Protocol::DCC, 0};
          trainAPI.onLocoSpeedChanged(loco, speed, (ModelRail::Direction)dir, 128);
        } else {
          Serial.println("Invalid t command format. Use: <t CAB SPEED DIR>");
        }
      }
      break;
    case 'f':
      {
        int firstSpace = params.indexOf(' ');
        int secondSpace = params.indexOf(' ', firstSpace + 1);
        if (firstSpace != -1 && secondSpace != -1) {
          uint16_t cab = params.substring(0, firstSpace).toInt();
          int func = params.substring(firstSpace + 1, secondSpace).toInt();
          int state = params.substring(secondSpace + 1).toInt();

          ModelRail::LocoHandle loco = {cab, ModelRail::Protocol::DCC, 0};
          trainAPI.onLocoFunctionChanged(loco, func, state);
        } else {
          Serial.println("Invalid f command format. Use: <f CAB FUNC STATE>");
        }
      }
      break;
    case 'T':
      {
        int firstSpace = params.indexOf(' ');
        if (firstSpace != -1) {
          uint16_t addr = params.substring(0, firstSpace).toInt();
          int state = params.substring(firstSpace + 1).toInt();
          trainAPI.onTurnoutChanged(addr, state, false);
        } else {
          Serial.println("Invalid T command format. Use: <T ADDR STATE>");
        }
      }
      break;
    case 'S':
      {
        int firstSpace = params.indexOf(' ');
        if (firstSpace != -1) {
          uint16_t addr = params.substring(0, firstSpace).toInt();
          int aspect = params.substring(firstSpace + 1).toInt();
          trainAPI.onSignalAspectChanged(addr, aspect, false);
        } else {
          Serial.println("Invalid S command format. Use: <S ADDR ASPECT>");
        }
      }
      break;
    default:
      Serial.println("Unknown command");
  }
}
