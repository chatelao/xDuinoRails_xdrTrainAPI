#pragma once

#include "xDuinoRails_xTrainAPI.h"
#include <yxml.h>
#include <Arduino.h>

#ifndef USE_EXTENDED_CLI_SYNTAX
#define USE_EXTENDED_CLI_SYNTAX 1
#endif

namespace ModelRail {

class CmdLinePrinter : public IUnifiedModelTrainListener {
public:
    CmdLinePrinter(Stream& stream) : _stream(&stream) {}

    uint64_t getImplementedApi() const override { return (uint64_t)ApiFeature::ALL_FEATURES; }

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
        _stream->print("<SIGNAL id=\"");
        _stream->print(address);
        _stream->print("\" aspect=\"");
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
        _stream->print("onLocoFunctionAnalogChange cab=");
        _stream->print(loco.address);
        _stream->print(" fIndex=");
        _stream->print(fIndex);
        _stream->print(" value=");
        _stream->println(value);
    }
    void onLocoDispatchStateChange(const LocoHandle& loco, bool isAcquired, std::string ownerId) override {
        _stream->print("onLocoDispatchStateChange cab=");
        _stream->print(loco.address);
        _stream->print(" isAcquired=");
        _stream->println(isAcquired);
    }
    void onConsistLink(const LocoHandle& master, const LocoHandle& slave, ConsistType type, bool inverted) override {
        _stream->println("onConsistLink");
    }
    void onConsistUnlink(const LocoHandle& slave) override {
        _stream->println("onConsistUnlink");
    }
    void onAccessoryAnalogValue(uint16_t address, float value0to1) override {
        _stream->print("onAccessoryAnalogValue addr=");
        _stream->print(address);
        _stream->print(" value=");
        _stream->println(value0to1);
    }
    void onAccessoryError(uint16_t address, uint8_t errorId, std::string errorMsg) override {
        _stream->print("onAccessoryError addr=");
        _stream->println(address);
    }
    void onSensorStateChange(uint32_t sensorId, bool isActive) override {
        _stream->print("onSensorStateChange id=");
        _stream->print(sensorId);
        _stream->print(" isActive=");
        _stream->println(isActive);
    }
    void onFastClockUpdated(int64_t modelTimeUnix, float factor) override {
        _stream->println("onFastClockUpdated");
    }
    void onHardwareNodeAttached(std::string nodeUid, std::string productName, bool booster) override {
        _stream->println("onHardwareNodeAttached");
    }
    void onHardwareNodeLost(std::string nodeUid) override {
        _stream->println("onHardwareNodeLost");
    }
    void onSystemMessage(std::string source, std::string message) override {
        _stream->println("onSystemMessage");
    }
    void onLocoDetectedOnBlock(uint32_t sensorId, const LocoHandle& loco, DecoderOrientation orientation) override {
        _stream->print("onLocoDetectedOnBlock id=");
        _stream->println(sensorId);
    }
    void onLocoTelemetryData(const LocoHandle& loco, TelemetryType type, float value) override {
        _stream->println("onLocoTelemetryData");
    }
    void onLocoExternalStateChange(const LocoHandle& loco, ExternalState state) override {
        _stream->println("onLocoExternalStateChange");
    }
    void onLocoRailComRawData(const LocoHandle& loco, uint8_t appId, const std::vector<uint8_t>& data) override {
        _stream->println("onLocoRailComRawData");
    }
    void onNewLocoDiscovered(const LocoHandle& loco, const std::string& name, const std::string& icon) override {
        _stream->println("onNewLocoDiscovered");
    }
    void onCvRead(const LocoHandle& loco, int cvNumber) override {
        _stream->print("onCvRead cab=");
        _stream->print(loco.address);
        _stream->print(" cv=");
        _stream->println(cvNumber);
    }
    void onCvWrite(const LocoHandle& loco, int cvNumber, uint8_t value) override {
        _stream->print("onCvWrite cab=");
        _stream->print(loco.address);
        _stream->print(" cv=");
        _stream->print(cvNumber);
        _stream->print(" value=");
        _stream->println(value);
    }
    void onCvReadDone(const LocoHandle& loco, int cvNumber, uint8_t value, bool success) override {
        _stream->println("onCvReadDone");
    }
    void onSusiConfigRead(const LocoHandle& loco, uint8_t bankIndex, uint8_t susiIndex, uint8_t value) override {
        _stream->println("onSusiConfigRead");
    }
    void onConfigBlockLoad(const LocoHandle& loco, std::string domain, const std::vector<uint8_t>& data) override {
        _stream->println("onConfigBlockLoad");
    }
    void onProgressUpdate(std::string operation, float percent) override {
        _stream->println("onProgressUpdate");
    }

    // GROUP F: REAL-TIME SYNCHRONIZATION (Added in v2.3)
    void onLocoEventSync(const LocoHandle& loco, SyncType type, uint32_t value) override {
#if USE_EXTENDED_CLI_SYNTAX
        _stream->print("<MECH_SYNC cab=\"");
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

    uint64_t getImplementedApi() const override { return (uint64_t)ApiFeature::ALL_FEATURES; }

    // Helper to start the XML document
    void begin() {
        _stream->println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        _stream->println("<xTrainEvents>");
    }

    // Helper to end the XML document
    void end() {
        _stream->println("</xTrainEvents>");
    }

    void onLocoSpeedChange(const LocoHandle& loco, float speedPercent, Direction direction, int speedSteps) override {
        _stream->println("    <event type=\"onLocoSpeedChange\">");
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->print("\" mfxUid=\"");
        _stream->print(loco.mfxUid);
        _stream->println("\" />");
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
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->println("\" />");
        _stream->print("        <function fIndex=\"");
        _stream->print(fIndex);
        _stream->print("\" isActive=\"");
        _stream->print(isActive ? "true" : "false");
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onLocoFunctionAnalogChange(const LocoHandle& loco, int fIndex, uint8_t value) override {
        _stream->println("    <event type=\"onLocoFunctionAnalogChange\">");
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->println("\" />");
        _stream->print("        <function fIndex=\"");
        _stream->print(fIndex);
        _stream->print("\" value=\"");
        _stream->print(value);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onLocoDispatchStateChange(const LocoHandle& loco, bool isAcquired, std::string ownerId) override {
        _stream->println("    <event type=\"onLocoDispatchStateChange\">");
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->println("\" />");
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
        _stream->print(modelTimeUnix);
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
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->println("\" />");
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
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->println("\" />");
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
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->println("\" />");
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
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->println("\" />");
        _stream->print("        <railcom appId=\"");
        _stream->print(appId);
        _stream->print("\" data=\"");
        for (uint8_t byte : data) {
            if (byte < 16) _stream->print('0');
            _stream->print(byte, HEX);
        }
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onNewLocoDiscovered(const LocoHandle& loco, const std::string& name, const std::string& icon) override {
        _stream->println("    <event type=\"onNewLocoDiscovered\">");
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->println("\" />");
        _stream->print("        <discovery name=\"");
        escapeXml(name);
        _stream->print("\" icon=\"");
        escapeXml(icon);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onCvRead(const LocoHandle& loco, int cvNumber) override {
        _stream->println("    <event type=\"onCvRead\">");
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->println("\" />");
        _stream->print("        <cv cvNumber=\"");
        _stream->print(cvNumber);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onCvWrite(const LocoHandle& loco, int cvNumber, uint8_t value) override {
        _stream->println("    <event type=\"onCvWrite\">");
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->println("\" />");
        _stream->print("        <cv cvNumber=\"");
        _stream->print(cvNumber);
        _stream->print("\" value=\"");
        _stream->print(value);
        _stream->println("\" />");
        _stream->println("    </event>");
    }
    void onCvReadDone(const LocoHandle& loco, int cvNumber, uint8_t value, bool success) override {
        _stream->println("    <event type=\"onCvReadDone\">");
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->println("\" />");
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
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->println("\" />");
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
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->println("\" />");
        _stream->print("        <config domain=\"");
        escapeXml(domain);
        _stream->print("\" data=\"");
        for (uint8_t byte : data) {
            if (byte < 16) _stream->print('0');
            _stream->print(byte, HEX);
        }
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
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"");
        printProtocol(loco.protocol);
        _stream->print("\" mfxUid=\"");
        _stream->print(loco.mfxUid);
        _stream->println("\" />");
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

    void printProtocol(Protocol protocol) {
        switch (protocol) {
            case Protocol::DCC: _stream->print("DCC"); break;
            case Protocol::MM_I: _stream->print("MM_I"); break;
            case Protocol::MM_II: _stream->print("MM_II"); break;
            case Protocol::MFX: _stream->print("MFX"); break;
            case Protocol::SELECTRIX: _stream->print("SELECTRIX"); break;
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
    }

    void parse(const String& xml) {
        LocoHandle loco;
        float speedPercent;
        Direction direction;
        int speedSteps;

        for (size_t i = 0; i < xml.length(); i++) {
            yxml_ret_t r = yxml_parse(&_xml, xml[i]);
            if (r == YXML_ELEMSTART && strcmp(_xml.elem, "loco") == 0) {
                // do nothing
            } else if (r == YXML_ATTRSTART && strcmp(_xml.attr, "address") == 0) {
                // do nothing
            } else if (r == YXML_ATTRVAL) {
                if (strcmp(_xml.elem, "loco") == 0 && strcmp(_xml.attr, "address") == 0) {
                    loco.address = atoi(_xml.data);
                } else if (strcmp(_xml.elem, "speed") == 0 && strcmp(_xml.attr, "speedPercent") == 0) {
                    speedPercent = atof(_xml.data);
                } else if (strcmp(_xml.elem, "speed") == 0 && strcmp(_xml.attr, "direction") == 0) {
                    direction = strcmp(_xml.data, "FORWARD") == 0 ? Direction::FORWARD : Direction::REVERSE;
                } else if (strcmp(_xml.elem, "speed") == 0 && strcmp(_xml.attr, "speedSteps") == 0) {
                    speedSteps = atoi(_xml.data);
                }
            } else if (r == YXML_ELEMEND && strcmp(_xml.elem, "event") == 0) {
                _listener->onLocoSpeedChange(loco, speedPercent, direction, speedSteps);
            }
        }
        yxml_eof(&_xml);
    }

private:
    IUnifiedModelTrainListener* _listener;
    yxml_t _xml;
    uint8_t _stack[4096];
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
        } else if (cmd == "SIGNAL") {
            uint16_t addr = getParamValue(params, "id").toInt();
            int aspect = getParamValue(params, "aspect").toInt();
            _listener->onSignalAspectChange(addr, aspect, false);
            return;
        } else if (cmd == "MECH_SYNC") {
            uint16_t cab = getParamValue(params, "cab").toInt();
            String typeStr = getParamValue(params, "type");
            uint8_t value = getParamValue(params, "value").toInt();
            SyncType type;
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
