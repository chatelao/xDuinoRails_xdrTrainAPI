#include <xDuinoRails_xTrainAPI_utils.h>

class VerifyingListener : public ModelRail::IUnifiedModelTrainListener {
public:
    void onLocoSpeedChanged(const ModelRail::LocoHandle& loco, float speedPercent, ModelRail::Direction direction, int speedSteps) override {
        ModelRail::XmlPrinter printer(Serial);
        printer.onLocoSpeedChanged(loco, speedPercent, direction, speedSteps);
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
};

VerifyingListener listener;
ModelRail::XmlParser parser(listener);

void setup() {
    Serial.begin(115200);
}

void loop() {
    if (Serial.available() > 0) {
        String xml = Serial.readString();
        parser.parse(xml);
    }
}
