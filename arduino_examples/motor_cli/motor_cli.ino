#include <Arduino.h>
#include <XDuinoRails_MotorDriver.h>
#include <xDuinoRails_xTrainAPI_utils.h>

// 1. Define Pin Connections
const int MOTOR_PWM_A_PIN = 7;
const int MOTOR_PWM_B_PIN = 8;
const int MOTOR_BEMF_A_PIN = A3;
const int MOTOR_BEMF_B_PIN = A2;

// 2. Create an instance of the motor driver
XDuinoRails_MotorDriver motor(MOTOR_PWM_A_PIN, MOTOR_PWM_B_PIN, MOTOR_BEMF_A_PIN, MOTOR_BEMF_B_PIN);

class MotorControlListener : public ModelRail::IUnifiedModelTrainListener {
public:
    MotorControlListener(XDuinoRails_MotorDriver& motor) : _motor(motor) {}

    void onLocoSpeedChanged(const ModelRail::LocoHandle& loco, float speedPercent, ModelRail::Direction direction, int speedSteps) override {
        _motor.setTargetSpeed(speedPercent);
        _motor.setDirection(direction == ModelRail::Direction::FORWARD);
    }

    // Implement other pure virtual functions from the interface
    void onLocoFunctionChanged(const ModelRail::LocoHandle& loco, int fIndex, bool isActive) override {}
    void onLocoFunctionAnalogValue(const ModelRail::LocoHandle& loco, int fIndex, uint8_t value) override {}
    void onLocoDispatchStateChanged(const ModelRail::LocoHandle& loco, bool isAcquired, std::string ownerId) override {}
    void onConsistLink(const ModelRail::LocoHandle& master, const ModelRail::LocoHandle& slave, ModelRail::ConsistType type, bool inverted) override {}
    void onConsistUnlink(const ModelRail::LocoHandle& slave) override {}
    void onTurnoutChanged(uint16_t address, bool isThrown, bool isFeedback) override {}
    void onSignalAspectChanged(uint16_t address, uint8_t aspectId, bool isFeedback) override {}
    void onAccessoryAnalogValue(uint16_t address, float value0to1) override {}
    void onAccessoryError(uint16_t address, uint8_t errorId, std::string errorMsg) override {}
    void onSensorStateChanged(uint32_t sensorId, bool isActive) override {}
    void onTrackPowerChanged(ModelRail::PowerState state) override {}
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
    void onMechanicalSyncEvent(const ModelRail::LocoHandle& loco, ModelRail::SyncType type, uint8_t value) override {}

private:
    XDuinoRails_MotorDriver& _motor;
};

MotorControlListener listener(motor);
ModelRail::CmdLineParser parser(listener);

void setup() {
  Serial.begin(115200);
  motor.begin();
  Serial.println("Motor CLI initialized.");
  Serial.println("Example command:");
  Serial.println("  THROTTLE cab=\"1\" speed=\"100\" direction=\"1\"");
}

void loop() {
  motor.update();

  if (Serial.available() > 0) {
    String line = Serial.readStringUntil('\n');
    parser.parse(line);
  }
}
