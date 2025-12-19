#pragma once

// The Arduino build system adds the library's root directory to the include path,
// so we can include the header directly.
#include "xDuinoRails_xTrainAPI.h"
#include "yxml.h"
#include <Arduino.h>
#include <vector>
#include <string>

#ifndef USE_EXTENDED_CLI_SYNTAX
#define USE_EXTENDED_CLI_SYNTAX 1
#endif

namespace ModelRail {

// Helper to convert hex string to bytes
static std::vector<uint8_t> hexToBytes(const std::string& hex) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        char byte = (char)strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

// Helper to convert bytes to hex string
static void printHex(Stream* stream, const std::vector<uint8_t>& data) {
    for (uint8_t byte : data) {
        if (byte < 16) stream->print('0');
        stream->print(byte, HEX);
    }
}

class CmdLinePrinter : public IUnifiedModelTrainListener {
public:
    CmdLinePrinter(Stream& stream) : _stream(&stream) {}

    uint64_t getImplementedApi() const override { return (1ULL << 28) - 1; }

    void onLocoSpeedChange(const LocoHandle& loco, float speedPercent, Direction direction, int speedSteps) override {
#if USE_EXTENDED_CLI_SYNTAX
        _stream->print("<THROTTLE cab=\"");
        _stream->print(loco.address);
        _stream->print("\" speed=\"");
        _stream->print((int)speedPercent);
        _stream->print("\" direction=\"");
        _stream->print(direction == Direction::FORWARD ? 1 : 0);
        _stream->print("\" steps=\"");
        _stream->print(speedSteps);
        _stream->println("\">");
#else
        _stream->print("<t ");
        _stream->print(loco.address);
        _stream->print(" ");
        _stream->print((int)speedPercent);
        _stream->print(" ");
        _stream->print(direction == Direction::FORWARD ? 1 : 0);
        _stream->println(">");
#endif
    }

    void onLocoFunctionChange(const LocoHandle& loco, int fIndex, bool isActive) override {
#if USE_EXTENDED_CLI_SYNTAX
        _stream->print("<FUNCTION cab=\"");
        _stream->print(loco.address);
        _stream->print("\" function=\"");
        _stream->print(fIndex);
        _stream->print("\" state=\"");
        _stream->print(isActive ? 1 : 0);
        _stream->println("\">");
#else
        _stream->print("<f ");
        _stream->print(loco.address);
        _stream->print(" ");
        _stream->print(fIndex);
        _stream->print(" ");
        _stream->print(isActive ? 1 : 0);
        _stream->println(">");
#endif
    }

    void onTurnoutChange(uint16_t address, bool isThrown, bool isFeedback) override {
#if USE_EXTENDED_CLI_SYNTAX
        _stream->print("<TURNOUT id=\"");
        _stream->print(address);
        _stream->print("\" state=\"");
        _stream->print(isThrown ? 1 : 0);
        _stream->println("\">");
#else
        _stream->print("<T ");
        _stream->print(address);
        _stream->print(" ");
        _stream->print(isThrown ? 1 : 0);
        _stream->println(">");
#endif
    }

    void onSignalAspectChange(uint16_t address, uint8_t aspectId, bool isFeedback) override {
#if USE_EXTENDED_CLI_SYNTAX
        _stream->print("<ACCESSORY id=\"");
        _stream->print(address);
        _stream->print("\" state=\"");
        _stream->print(aspectId);
        _stream->println("\">");
#else
        _stream->print("<S ");
        _stream->print(address);
        _stream->print(" ");
        _stream->print(aspectId);
        _stream->println(">");
#endif
    }

    void onTrackPowerChange(PowerState state) override {
#if USE_EXTENDED_CLI_SYNTAX
        _stream->print("<POWER state=\"");
        _stream->print(state == PowerState::ON ? "ON" : "OFF");
        _stream->println("\">");
#else
        _stream->print("<");
        _stream->print(state == PowerState::ON ? 1 : 0);
        _stream->println(">");
#endif
    }

    void onLocoFunctionAnalogChange(const LocoHandle& loco, int fIndex, uint8_t value) override {
        _stream->print("<LOCO_FUNCTION_ANALOG cab=\"");
        _stream->print(loco.address);
        _stream->print("\" function=\"");
        _stream->print(fIndex);
        _stream->print("\" value=\"");
        _stream->print(value);
        _stream->println("\">");
    }

    void onLocoDispatchStateChange(const LocoHandle& loco, bool isAcquired, std::string ownerId) override {
        _stream->print("<DISPATCH cab=\"");
        _stream->print(loco.address);
        _stream->print("\" state=\"");
        _stream->print(isAcquired ? "ACQUIRED" : "RELEASED");
        _stream->print("\" owner=\"");
        _stream->print(ownerId.c_str());
        _stream->println("\">");
    }

    void onConsistLink(const LocoHandle& master, const LocoHandle& slave, ConsistType type, bool inverted) override {
        _stream->print("<CONSIST_LINK master=\"");
        _stream->print(master.address);
        _stream->print("\" slave=\"");
        _stream->print(slave.address);
        _stream->print("\" type=\"");
        switch (type) {
            case ConsistType::ADVANCED_DCC: _stream->print("ADVANCED"); break;
            case ConsistType::UNIVERSAL_HOST: _stream->print("UNIVERSAL"); break;
            case ConsistType::MU_LOCONET: _stream->print("LOCONET"); break;
        }
        _stream->print("\" inverted=\"");
        _stream->print(inverted ? "true" : "false");
        _stream->println("\">");
    }

    void onConsistUnlink(const LocoHandle& slave) override {
        _stream->print("<CONSIST_UNLINK slave=\"");
        _stream->print(slave.address);
        _stream->println("\">");
    }

    void onAccessoryAnalogValue(uint16_t address, float value0to1) override {
        _stream->print("<ACCESSORY_ANALOG address=\"");
        _stream->print(address);
        _stream->print("\" value=\"");
        _stream->print(value0to1);
        _stream->println("\">");
    }

    void onAccessoryError(uint16_t address, uint8_t errorId, std::string errorMsg) override {
        _stream->print("<ACCESSORY_ERROR address=\"");
        _stream->print(address);
        _stream->print("\" errorId=\"");
        _stream->print(errorId);
        _stream->print("\" errorMessage=\"");
        _stream->print(errorMsg.c_str());
        _stream->println("\">");
    }

    void onSensorStateChange(uint32_t sensorId, bool isActive) override {
        _stream->print("<SENSOR id=\"");
        _stream->print(sensorId);
        _stream->print("\" state=\"");
        _stream->print(isActive ? "ACTIVE" : "INACTIVE");
        _stream->println("\">");
    }

    void onFastClockUpdated(int64_t modelTimeUnix, float factor) override {
        _stream->print("<FAST_CLOCK time=\"");
        _stream->print((long)modelTimeUnix);
        _stream->print("\" factor=\"");
        _stream->print(factor);
        _stream->println("\">");
    }

    void onHardwareNodeAttached(std::string nodeUid, std::string productName, bool booster) override {
        _stream->print("<HARDWARE_NODE_ATTACHED uid=\"");
        _stream->print(nodeUid.c_str());
        _stream->print("\" productName=\"");
        _stream->print(productName.c_str());
        _stream->print("\" booster=\"");
        _stream->print(booster ? "true" : "false");
        _stream->println("\">");
    }

    void onHardwareNodeLost(std::string nodeUid) override {
        _stream->print("<HARDWARE_NODE_LOST uid=\"");
        _stream->print(nodeUid.c_str());
        _stream->println("\">");
    }

    void onSystemMessage(std::string source, std::string message) override {
        _stream->print("<SYSTEM_MESSAGE source=\"");
        _stream->print(source.c_str());
        _stream->print("\" message=\"");
        _stream->print(message.c_str());
        _stream->println("\">");
    }

    void onLocoDetectedOnBlock(uint32_t sensorId, const LocoHandle& loco, DecoderOrientation orientation) override {
        _stream->print("<LOCO_DETECTED_ON_BLOCK sensorId=\"");
        _stream->print(sensorId);
        _stream->print("\" cab=\"");
        _stream->print(loco.address);
        _stream->print("\" orientation=\"");
        switch (orientation) {
            case DecoderOrientation::NORMAL: _stream->print("NORMAL"); break;
            case DecoderOrientation::INVERTED: _stream->print("INVERTED"); break;
            case DecoderOrientation::UNKNOWN: _stream->print("UNKNOWN"); break;
        }
        _stream->println("\">");
    }

    void onLocoTelemetryData(const LocoHandle& loco, TelemetryType type, float value) override {
        _stream->print("<LOCO_TELEMETRY cab=\"");
        _stream->print(loco.address);
        _stream->print("\" type=\"");
        switch (type) {
            case TelemetryType::SPEED_KMH: _stream->print("SPEED"); break;
            default: _stream->print("GENERIC"); break;
        }
        _stream->print("\" value=\"");
        _stream->print(value);
        _stream->println("\">");
    }

    void onLocoExternalStateChange(const LocoHandle& loco, ExternalState state) override {
        _stream->print("<LOCO_EXTERNAL_STATE cab=\"");
        _stream->print(loco.address);
        _stream->print("\" state=\"");
        _stream->print((int)state);
        _stream->println("\">");
    }

    void onLocoRailComRawData(const LocoHandle& loco, uint8_t appId, const std::vector<uint8_t>& data) override {
        _stream->print("<LOCO_RAILCOM_RAW cab=\"");
        _stream->print(loco.address);
        _stream->print("\" appId=\"");
        _stream->print(appId);
        _stream->print("\" data=\"");
        printHex(_stream, data);
        _stream->println("\">");
    }

    void onNewLocoDiscovered(const LocoHandle& loco, const std::string& name, const std::string& icon) override {
        _stream->print("<NEW_LOCO_DISCOVERED cab=\"");
        _stream->print(loco.address);
        _stream->print("\" name=\"");
        _stream->print(name.c_str());
        _stream->print("\" icon=\"");
        _stream->print(icon.c_str());
        _stream->println("\">");
    }

    void onCvRead(const LocoHandle& loco, int cvNumber) override {
        _stream->print("<READ_CV cv=\"");
        _stream->print(cvNumber);
        _stream->print("\" track=\"");
        _stream->print(loco.address > 0 ? 0 : 1);
        _stream->println("\">");
    }

    void onCvWrite(const LocoHandle& loco, int cvNumber, uint8_t value) override {
        _stream->print("<WRITE_CV_BYTE cv=\"");
        _stream->print(cvNumber);
        _stream->print("\" value=\"");
        _stream->print(value);
        _stream->print("\" track=\"");
        _stream->print(loco.address > 0 ? 0 : 1);
        _stream->println("\">");
    }

    void onCvReadDone(const LocoHandle& loco, int cvNumber, uint8_t value, bool success) override {
        _stream->print("<CV_READ_RESULT cab=\"");
        _stream->print(loco.address);
        _stream->print("\" cv=\"");
        _stream->print(cvNumber);
        _stream->print("\" value=\"");
        _stream->print(value);
        _stream->print("\" success=\"");
        _stream->print(success ? "true" : "false");
        _stream->println("\">");
    }

    void onSusiConfigRead(const LocoHandle& loco, uint8_t bankIndex, uint8_t susiIndex, uint8_t value) override {
        _stream->print("<SUSI_CONFIG_READ cab=\"");
        _stream->print(loco.address);
        _stream->print("\" bank=\"");
        _stream->print(bankIndex);
        _stream->print("\" index=\"");
        _stream->print(susiIndex);
        _stream->print("\" value=\"");
        _stream->print(value);
        _stream->println("\">");
    }

    void onConfigBlockLoad(const LocoHandle& loco, std::string domain, const std::vector<uint8_t>& data) override {
        _stream->print("<CONFIG_BLOCK_LOADED cab=\"");
        _stream->print(loco.address);
        _stream->print("\" domain=\"");
        _stream->print(domain.c_str());
        _stream->print("\" data=\"");
        printHex(_stream, data);
        _stream->println("\">");
    }

    void onProgressUpdate(std::string operation, float percent) override {
        _stream->print("<PROGRESS_UPDATE operation=\"");
        _stream->print(operation.c_str());
        _stream->print("\" percent=\"");
        _stream->print(percent);
        _stream->println("\">");
    }

    void onLocoEventSync(const LocoHandle& loco, SyncType type, uint32_t value) override {
#if USE_EXTENDED_CLI_SYNTAX
        _stream->print("<MECHANICAL_SYNC_EVENT cab=\"");
        _stream->print(loco.address);
        _stream->print("\" type=\"");
        switch (type) {
            case SyncType::CAM_PULSE: _stream->print("CAM_PULSE"); break;
            case SyncType::CYLINDER_CYCLE: _stream->print("CYLINDER_CYCLE"); break;
            case SyncType::GEAR_CHANGE_UP: _stream->print("GEAR_CHANGE_UP"); break;
            case SyncType::GEAR_CHANGE_DOWN: _stream->print("GEAR_CHANGE_DOWN"); break;
            case SyncType::BRAKE_SQUEAL_START: _stream->print("BRAKE_SQUEAL_START"); break;
            case SyncType::DOOR_MOVEMENT: _stream->print("DOOR_MOVEMENT"); break;
        }
        _stream->print("\" value=\"");
        _stream->print(value);
        _stream->println("\">");
#else
        _stream->print("<m ");
        _stream->print(loco.address);
        _stream->print(" ");
        _stream->print(static_cast<uint8_t>(type));
        _stream->print(" ");
        _stream->print(value);
        _stream->println(">");
#endif
    }

private:
    Stream* _stream;
};

class XmlPrinter : public IUnifiedModelTrainListener {
public:
    XmlPrinter(Stream& stream) : _stream(&stream) {}

    uint64_t getImplementedApi() const override { return (1ULL << 28) - 1; }

    void begin() {
        _stream->println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        _stream->println("<xTrainEvents>");
    }

    void end() {
        _stream->println("</xTrainEvents>");
    }

    void onLocoSpeedChange(const LocoHandle& loco, float speedPercent, Direction direction, int speedSteps) override {
        _stream->println("    <event type=\"onLocoSpeedChange\">");
        printLoco(loco);
        _stream->print("        <speed speedPercent=\"");
        _stream->print(speedPercent);
        _stream->print("\" direction=\"");
        _stream->print(direction == Direction::FORWARD ? "FORWARD" : "REVERSE");
        _stream->print("\" speedSteps=\"");
        _stream->print(speedSteps);
        _stream->println("\" />");
        _stream->println("    </event>");
    }

    void onLocoFunctionChange(const LocoHandle& loco, int fIndex, bool isActive) override {
        _stream->println("    <event type=\"onLocoFunctionChange\">");
        printLoco(loco);
        _stream->print("        <function fIndex=\"");
        _stream->print(fIndex);
        _stream->print("\" isActive=\"");
        _stream->print(isActive ? "true" : "false");
        _stream->println("\" />");
        _stream->println("    </event>");
    }

    void onLocoFunctionAnalogChange(const LocoHandle& loco, int fIndex, uint8_t value) override {
        _stream->println("    <event type=\"onLocoFunctionAnalogChange\">");
        printLoco(loco);
        _stream->print("        <function fIndex=\"");
        _stream->print(fIndex);
        _stream->print("\" value=\"");
        _stream->print(value);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onLocoDispatchStateChange(const LocoHandle& loco, bool isAcquired, std::string ownerId) override {
        _stream->println("    <event type=\"onLocoDispatchStateChange\">");
        printLoco(loco);
        _stream->print("        <dispatch isAcquired=\"");
        _stream->print(isAcquired ? "true" : "false");
        _stream->print("\" ownerId=\"");
        escapeXml(ownerId);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onConsistLink(const LocoHandle& master, const LocoHandle& slave, ConsistType type, bool inverted) override {
        _stream->println("    <event type=\"onConsistLink\">");
        _stream->print("        <master address=\"");
        _stream->print(master.address);
        _stream->print("\" protocol=\"");
        printProtocol(master.protocol);
        _stream->println("\" />");
        _stream->print("        <slave address=\"");
        _stream->print(slave.address);
        _stream->print("\" protocol=\"");
        printProtocol(slave.protocol);
        _stream->println("\" />");
        _stream->print("        <consist type=\"");
        switch (type) {
            case ConsistType::ADVANCED_DCC: _stream->print("ADVANCED_DCC"); break;
            case ConsistType::UNIVERSAL_HOST: _stream->print("UNIVERSAL_HOST"); break;
            case ConsistType::MU_LOCONET: _stream->print("MU_LOCONET"); break;
        }
        _stream->print("\" inverted=\"");
        _stream->print(inverted ? "true" : "false");
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onConsistUnlink(const LocoHandle& slave) override {
        _stream->println("    <event type=\"onConsistUnlink\">");
        _stream->print("        <slave address=\"");
        _stream->print(slave.address);
        _stream->print("\" protocol=\"");
        printProtocol(slave.protocol);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onTurnoutChange(uint16_t address, bool isThrown, bool isFeedback) override {
        _stream->println("    <event type=\"onTurnoutChange\">");
        _stream->print("        <turnout address=\"");
        _stream->print(address);
        _stream->print("\" isThrown=\"");
        _stream->print(isThrown ? "true" : "false");
        _stream->print("\" isFeedback=\"");
        _stream->print(isFeedback ? "true" : "false");
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onSignalAspectChange(uint16_t address, uint8_t aspectId, bool isFeedback) override {
        _stream->println("    <event type=\"onSignalAspectChange\">");
        _stream->print("        <signal address=\"");
        _stream->print(address);
        _stream->print("\" aspectId=\"");
        _stream->print(aspectId);
        _stream->print("\" isFeedback=\"");
        _stream->print(isFeedback ? "true" : "false");
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onAccessoryAnalogValue(uint16_t address, float value0to1) override {
        _stream->println("    <event type=\"onAccessoryAnalogValue\">");
        _stream->print("        <accessory address=\"");
        _stream->print(address);
        _stream->print("\" value0to1=\"");
        _stream->print(value0to1);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onAccessoryError(uint16_t address, uint8_t errorId, std::string errorMsg) override {
        _stream->println("    <event type=\"onAccessoryError\">");
        _stream->print("        <accessory address=\"");
        _stream->print(address);
        _stream->print("\" errorId=\"");
        _stream->print(errorId);
        _stream->print("\" errorMsg=\"");
        escapeXml(errorMsg);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onSensorStateChange(uint32_t sensorId, bool isActive) override {
        _stream->println("    <event type=\"onSensorStateChange\">");
        _stream->print("        <sensor sensorId=\"");
        _stream->print(sensorId);
        _stream->print("\" isActive=\"");
        _stream->print(isActive ? "true" : "false");
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onTrackPowerChange(PowerState state) override {
        _stream->println("    <event type=\"onTrackPowerChange\">");
        _stream->print("        <power state=\"");
        switch (state) {
            case PowerState::OFF: _stream->print("OFF"); break;
            case PowerState::ON: _stream->print("ON"); break;
            case PowerState::EMERGENCY_STOP: _stream->print("EMERGENCY_STOP"); break;
            case PowerState::SHORT_CIRCUIT: _stream->print("SHORT_CIRCUIT"); break;
        }
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onFastClockUpdated(int64_t modelTimeUnix, float factor) override {
        _stream->println("    <event type=\"onFastClockUpdated\">");
        _stream->print("        <clock modelTimeUnix=\"");
        _stream->print((long)modelTimeUnix);
        _stream->print("\" factor=\"");
        _stream->print(factor);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onHardwareNodeAttached(std::string nodeUid, std::string productName, bool booster) override {
        _stream->println("    <event type=\"onHardwareNodeAttached\">");
        _stream->print("        <node nodeUid=\"");
        escapeXml(nodeUid);
        _stream->print("\" productName=\"");
        escapeXml(productName);
        _stream->print("\" booster=\"");
        _stream->print(booster ? "true" : "false");
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onHardwareNodeLost(std::string nodeUid) override {
        _stream->println("    <event type=\"onHardwareNodeLost\">");
        _stream->print("        <node nodeUid=\"");
        escapeXml(nodeUid);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onSystemMessage(std::string source, std::string message) override {
        _stream->println("    <event type=\"onSystemMessage\">");
        _stream->print("        <message source=\"");
        escapeXml(source);
        _stream->print("\" message=\"");
        escapeXml(message);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onLocoDetectedOnBlock(uint32_t sensorId, const LocoHandle& loco, DecoderOrientation orientation) override {
        _stream->println("    <event type=\"onLocoDetectedOnBlock\">");
        printLoco(loco);
        _stream->print("        <detection sensorId=\"");
        _stream->print(sensorId);
        _stream->print("\" orientation=\"");
        switch (orientation) {
            case DecoderOrientation::NORMAL: _stream->print("NORMAL"); break;
            case DecoderOrientation::INVERTED: _stream->print("INVERTED"); break;
            case DecoderOrientation::UNKNOWN: _stream->print("UNKNOWN"); break;
        }
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onLocoTelemetryData(const LocoHandle& loco, TelemetryType type, float value) override {
        _stream->println("    <event type=\"onLocoTelemetryData\">");
        printLoco(loco);
        _stream->print("        <telemetry type=\"");
        switch (type) {
            case TelemetryType::SPEED_KMH: _stream->print("SPEED_KMH"); break;
            case TelemetryType::LOAD_PERCENT: _stream->print("LOAD_PERCENT"); break;
            case TelemetryType::VOLTAGE_TRACK: _stream->print("VOLTAGE_TRACK"); break;
            case TelemetryType::CURRENT_AMP: _stream->print("CURRENT_AMP"); break;
            case TelemetryType::FUEL_LEVEL: _stream->print("FUEL_LEVEL"); break;
            case TelemetryType::TEMP_CELSIUS: _stream->print("TEMP_CELSIUS"); break;
            case TelemetryType::QOS_ERROR_RATE: _stream->print("QOS_ERROR_RATE"); break;
            case TelemetryType::ODOMETER_VALUE: _stream->print("ODOMETER_VALUE"); break;
            case TelemetryType::POSITION_CONFIDENCE: _stream->print("POSITION_CONFIDENCE"); break;
        }
        _stream->print("\" value=\"");
        _stream->print(value);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onLocoExternalStateChange(const LocoHandle& loco, ExternalState state) override {
        _stream->println("    <event type=\"onLocoExternalStateChange\">");
        printLoco(loco);
        _stream->print("        <externalState state=\"");
        switch (state) {
            case ExternalState::FREE_RUN: _stream->print("FREE_RUN"); break;
            case ExternalState::STOPPED_BY_ABC_SIGNAL: _stream->print("STOPPED_BY_ABC_SIGNAL"); break;
            case ExternalState::STOPPED_BY_DC_BRAKE: _stream->print("STOPPED_BY_DC_BRAKE"); break;
            case ExternalState::STOPPED_BY_HLU: _stream->print("STOPPED_BY_HLU"); break;
            case ExternalState::RESTRICTED_SPEED_HLU: _stream->print("RESTRICTED_SPEED_HLU"); break;
        }
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onLocoRailComRawData(const LocoHandle& loco, uint8_t appId, const std::vector<uint8_t>& data) override {
        _stream->println("    <event type=\"onLocoRailComRawData\">");
        printLoco(loco);
        _stream->print("        <railcom appId=\"");
        _stream->print(appId);
        _stream->print("\" data=\"");
        printHex(_stream, data);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onNewLocoDiscovered(const LocoHandle& loco, const std::string& name, const std::string& icon) override {
        _stream->println("    <event type=\"onNewLocoDiscovered\">");
        printLoco(loco);
        _stream->print("        <discovery name=\"");
        escapeXml(name);
        _stream->print("\" icon=\"");
        escapeXml(icon);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onCvRead(const LocoHandle& loco, int cvNumber) override {
        _stream->println("    <event type=\"onCvRead\">");
        printLoco(loco);
        _stream->print("        <cv cvNumber=\"");
        _stream->print(cvNumber);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onCvWrite(const LocoHandle& loco, int cvNumber, uint8_t value) override {
        _stream->println("    <event type=\"onCvWrite\">");
        printLoco(loco);
        _stream->print("        <cv cvNumber=\"");
        _stream->print(cvNumber);
        _stream->print("\" value=\"");
        _stream->print(value);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onCvReadDone(const LocoHandle& loco, int cvNumber, uint8_t value, bool success) override {
        _stream->println("    <event type=\"onCvReadDone\">");
        printLoco(loco);
        _stream->print("        <cv cvNumber=\"");
        _stream->print(cvNumber);
        _stream->print("\" value=\"");
        _stream->print(value);
        _stream->print("\" success=\"");
        _stream->print(success ? "true" : "false");
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onSusiConfigRead(const LocoHandle& loco, uint8_t bankIndex, uint8_t susiIndex, uint8_t value) override {
        _stream->println("    <event type=\"onSusiConfigRead\">");
        printLoco(loco);
        _stream->print("        <susi bank=\"");
        _stream->print(bankIndex);
        _stream->print("\" index=\"");
        _stream->print(susiIndex);
        _stream->print("\" value=\"");
        _stream->print(value);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onConfigBlockLoad(const LocoHandle& loco, std::string domain, const std::vector<uint8_t>& data) override {
        _stream->println("    <event type=\"onConfigBlockLoad\">");
        printLoco(loco);
        _stream->print("        <config domain=\"");
        escapeXml(domain);
        _stream->print("\" data=\"");
        printHex(_stream, data);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onProgressUpdate(std::string operation, float percent) override {
        _stream->println("    <event type=\"onProgressUpdate\">");
        _stream->print("        <progress operation=\"");
        escapeXml(operation);
        _stream->print("\" percent=\"");
        _stream->print(percent);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onLocoEventSync(const LocoHandle& loco, SyncType type, uint32_t value) override {
        _stream->println("    <event type=\"onLocoEventSync\">");
        printLoco(loco);
        _stream->print("        <sync type=\"");
        switch (type) {
            case SyncType::CAM_PULSE: _stream->print("CAM_PULSE"); break;
            case SyncType::CYLINDER_CYCLE: _stream->print("CYLINDER_CYCLE"); break;
            case SyncType::GEAR_CHANGE_UP: _stream->print("GEAR_CHANGE_UP"); break;
            case SyncType::GEAR_CHANGE_DOWN: _stream->print("GEAR_CHANGE_DOWN"); break;
            case SyncType::BRAKE_SQUEAL_START: _stream->print("BRAKE_SQUEAL_START"); break;
            case SyncType::DOOR_MOVEMENT: _stream->print("DOOR_MOVEMENT"); break;
        }
        _stream->print("\" value=\"");
        _stream->print(value);
        _stream->println("\" />");
        _stream->println("    </event>");
    }

private:
    Stream* _stream;

    void printLoco(const LocoHandle& loco) {
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->print("\" mfxUid=\"");
        _stream->print(loco.mfxUid);
        _stream->println("\" />");
    }

    void printProtocol(Protocol protocol) {
        switch (protocol) {
            case Protocol::DCC: _stream->print("DCC"); break;
            case Protocol::MM_I: _stream->print("MM_I"); break;
            case Protocol::MM_II: _stream->print("MM_II"); break;
            case Protocol::MFX: _stream->print("MFX"); break;
            case Protocol::SX: _stream->print("SX"); break;
            case Protocol::SX2: _stream->print("SX2"); break;
            case Protocol::LOCONET: _stream->print("LOCONET"); break;
            case Protocol::BIDIB: _stream->print("BIDIB"); break;
            case Protocol::XPRESSNET: _stream->print("XPRESSNET"); break;
            case Protocol::CAN_GENERIC: _stream->print("CAN_GENERIC"); break;
        }
    }

    void escapeXml(const std::string& str) {
        for (char c : str) {
            switch (c) {
                case '&': _stream->print("&amp;"); break;
                case '<': _stream->print("&lt;"); break;
                case '>': _stream->print("&gt;"); break;
                case '"': _stream->print("&quot;"); break;
                case '\'': _stream->print("&apos;"); break;
                default: _stream->print(c);
            }
        }
    }
};

class XmlParser {
public:
    XmlParser(IUnifiedModelTrainListener& listener) : _listener(&listener) {
        yxml_init(&_xml, _stack, sizeof(_stack));
        reset();
    }

    void parse(const String& xml) {
        for (size_t i = 0; i < xml.length(); i++) {
            yxml_ret_t r = yxml_parse(&_xml, xml[i]);
            if (r == YXML_ATTRSTART) {
                _currentAttr = _xml.attr;
                _currentValue = "";
            } else if (r == YXML_ATTRVAL) {
                _currentValue += _xml.data;
            } else if (r == YXML_ATTREND) {
                processAttribute(_xml.elem, _currentAttr, _currentValue);
            } else if (r == YXML_ELEMEND) {
                if (strcmp(_xml.elem, "event") == 0) {
                    dispatch();
                    reset();
                }
            }
        }
    }

private:
    IUnifiedModelTrainListener* _listener;
    yxml_t _xml;
    uint8_t _stack[1024];

    String _eventType;
    LocoHandle _loco;
    LocoHandle _master;
    LocoHandle _slave;
    String _currentAttr;
    String _currentValue;

    float _speedPercent;
    Direction _direction;
    int _speedSteps;
    int _fIndex;
    bool _isActive;
    uint8_t _value8;
    bool _isAcquired;
    std::string _ownerId;
    ConsistType _consistType;
    bool _inverted;
    uint16_t _address;
    bool _isThrown;
    bool _isFeedback;
    uint8_t _aspectId;
    float _valueFloat;
    uint8_t _errorId;
    std::string _errorMsg;
    uint32_t _sensorId;
    PowerState _powerState;
    int64_t _modelTime;
    float _factor;
    std::string _nodeUid;
    std::string _productName;
    bool _booster;
    std::string _source;
    std::string _message;
    DecoderOrientation _orientation;
    TelemetryType _telemetryType;
    ExternalState _externalState;
    uint8_t _appId;
    std::vector<uint8_t> _data;
    std::string _name;
    std::string _icon;
    int _cvNumber;
    bool _success;
    uint8_t _bankIndex;
    uint8_t _susiIndex;
    std::string _domain;
    std::string _operation;
    float _percent;
    SyncType _syncType;
    uint32_t _syncValue;

    void reset() {
        _eventType = "";
        _loco = {0, Protocol::DCC, 0};
        _master = {0, Protocol::DCC, 0};
        _slave = {0, Protocol::DCC, 0};
        _currentAttr = "";
        _currentValue = "";
        _speedPercent = 0;
        _direction = Direction::FORWARD;
        _speedSteps = 128;
        _fIndex = 0;
        _isActive = false;
        _value8 = 0;
        _isAcquired = false;
        _ownerId = "";
        _consistType = ConsistType::ADVANCED_DCC;
        _inverted = false;
        _address = 0;
        _isThrown = false;
        _isFeedback = false;
        _aspectId = 0;
        _valueFloat = 0;
        _errorId = 0;
        _errorMsg = "";
        _sensorId = 0;
        _powerState = PowerState::OFF;
        _modelTime = 0;
        _factor = 1.0;
        _nodeUid = "";
        _productName = "";
        _booster = false;
        _source = "";
        _message = "";
        _orientation = DecoderOrientation::UNKNOWN;
        _telemetryType = TelemetryType::SPEED_KMH;
        _externalState = ExternalState::FREE_RUN;
        _appId = 0;
        _data.clear();
        _name = "";
        _icon = "";
        _cvNumber = 0;
        _success = false;
        _bankIndex = 0;
        _susiIndex = 0;
        _domain = "";
        _operation = "";
        _percent = 0;
        _syncType = SyncType::CAM_PULSE;
        _syncValue = 0;
    }

    void processAttribute(const char* elem, const String& attr, const String& val) {
        if (strcmp(elem, "event") == 0 && attr == "type") {
            _eventType = val;
        }
        else if (strcmp(elem, "loco") == 0) {
            parseLocoAttr(_loco, attr, val);
        } else if (strcmp(elem, "master") == 0) {
            parseLocoAttr(_master, attr, val);
        } else if (strcmp(elem, "slave") == 0) {
            parseLocoAttr(_slave, attr, val);
        }
        else if (strcmp(elem, "speed") == 0) {
            if (attr == "speedPercent") _speedPercent = val.toFloat();
            else if (attr == "direction") _direction = parseDirection(val);
            else if (attr == "speedSteps") _speedSteps = val.toInt();
        } else if (strcmp(elem, "function") == 0) {
            if (attr == "fIndex") _fIndex = val.toInt();
            else if (attr == "isActive") _isActive = (val == "true");
            else if (attr == "value") _value8 = val.toInt();
        } else if (strcmp(elem, "dispatch") == 0) {
            if (attr == "isAcquired") _isAcquired = (val == "true");
            else if (attr == "ownerId") _ownerId = val.c_str();
        } else if (strcmp(elem, "consist") == 0) {
            if (attr == "type") _consistType = parseConsistType(val);
            else if (attr == "inverted") _inverted = (val == "true");
        } else if (strcmp(elem, "turnout") == 0) {
            if (attr == "address") _address = val.toInt();
            else if (attr == "isThrown") _isThrown = (val == "true");
            else if (attr == "isFeedback") _isFeedback = (val == "true");
        } else if (strcmp(elem, "signal") == 0) {
            if (attr == "address") _address = val.toInt();
            else if (attr == "aspectId") _aspectId = val.toInt();
            else if (attr == "isFeedback") _isFeedback = (val == "true");
        } else if (strcmp(elem, "accessory") == 0) {
            if (attr == "address") _address = val.toInt();
            else if (attr == "value0to1") _valueFloat = val.toFloat();
            else if (attr == "errorId") _errorId = val.toInt();
            else if (attr == "errorMsg") _errorMsg = val.c_str();
        } else if (strcmp(elem, "sensor") == 0) {
            if (attr == "sensorId") _sensorId = (uint32_t)strtoul(val.c_str(), NULL, 10);
            else if (attr == "isActive") _isActive = (val == "true");
        } else if (strcmp(elem, "power") == 0) {
            if (attr == "state") _powerState = parsePowerState(val);
        } else if (strcmp(elem, "clock") == 0) {
            if (attr == "modelTimeUnix") _modelTime = (int64_t)strtoll(val.c_str(), NULL, 10);
            else if (attr == "factor") _factor = val.toFloat();
        } else if (strcmp(elem, "node") == 0) {
            if (attr == "nodeUid") _nodeUid = val.c_str();
            else if (attr == "productName") _productName = val.c_str();
            else if (attr == "booster") _booster = (val == "true");
        } else if (strcmp(elem, "message") == 0) {
            if (attr == "source") _source = val.c_str();
            else if (attr == "message") _message = val.c_str();
        } else if (strcmp(elem, "detection") == 0) {
            if (attr == "sensorId") _sensorId = (uint32_t)strtoul(val.c_str(), NULL, 10);
            else if (attr == "orientation") _orientation = parseOrientation(val);
        } else if (strcmp(elem, "telemetry") == 0) {
            if (attr == "type") _telemetryType = parseTelemetryType(val);
            else if (attr == "value") _valueFloat = val.toFloat();
        } else if (strcmp(elem, "externalState") == 0) {
            if (attr == "state") _externalState = parseExternalState(val);
        } else if (strcmp(elem, "railcom") == 0) {
            if (attr == "appId") _appId = val.toInt();
            else if (attr == "data") _data = hexToBytes(val.c_str());
        } else if (strcmp(elem, "discovery") == 0) {
            if (attr == "name") _name = val.c_str();
            else if (attr == "icon") _icon = val.c_str();
        } else if (strcmp(elem, "cv") == 0) {
            if (attr == "cvNumber") _cvNumber = val.toInt();
            else if (attr == "value") _value8 = val.toInt();
            else if (attr == "success") _success = (val == "true");
        } else if (strcmp(elem, "susi") == 0) {
            if (attr == "bank") _bankIndex = val.toInt();
            else if (attr == "index") _susiIndex = val.toInt();
            else if (attr == "value") _value8 = val.toInt();
        } else if (strcmp(elem, "config") == 0) {
            if (attr == "domain") _domain = val.c_str();
            else if (attr == "data") _data = hexToBytes(val.c_str());
        } else if (strcmp(elem, "progress") == 0) {
            if (attr == "operation") _operation = val.c_str();
            else if (attr == "percent") _percent = val.toFloat();
        } else if (strcmp(elem, "sync") == 0) {
            if (attr == "type") _syncType = parseSyncType(val);
            else if (attr == "value") _syncValue = (uint32_t)strtoul(val.c_str(), NULL, 10);
        }
    }

    void parseLocoAttr(LocoHandle& loco, const String& attr, const String& val) {
        if (attr == "address") loco.address = val.toInt();
        else if (attr == "protocol") loco.protocol = parseProtocol(val);
        else if (attr == "mfxUid") loco.mfxUid = (uint32_t)strtoul(val.c_str(), NULL, 10);
    }

    void dispatch() {
        if (_eventType == "onLocoSpeedChange") _listener->onLocoSpeedChange(_loco, _speedPercent, _direction, _speedSteps);
        else if (_eventType == "onLocoFunctionChange") _listener->onLocoFunctionChange(_loco, _fIndex, _isActive);
        else if (_eventType == "onLocoFunctionAnalogChange") _listener->onLocoFunctionAnalogChange(_loco, _fIndex, _value8);
        else if (_eventType == "onLocoDispatchStateChange") _listener->onLocoDispatchStateChange(_loco, _isAcquired, _ownerId);
        else if (_eventType == "onConsistLink") _listener->onConsistLink(_master, _slave, _consistType, _inverted);
        else if (_eventType == "onConsistUnlink") _listener->onConsistUnlink(_slave);
        else if (_eventType == "onTurnoutChange") _listener->onTurnoutChange(_address, _isThrown, _isFeedback);
        else if (_eventType == "onSignalAspectChange") _listener->onSignalAspectChange(_address, _aspectId, _isFeedback);
        else if (_eventType == "onAccessoryAnalogValue") _listener->onAccessoryAnalogValue(_address, _valueFloat);
        else if (_eventType == "onAccessoryError") _listener->onAccessoryError(_address, _errorId, _errorMsg);
        else if (_eventType == "onSensorStateChange") _listener->onSensorStateChange(_sensorId, _isActive);
        else if (_eventType == "onTrackPowerChange") _listener->onTrackPowerChange(_powerState);
        else if (_eventType == "onFastClockUpdated") _listener->onFastClockUpdated(_modelTime, _factor);
        else if (_eventType == "onHardwareNodeAttached") _listener->onHardwareNodeAttached(_nodeUid, _productName, _booster);
        else if (_eventType == "onHardwareNodeLost") _listener->onHardwareNodeLost(_nodeUid);
        else if (_eventType == "onSystemMessage") _listener->onSystemMessage(_source, _message);
        else if (_eventType == "onLocoDetectedOnBlock") _listener->onLocoDetectedOnBlock(_sensorId, _loco, _orientation);
        else if (_eventType == "onLocoTelemetryData") _listener->onLocoTelemetryData(_loco, _telemetryType, _valueFloat);
        else if (_eventType == "onLocoExternalStateChange") _listener->onLocoExternalStateChange(_loco, _externalState);
        else if (_eventType == "onLocoRailComRawData") _listener->onLocoRailComRawData(_loco, _appId, _data);
        else if (_eventType == "onNewLocoDiscovered") _listener->onNewLocoDiscovered(_loco, _name, _icon);
        else if (_eventType == "onCvRead") _listener->onCvRead(_loco, _cvNumber);
        else if (_eventType == "onCvWrite") _listener->onCvWrite(_loco, _cvNumber, _value8);
        else if (_eventType == "onCvReadDone") _listener->onCvReadDone(_loco, _cvNumber, _value8, _success);
        else if (_eventType == "onSusiConfigRead") _listener->onSusiConfigRead(_loco, _bankIndex, _susiIndex, _value8);
        else if (_eventType == "onConfigBlockLoad") _listener->onConfigBlockLoad(_loco, _domain, _data);
        else if (_eventType == "onProgressUpdate") _listener->onProgressUpdate(_operation, _percent);
        else if (_eventType == "onLocoEventSync") _listener->onLocoEventSync(_loco, _syncType, _syncValue);
    }

    Direction parseDirection(const String& str) {
        if (str == "REVERSE") return Direction::REVERSE;
        if (str == "FORWARD") return Direction::FORWARD;
        return Direction::UNKNOWN;
    }

    Protocol parseProtocol(const String& str) {
        if (str == "DCC") return Protocol::DCC;
        if (str == "MM_I") return Protocol::MM_I;
        if (str == "MM_II") return Protocol::MM_II;
        if (str == "MFX") return Protocol::MFX;
        if (str == "SX") return Protocol::SX;
        if (str == "SX2") return Protocol::SX2;
        if (str == "LOCONET") return Protocol::LOCONET;
        if (str == "BIDIB") return Protocol::BIDIB;
        if (str == "XPRESSNET") return Protocol::XPRESSNET;
        if (str == "CAN_GENERIC") return Protocol::CAN_GENERIC;
        return Protocol::DCC;
    }

    ConsistType parseConsistType(const String& str) {
        if (str == "ADVANCED_DCC") return ConsistType::ADVANCED_DCC;
        if (str == "UNIVERSAL_HOST") return ConsistType::UNIVERSAL_HOST;
        if (str == "MU_LOCONET") return ConsistType::MU_LOCONET;
        return ConsistType::ADVANCED_DCC;
    }

    PowerState parsePowerState(const String& str) {
        if (str == "OFF") return PowerState::OFF;
        if (str == "ON") return PowerState::ON;
        if (str == "EMERGENCY_STOP") return PowerState::EMERGENCY_STOP;
        if (str == "SHORT_CIRCUIT") return PowerState::SHORT_CIRCUIT;
        return PowerState::OFF;
    }

    DecoderOrientation parseOrientation(const String& str) {
        if (str == "NORMAL") return DecoderOrientation::NORMAL;
        if (str == "INVERTED") return DecoderOrientation::INVERTED;
        return DecoderOrientation::UNKNOWN;
    }

    TelemetryType parseTelemetryType(const String& str) {
        if (str == "SPEED_KMH") return TelemetryType::SPEED_KMH;
        if (str == "LOAD_PERCENT") return TelemetryType::LOAD_PERCENT;
        if (str == "VOLTAGE_TRACK") return TelemetryType::VOLTAGE_TRACK;
        if (str == "CURRENT_AMP") return TelemetryType::CURRENT_AMP;
        if (str == "FUEL_LEVEL") return TelemetryType::FUEL_LEVEL;
        if (str == "TEMP_CELSIUS") return TelemetryType::TEMP_CELSIUS;
        if (str == "QOS_ERROR_RATE") return TelemetryType::QOS_ERROR_RATE;
        if (str == "ODOMETER_VALUE") return TelemetryType::ODOMETER_VALUE;
        if (str == "POSITION_CONFIDENCE") return TelemetryType::POSITION_CONFIDENCE;
        return TelemetryType::SPEED_KMH;
    }

    ExternalState parseExternalState(const String& str) {
        if (str == "FREE_RUN") return ExternalState::FREE_RUN;
        if (str == "STOPPED_BY_ABC_SIGNAL") return ExternalState::STOPPED_BY_ABC_SIGNAL;
        if (str == "STOPPED_BY_DC_BRAKE") return ExternalState::STOPPED_BY_DC_BRAKE;
        if (str == "STOPPED_BY_HLU") return ExternalState::STOPPED_BY_HLU;
        if (str == "RESTRICTED_SPEED_HLU") return ExternalState::RESTRICTED_SPEED_HLU;
        return ExternalState::FREE_RUN;
    }

    SyncType parseSyncType(const String& str) {
        if (str == "CAM_PULSE") return SyncType::CAM_PULSE;
        if (str == "CYLINDER_CYCLE") return SyncType::CYLINDER_CYCLE;
        if (str == "GEAR_CHANGE_UP") return SyncType::GEAR_CHANGE_UP;
        if (str == "GEAR_CHANGE_DOWN") return SyncType::GEAR_CHANGE_DOWN;
        if (str == "BRAKE_SQUEAL_START") return SyncType::BRAKE_SQUEAL_START;
        if (str == "DOOR_MOVEMENT") return SyncType::DOOR_MOVEMENT;
        return SyncType::CAM_PULSE;
    }
};

class CmdLineParser {
public:
    CmdLineParser(IUnifiedModelTrainListener& listener) : _listener(&listener) {}

    String getParamValue(const String& params, const char* key) {
        String searchKey = String(key) + "=\"";
        int keyIndex = params.indexOf(searchKey);
        if (keyIndex == -1) {
            return "";
        }
        int valueStartIndex = keyIndex + searchKey.length();
        int valueEndIndex = params.indexOf('"', valueStartIndex);
        if (valueEndIndex == -1) {
            return "";
        }
        return params.substring(valueStartIndex, valueEndIndex);
    }

    void parse(const String& command) {
        if (command.length() == 0) {
            return;
        }

        // Try parsing extended format first
        int firstSpace = command.indexOf(' ');
        String cmd;
        String params;
        if (firstSpace != -1) {
            cmd = command.substring(0, firstSpace);
            params = command.substring(firstSpace + 1);
        } else {
            cmd = command;
        }

        if (cmd == "POWER") {
            String state = getParamValue(params, "state");
            if (state == "ON") {
                _listener->onTrackPowerChange(PowerState::ON);
            } else if (state == "OFF") {
                _listener->onTrackPowerChange(PowerState::OFF);
            }
            return;
        } else if (cmd == "THROTTLE") {
            uint16_t cab = getParamValue(params, "cab").toInt();
            int speed = getParamValue(params, "speed").toInt();
            int dir = getParamValue(params, "direction").toInt();
            int steps = getParamValue(params, "steps").toInt();
            if (steps == 0) steps = 128; // default

            LocoHandle loco = {cab, Protocol::DCC, 0};
            _listener->onLocoSpeedChange(loco, speed, (Direction)dir, steps);
            return;
        } else if (cmd == "FUNCTION") {
            uint16_t cab = getParamValue(params, "cab").toInt();
            int func = getParamValue(params, "function").toInt();
            int state = getParamValue(params, "state").toInt();

            LocoHandle loco = {cab, Protocol::DCC, 0};
            _listener->onLocoFunctionChange(loco, func, state);
            return;
        } else if (cmd == "TURNOUT") {
            uint16_t addr = getParamValue(params, "id").toInt();
            int state = getParamValue(params, "state").toInt();
            _listener->onTurnoutChange(addr, state, false);
            return;
        } else if (cmd == "ACCESSORY" || cmd == "SIGNAL") { // Support both
            uint16_t addr = getParamValue(params, "id").toInt();
            int state = getParamValue(params, "state").toInt(); // aspectId
            _listener->onSignalAspectChange(addr, state, false);
            return;
        } else if (cmd == "MECHANICAL_SYNC_EVENT" || cmd == "MECH_SYNC") {
            uint16_t cab = getParamValue(params, "cab").toInt();
            String typeStr = getParamValue(params, "type");
            uint8_t value = getParamValue(params, "value").toInt();
            SyncType type = SyncType::CAM_PULSE;
            if (typeStr == "CAM_PULSE") type = SyncType::CAM_PULSE;
            else if (typeStr == "CYLINDER_CYCLE") type = SyncType::CYLINDER_CYCLE;
            else if (typeStr == "GEAR_CHANGE_UP") type = SyncType::GEAR_CHANGE_UP;
            else if (typeStr == "GEAR_CHANGE_DOWN") type = SyncType::GEAR_CHANGE_DOWN;
            else if (typeStr == "BRAKE_SQUEAL_START") type = SyncType::BRAKE_SQUEAL_START;
            else if (typeStr == "DOOR_MOVEMENT") type = SyncType::DOOR_MOVEMENT;

            LocoHandle loco = {cab, Protocol::DCC, 0};
            _listener->onLocoEventSync(loco, type, value);
            return;
        }

        // New Commands Support
        else if (cmd == "DISPATCH") {
             uint16_t cab = getParamValue(params, "cab").toInt();
             String state = getParamValue(params, "state");
             String owner = getParamValue(params, "owner");
             LocoHandle loco = {cab, Protocol::DCC, 0};
             _listener->onLocoDispatchStateChange(loco, (state == "ACQUIRED"), owner.c_str());
        }
        else if (cmd == "CONSIST_LINK") {
            uint16_t master = getParamValue(params, "master").toInt();
            uint16_t slave = getParamValue(params, "slave").toInt();
            String typeStr = getParamValue(params, "type");
            bool inverted = (getParamValue(params, "inverted") == "true");
            ConsistType type = ConsistType::ADVANCED_DCC;
            if (typeStr == "UNIVERSAL") type = ConsistType::UNIVERSAL_HOST;
            else if (typeStr == "LOCONET") type = ConsistType::MU_LOCONET;

            LocoHandle m = {master, Protocol::DCC, 0};
            LocoHandle s = {slave, Protocol::DCC, 0};
            _listener->onConsistLink(m, s, type, inverted);
        }
        else if (cmd == "CONSIST_UNLINK") {
            uint16_t slave = getParamValue(params, "slave").toInt();
            LocoHandle s = {slave, Protocol::DCC, 0};
            _listener->onConsistUnlink(s);
        }
        else if (cmd == "ACCESSORY_ANALOG") {
            uint16_t addr = getParamValue(params, "address").toInt();
            float val = getParamValue(params, "value").toFloat();
            _listener->onAccessoryAnalogValue(addr, val);
        }
        else if (cmd == "ACCESSORY_ERROR") {
             uint16_t addr = getParamValue(params, "address").toInt();
             int err = getParamValue(params, "errorId").toInt();
             String msg = getParamValue(params, "errorMessage");
             _listener->onAccessoryError(addr, err, msg.c_str());
        }
        else if (cmd == "SENSOR") {
            uint32_t id = getParamValue(params, "id").toInt();
            String state = getParamValue(params, "state");
            _listener->onSensorStateChange(id, (state == "ACTIVE"));
        }
        else if (cmd == "FAST_CLOCK") {
            long time = getParamValue(params, "time").toInt();
            float factor = getParamValue(params, "factor").toFloat();
            _listener->onFastClockUpdated(time, factor);
        }
        else if (cmd == "HARDWARE_NODE_ATTACHED") {
            String uid = getParamValue(params, "uid");
            String prod = getParamValue(params, "productName");
            bool boost = (getParamValue(params, "booster") == "true");
            _listener->onHardwareNodeAttached(uid.c_str(), prod.c_str(), boost);
        }
        else if (cmd == "HARDWARE_NODE_LOST") {
             String uid = getParamValue(params, "uid");
             _listener->onHardwareNodeLost(uid.c_str());
        }
        else if (cmd == "SYSTEM_MESSAGE") {
             String src = getParamValue(params, "source");
             String msg = getParamValue(params, "message");
             _listener->onSystemMessage(src.c_str(), msg.c_str());
        }
        else if (cmd == "LOCO_DETECTED_ON_BLOCK") {
            uint32_t sid = getParamValue(params, "sensorId").toInt();
            uint16_t cab = getParamValue(params, "cab").toInt();
            String orientStr = getParamValue(params, "orientation");
            DecoderOrientation orient = DecoderOrientation::UNKNOWN;
            if (orientStr == "NORMAL") orient = DecoderOrientation::NORMAL;
            else if (orientStr == "INVERTED") orient = DecoderOrientation::INVERTED;

            LocoHandle loco = {cab, Protocol::DCC, 0};
            _listener->onLocoDetectedOnBlock(sid, loco, orient);
        }
        else if (cmd == "LOCO_TELEMETRY") {
             uint16_t cab = getParamValue(params, "cab").toInt();
             String typeStr = getParamValue(params, "type");
             float val = getParamValue(params, "value").toFloat();
             TelemetryType type = TelemetryType::SPEED_KMH;
             // Simply mapping SPEED for now as in printer
             if (typeStr == "SPEED") type = TelemetryType::SPEED_KMH;

             LocoHandle loco = {cab, Protocol::DCC, 0};
             _listener->onLocoTelemetryData(loco, type, val);
        }

        // If not an extended command, try parsing legacy format
        char cmdChar = command.charAt(0);
        params = command.substring(1);
        params.trim();

        switch (cmdChar) {
            case '1':
                _listener->onTrackPowerChange(PowerState::ON);
                break;
            case '0':
                _listener->onTrackPowerChange(PowerState::OFF);
                break;
            case 't':
                {
                    int firstSpace = params.indexOf(' ');
                    int secondSpace = params.indexOf(' ', firstSpace + 1);
                    if (firstSpace != -1 && secondSpace != -1) {
                        uint16_t cab = params.substring(0, firstSpace).toInt();
                        int speed = params.substring(firstSpace + 1, secondSpace).toInt();
                        int dir = params.substring(secondSpace + 1).toInt();

                        LocoHandle loco = {cab, Protocol::DCC, 0};
                        _listener->onLocoSpeedChange(loco, speed, (Direction)dir, 128);
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

                        LocoHandle loco = {cab, Protocol::DCC, 0};
                        _listener->onLocoFunctionChange(loco, func, state);
                    }
                }
                break;
            case 'T':
                {
                    int firstSpace = params.indexOf(' ');
                    if (firstSpace != -1) {
                        uint16_t addr = params.substring(0, firstSpace).toInt();
                        int state = params.substring(firstSpace + 1).toInt();
                        _listener->onTurnoutChange(addr, state, false);
                    }
                }
                break;
            case 'S':
                {
                    int firstSpace = params.indexOf(' ');
                    if (firstSpace != -1) {
                        uint16_t addr = params.substring(0, firstSpace).toInt();
                        int aspect = params.substring(firstSpace + 1).toInt();
                        _listener->onSignalAspectChange(addr, aspect, false);
                    }
                }
                break;
            case 'm':
                {
                    int firstSpace = params.indexOf(' ');
                    int secondSpace = params.indexOf(' ', firstSpace + 1);
                    if (firstSpace != -1 && secondSpace != -1) {
                        uint16_t cab = params.substring(0, firstSpace).toInt();
                        int type = params.substring(firstSpace + 1, secondSpace).toInt();
                        int value = params.substring(secondSpace + 1).toInt();

                        LocoHandle loco = {cab, Protocol::DCC, 0};
                        _listener->onLocoEventSync(loco, (SyncType)type, value);
                    }
                }
                break;
        }
    }

private:
    IUnifiedModelTrainListener* _listener;
};

}
