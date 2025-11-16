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

    // Implement other pure virtual functions from the interface
    void onLocoFunctionAnalogChange(const LocoHandle& loco, int fIndex, uint8_t value) override {}
    void onLocoDispatchStateChange(const LocoHandle& loco, bool isAcquired, std::string ownerId) override {}
    void onConsistLink(const LocoHandle& master, const LocoHandle& slave, ConsistType type, bool inverted) override {}
    void onConsistUnlink(const LocoHandle& slave) override {}
    void onAccessoryAnalogValue(uint16_t address, float value0to1) override {}
    void onAccessoryError(uint16_t address, uint8_t errorId, std::string errorMsg) override {}
    void onSensorStateChange(uint32_t sensorId, bool isActive) override {}
    void onFastClockUpdated(int64_t modelTimeUnix, float factor) override {}
    void onHardwareNodeAttached(std::string nodeUid, std::string productName, bool booster) override {}
    void onHardwareNodeLost(std::string nodeUid) override {}
    void onSystemMessage(std::string source, std::string message) override {}
    void onLocoDetectedOnBlock(uint32_t sensorId, const LocoHandle& loco, DecoderOrientation orientation) override {}
    void onLocoTelemetryData(const LocoHandle& loco, TelemetryType type, float value) override {}
    void onLocoExternalStateChange(const LocoHandle& loco, ExternalState state) override {}
    void onLocoRailComRawData(const LocoHandle& loco, uint8_t appId, const std::vector<uint8_t>& data) override {}
    void onNewLocoDiscovered(const LocoHandle& loco, const std::string& name, const std::string& icon) override {}
    void onCvReadResult(const LocoHandle& loco, int cvNumber, uint8_t value, bool success) override {}
    void onSusiConfigRead(const LocoHandle& loco, uint8_t bankIndex, uint8_t susiIndex, uint8_t value) override {}
    void onConfigBlockLoad(const LocoHandle& loco, std::string domain, const std::vector<uint8_t>& data) override {}
    void onProgressUpdate(std::string operation, float percent) override {}

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

    void onLocoSpeedChange(const LocoHandle& loco, float speedPercent, Direction direction, int speedSteps) override {
        _stream->println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        _stream->println("<xTrainEvents>");
        _stream->println("    <event type=\"onLocoSpeedChange\">");
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"DCC\" mfxUid=\"");
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
        _stream->println("</xTrainEvents>");
    }

    // Implement other pure virtual functions from the interface
    void onLocoFunctionChange(const LocoHandle& loco, int fIndex, bool isActive) override {}
    void onLocoFunctionAnalogChange(const LocoHandle& loco, int fIndex, uint8_t value) override {}
    void onLocoDispatchStateChange(const LocoHandle& loco, bool isAcquired, std::string ownerId) override {}
    void onConsistLink(const LocoHandle& master, const LocoHandle& slave, ConsistType type, bool inverted) override {}
    void onConsistUnlink(const LocoHandle& slave) override {}
    void onTurnoutChange(uint16_t address, bool isThrown, bool isFeedback) override {}
    void onSignalAspectChange(uint16_t address, uint8_t aspectId, bool isFeedback) override {}
    void onAccessoryAnalogValue(uint16_t address, float value0to1) override {}
    void onAccessoryError(uint16_t address, uint8_t errorId, std::string errorMsg) override {}
    void onSensorStateChange(uint32_t sensorId, bool isActive) override {}
    void onTrackPowerChange(PowerState state) override {}
    void onFastClockUpdated(int64_t modelTimeUnix, float factor) override {}
    void onHardwareNodeAttached(std::string nodeUid, std::string productName, bool booster) override {}
    void onHardwareNodeLost(std::string nodeUid) override {}
    void onSystemMessage(std::string source, std::string message) override {}
    void onLocoDetectedOnBlock(uint32_t sensorId, const LocoHandle& loco, DecoderOrientation orientation) override {}
    void onLocoTelemetryData(const LocoHandle& loco, TelemetryType type, float value) override {}
    void onLocoExternalStateChange(const LocoHandle& loco, ExternalState state) override {}
    void onLocoRailComRawData(const LocoHandle& loco, uint8_t appId, const std::vector<uint8_t>& data) override {}
    void onNewLocoDiscovered(const LocoHandle& loco, const std::string& name, const std::string& icon) override {}
    void onCvReadResult(const LocoHandle& loco, int cvNumber, uint8_t value, bool success) override {}
    void onSusiConfigRead(const LocoHandle& loco, uint8_t bankIndex, uint8_t susiIndex, uint8_t value) override {}
    void onConfigBlockLoad(const LocoHandle& loco, std::string domain, const std::vector<uint8_t>& data) override {}
    void onProgressUpdate(std::string operation, float percent) override {}
    void onLocoEventSync(const LocoHandle& loco, SyncType type, uint32_t value) override {
        _stream->println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        _stream->println("<xTrainEvents>");
        _stream->println("    <event type=\"onLocoEventSync\">");
        _stream->print("        <loco address=\"");
        _stream->print(loco.address);
        _stream->print("\" protocol=\"DCC\" mfxUid=\"");
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
        _stream->println("</xTrainEvents>");
    }

private:
    Stream* _stream;
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
