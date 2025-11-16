#include "xDuinoRails_xTrainAPI.h"
#include "xDuinoRails_xTrainAPI_utils.h"
#include <string>
#include <vector>
#include <variant>

// A simple implementation of a Stream that uses a String as a buffer
class StringStream : public Stream {
public:
    StringStream() : _position(0) {}

    virtual int available() {
        return _buffer.length() - _position;
    }

    virtual int read() {
        if (available()) {
            return _buffer.charAt(_position++);
        }
        return -1;
    }

    virtual int peek() {
        if (available()) {
            return _buffer.charAt(_position);
        }
        return -1;
    }

    virtual size_t write(uint8_t c) {
        _buffer += (char)c;
        return 1;
    }

    void clear() {
        _buffer = "";
        _position = 0;
    }

private:
    String _buffer;
    int _position;
};

// A listener that verifies that the received API call matches the expected one
class VerifyingListener : public ModelRail::IUnifiedModelTrainListener {
public:
    struct ExpectedTrackPowerChanged {
        ModelRail::PowerState state;
    };

    struct ExpectedLocoSpeedChange {
        uint16_t address;
        float speedPercent;
        ModelRail::Direction direction;
    };

    struct ExpectedLocoEventSync {
        uint16_t address;
        ModelRail::SyncType type;
        uint8_t value;
    };

    using ExpectedCall = std::variant<
        std::monostate,
        ExpectedTrackPowerChanged,
        ExpectedLocoSpeedChange,
        ExpectedLocoEventSync
    >;

    VerifyingListener() : _testPassed(false) {}

    void onLocoSpeedChange(const ModelRail::LocoHandle& loco, float speedPercent, ModelRail::Direction direction, int speedSteps) override {
        if (auto* expected = std::get_if<ExpectedLocoSpeedChange>(&_expected)) {
            if (expected->address == loco.address &&
                expected->speedPercent == speedPercent &&
                expected->direction == direction) {
                _testPassed = true;
            }
        }
    }

    void onTrackPowerChange(ModelRail::PowerState state) override {
        if (auto* expected = std::get_if<ExpectedTrackPowerChanged>(&_expected)) {
            if (expected->state == state) {
                _testPassed = true;
            }
        }
    }

    void setExpected(ExpectedCall expected) {
        _expected = expected;
        _testPassed = false;
    }

    bool hasPassed() {
        return _testPassed;
    }

    // Implement other pure virtual functions from the interface
    void onLocoFunctionChange(const ModelRail::LocoHandle& loco, int fIndex, bool isActive) override {}
    void onTurnoutChange(uint16_t address, bool isThrown, bool isFeedback) override {}
    void onSignalAspectChange(uint16_t address, uint8_t aspectId, bool isFeedback) override {}
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
    void onLocoExternalStateChanged(const ModelRail::LocoHandle& loco, ModelRail::ExternalState state) override {}
    void onLocoRailComRawData(const ModelRail::LocoHandle& loco, uint8_t appId, const std::vector<uint8_t>& data) override {}
    void onNewLocoDiscovered(const ModelRail::LocoHandle& loco, const std::string& name, const std::string& icon) override {}
    void onCvReadResult(const ModelRail::LocoHandle& loco, int cvNumber, uint8_t value, bool success) override {}
    void onSusiConfigRead(const ModelRail::LocoHandle& loco, uint8_t bankIndex, uint8_t susiIndex, uint8_t value) override {}
    void onConfigBlockLoad(const ModelRail::LocoHandle& loco, std::string domain, const std::vector<uint8_t>& data) override {}
    void onProgressUpdate(std::string operation, float percent) override {}
    void onLocoEventSync(const ModelRail::LocoHandle& loco, ModelRail::SyncType type, uint32_t value) override {
        if (auto* expected = std::get_if<ExpectedLocoEventSync>(&_expected)) {
            if (expected->address == loco.address &&
                expected->type == type &&
                expected->value == value) {
                _testPassed = true;
            }
        }
    }

private:
    ExpectedCall _expected;
    bool _testPassed;
};

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    StringStream stream;
    ModelRail::CmdLinePrinter printer(stream);
    VerifyingListener listener;
    ModelRail::CmdLineParser parser(listener);

    // Test 1: onTrackPowerChange
    listener.setExpected(VerifyingListener::ExpectedTrackPowerChanged{ModelRail::PowerState::ON});
    printer.onTrackPowerChange(ModelRail::PowerState::ON);
    String command = stream.readStringUntil('>');
    command = command.substring(1);
    parser.parse(command);
    Serial.print("Test onTrackPowerChange: ");
    Serial.println(listener.hasPassed() ? "PASSED" : "FAILED");

    // Test 2: onLocoSpeedChange
    stream.clear();
    listener.setExpected(VerifyingListener::ExpectedLocoSpeedChange{123, 100, ModelRail::Direction::FORWARD});
    ModelRail::LocoHandle loco = {123, ModelRail::Protocol::DCC, 0};
    printer.onLocoSpeedChange(loco, 100, ModelRail::Direction::FORWARD, 128);
    command = stream.readStringUntil('>');
    command = command.substring(1);
    parser.parse(command);
    Serial.print("Test onLocoSpeedChange: ");
    Serial.println(listener.hasPassed() ? "PASSED" : "FAILED");

    // Test 3: onLocoEventSync
    stream.clear();
    listener.setExpected(VerifyingListener::ExpectedLocoEventSync{123, ModelRail::SyncType::CAM_PULSE, 1});
    printer.onLocoEventSync(loco, ModelRail::SyncType::CAM_PULSE, 1);
    command = stream.readStringUntil('>');
    command = command.substring(1);
    parser.parse(command);
    Serial.print("Test onLocoEventSync: ");
    Serial.println(listener.hasPassed() ? "PASSED" : "FAILED");
}

void loop() {
    // Nothing to do here
}
