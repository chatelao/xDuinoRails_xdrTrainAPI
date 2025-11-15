#pragma once

#include "xDuinoRails_xTrainAPI.h"
#include <Arduino.h>

namespace ModelRail {

class CmdLinePrinter : public IUnifiedModelTrainListener {
public:
    CmdLinePrinter(Stream& stream) : _stream(&stream) {}

    void onLocoSpeedChanged(const LocoHandle& loco, float speedPercent, Direction direction, int speedSteps) override {
        _stream->print("<t ");
        _stream->print(loco.address);
        _stream->print(" ");
        _stream->print((int)speedPercent);
        _stream->print(" ");
        _stream->print(direction == Direction::FORWARD ? 1 : 0);
        _stream->println(">");
    }

    void onLocoFunctionChanged(const LocoHandle& loco, int fIndex, bool isActive) override {
        _stream->print("<f ");
        _stream->print(loco.address);
        _stream->print(" ");
        _stream->print(fIndex);
        _stream->print(" ");
        _stream->print(isActive ? 1 : 0);
        _stream->println(">");
    }

    void onTurnoutChanged(uint16_t address, bool isThrown, bool isFeedback) override {
        _stream->print("<T ");
        _stream->print(address);
        _stream->print(" ");
        _stream->print(isThrown ? 1 : 0);
        _stream->println(">");
    }

    void onSignalAspectChanged(uint16_t address, uint8_t aspectId, bool isFeedback) override {
        _stream->print("<S ");
        _stream->print(address);
        _stream->print(" ");
        _stream->print(aspectId);
        _stream->println(">");
    }

    void onTrackPowerChanged(PowerState state) override {
        _stream->print("<");
        _stream->print(state == PowerState::ON ? 1 : 0);
        _stream->println(">");
    }

    // Implement other pure virtual functions from the interface
    void onLocoFunctionAnalogValue(const LocoHandle& loco, int fIndex, uint8_t value) override {}
    void onLocoDispatchStateChanged(const LocoHandle& loco, bool isAcquired, std::string ownerId) override {}
    void onConsistLink(const LocoHandle& master, const LocoHandle& slave, ConsistType type, bool inverted) override {}
    void onConsistUnlink(const LocoHandle& slave) override {}
    void onAccessoryAnalogValue(uint16_t address, float value0to1) override {}
    void onAccessoryError(uint16_t address, uint8_t errorId, std::string errorMsg) override {}
    void onSensorStateChanged(uint32_t sensorId, bool isActive) override {}
    void onFastClockUpdated(int64_t modelTimeUnix, float factor) override {}
    void onHardwareNodeAttached(std::string nodeUid, std::string productName, bool booster) override {}
    void onHardwareNodeLost(std::string nodeUid) override {}
    void onSystemMessage(std::string source, std::string message) override {}
    void onLocoDetectedOnBlock(uint32_t sensorId, const LocoHandle& loco, DecoderOrientation orientation) override {}
    void onLocoTelemetryData(const LocoHandle& loco, TelemetryType type, float value) override {}
    void onLocoExternalStateChanged(const LocoHandle& loco, ExternalState state) override {}
    void onLocoRailComRawData(const LocoHandle& loco, uint8_t appId, const std::vector<uint8_t>& data) override {}
    void onNewLocoDiscovered(const LocoHandle& loco, const std::string& name, const std::string& icon) override {}
    void onCvReadResult(const LocoHandle& loco, int cvNumber, uint8_t value, bool success) override {}
    void onSusiConfigRead(const LocoHandle& loco, uint8_t bankIndex, uint8_t susiIndex, uint8_t value) override {}
    void onConfigBlockLoaded(const LocoHandle& loco, std::string domain, const std::vector<uint8_t>& data) override {}
    void onProgressUpdate(std::string operation, float percent) override {}
        // GROUP F: REAL-TIME SYNCHRONIZATION (Added in v2.3)
    void onMechanicalSyncEvent(const LocoHandle& loco, SyncType type, uint8_t value) override {}

private:
    Stream* _stream;
};

class CmdLineParser {
public:
    CmdLineParser(IUnifiedModelTrainListener& listener) : _listener(&listener) {}

    void parse(const String& command) {
        char cmdChar = command.charAt(0);
        String params = command.substring(1);
        params.trim();

        switch (cmdChar) {
            case '1':
                _listener->onTrackPowerChanged(PowerState::ON);
                break;
            case '0':
                _listener->onTrackPowerChanged(PowerState::OFF);
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
                        _listener->onLocoSpeedChanged(loco, speed, (Direction)dir, 128);
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
                        _listener->onLocoFunctionChanged(loco, func, state);
                    }
                }
                break;
            case 'T':
                {
                    int firstSpace = params.indexOf(' ');
                    if (firstSpace != -1) {
                        uint16_t addr = params.substring(0, firstSpace).toInt();
                        int state = params.substring(firstSpace + 1).toInt();
                        _listener->onTurnoutChanged(addr, state, false);
                    }
                }
                break;
            case 'S':
                {
                    int firstSpace = params.indexOf(' ');
                    if (firstSpace != -1) {
                        uint16_t addr = params.substring(0, firstSpace).toInt();
                        int aspect = params.substring(firstSpace + 1).toInt();
                        _listener->onSignalAspectChanged(addr, aspect, false);
                    }
                }
                break;
        }
    }

private:
    IUnifiedModelTrainListener* _listener;
};

}