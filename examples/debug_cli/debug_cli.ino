#include "xDuinoRails_xTrainAPI.h"
#include "xDuinoRails_xTrainAPI_utils.h"
#include "help.h"

class MyTrainAPI : public ModelRail::IUnifiedModelTrainListener {
public:
    void onLocoSpeedChange(const ModelRail::LocoHandle& loco, float speedPercent, ModelRail::Direction direction, int speedSteps) override {
        Serial.print("onLocoSpeedChange(loco.address=");
        Serial.print(loco.address);
        Serial.print(", speedPercent=");
        Serial.print(speedPercent);
        Serial.print(", direction=");
        Serial.print(direction == ModelRail::Direction::FORWARD ? "FORWARD" : "REVERSE");
        Serial.println(")");
    }

    void onLocoFunctionChange(const ModelRail::LocoHandle& loco, int fIndex, bool isActive) override {
        Serial.print("onLocoFunctionChange(loco.address=");
        Serial.print(loco.address);
        Serial.print(", fIndex=");
        Serial.print(fIndex);
        Serial.print(", isActive=");
        Serial.print(isActive ? "true" : "false");
        Serial.println(")");
    }

    void onTurnoutChange(uint16_t address, bool isThrown, bool isFeedback) override {
        Serial.print("onTurnoutChange(address=");
        Serial.print(address);
        Serial.print(", isThrown=");
        Serial.print(isThrown ? "true" : "false");
        Serial.println(")");
    }

    void onSignalAspectChange(uint16_t address, uint8_t aspectId, bool isFeedback) override {
        Serial.print("onSignalAspectChange(address=");
        Serial.print(address);
        Serial.print(", aspectId=");
        Serial.print(aspectId);
        Serial.println(")");
    }

    void onTrackPowerChange(ModelRail::PowerState state) override {
        Serial.print("onTrackPowerChange(state=");
        Serial.print(state == ModelRail::PowerState::ON ? "ON" : "OFF");
        Serial.println(")");
    }

    // Implement other pure virtual functions from the interface
    void onLocoFunctionAnalogChange(const ModelRail::LocoHandle& loco, int fIndex, uint8_t value) override {}
    void onLocoDispatchStateChange(const ModelRail::LocoHandle& loco, bool isAcquired, std::string ownerId) override {}
    void onConsistLink(const ModelRail::LocoHandle& master, const ModelRail::LocoHandle& slave, ModelRail::ConsistType type, bool inverted) override {}
    void onConsistUnlink(const ModelRail::LocoHandle& slave) override {}
    void onAccessoryAnalogValue(uint16_t address, float value0to1) override {}
    void onAccessoryError(uint16_t address, uint8_t errorId, std::string errorMsg) override {}
    void onSensorStateChange(uint32_t sensorId, bool isActive) override {}
    void onFastClockUpdated(int64_t modelTimeUnix, float factor) override {}
    void onHardwareNodeAttached(std::string nodeUid, std::string productName, bool booster) override {}
    void onHardwareNodeLost(std::string nodeUid) override {}
    void onSystemMessage(std::string source, std::string message) override {}
    void onLocoDetectedOnBlock(uint32_t sensorId, const ModelRail::LocoHandle& loco, ModelRail::DecoderOrientation orientation) override {}
    void onLocoTelemetryData(const ModelRail::LocoHandle& loco, ModelRail::TelemetryType type, float value) override {}
    void onLocoExternalStateChange(const ModelRail::LocoHandle& loco, ModelRail::ExternalState state) override {}
    void onLocoRailComRawData(const ModelRail::LocoHandle& loco, uint8_t appId, const std::vector<uint8_t>& data) override {}
    void onNewLocoDiscovered(const ModelRail::LocoHandle& loco, const std::string& name, const std::string& icon) override {}
    void onCvReadDone(const ModelRail::LocoHandle& loco, int cvNumber, uint8_t value, bool success) override {}
    void onSusiConfigRead(const ModelRail::LocoHandle& loco, uint8_t bankIndex, uint8_t susiIndex, uint8_t value) override {}
    void onConfigBlockLoad(const ModelRail::LocoHandle& loco, std::string domain, const std::vector<uint8_t>& data) override {}
    void onProgressUpdate(std::string operation, float percent) override {}
    void onLocoEventSync(const ModelRail::LocoHandle& loco, ModelRail::SyncType type, uint32_t value) override {}
};

MyTrainAPI trainAPI;
ModelRail::CmdLineParser parser(trainAPI);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("xDuinoRails_xTrainAPI Debug CLI");
  Serial.println("Ready to receive commands... Type 'h' for help.");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("h") || command.startsWith("?") || command.startsWith("help")) {
        int firstSpace = command.indexOf(' ');
        String helpCmd = "";
        if (firstSpace != -1) {
            helpCmd = command.substring(firstSpace + 1);
            helpCmd.trim();
        }
        showHelp(helpCmd);
    } else if (command.startsWith("<") && command.endsWith(">")) {
      command = command.substring(1, command.length() - 1);
      parser.parse(command);
    } else if (command.length() > 0) {
      Serial.println("Invalid command format. Commands must be enclosed in < >.");
      showHelp("");
    }
  }
}
