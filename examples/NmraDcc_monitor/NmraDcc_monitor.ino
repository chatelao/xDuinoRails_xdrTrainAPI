#include <NmraDcc.h>
#include "xDuinoRails_xTrainAPI.h"
#include "xDuinoRails_xTrainAPI_utils.h"

// This example requires the NmraDcc library.
// You can install it from the Arduino Library Manager.

NmraDcc Dcc;
ModelRail::CmdLinePrinter printer(Serial);

void notifyDccSpeed(uint16_t Addr, DCC_ADDR_TYPE AddrType, uint8_t Speed, DCC_DIRECTION Dir, DCC_SPEED_STEPS SpeedSteps) {
    ModelRail::LocoHandle loco = {Addr, ModelRail::Protocol::DCC, 0};
    ModelRail::Direction direction = (Dir == DCC_DIR_FWD) ? ModelRail::Direction::FORWARD : ModelRail::Direction::REVERSE;
    printer.onLocoSpeedChange(loco, Speed, direction, 128);
}

void notifyDccFunc(uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8_t FuncState) {
    ModelRail::LocoHandle loco = {Addr, ModelRail::Protocol::DCC, 0};
    uint8_t FuncNum = 0;
    uint8_t FuncBit = 0;

    switch (FuncGrp) {
        case FN_0_4:
            FuncNum = 0;
            if (FuncState & FN_BIT_00) printer.onLocoFunctionChanged(loco, 0, (FuncState & FN_BIT_00) != 0);
            if (FuncState & FN_BIT_01) printer.onLocoFunctionChanged(loco, 1, (FuncState & FN_BIT_01) != 0);
            if (FuncState & FN_BIT_02) printer.onLocoFunctionChanged(loco, 2, (FuncState & FN_BIT_02) != 0);
            if (FuncState & FN_BIT_03) printer.onLocoFunctionChanged(loco, 3, (FuncState & FN_BIT_03) != 0);
            if (FuncState & FN_BIT_04) printer.onLocoFunctionChanged(loco, 4, (FuncState & FN_BIT_04) != 0);
            break;
        case FN_5_8:
            FuncNum = 5;
            for (FuncBit = 0; FuncBit < 4; FuncBit++) {
                printer.onLocoFunctionChanged(loco, FuncNum + FuncBit, (FuncState & (1 << FuncBit)) != 0);
            }
            break;
        case FN_9_12:
            FuncNum = 9;
            for (FuncBit = 0; FuncBit < 4; FuncBit++) {
                printer.onLocoFunctionChanged(loco, FuncNum + FuncBit, (FuncState & (1 << FuncBit)) != 0);
            }
            break;
        case FN_13_20:
            FuncNum = 13;
            for (FuncBit = 0; FuncBit < 8; FuncBit++) {
                printer.onLocoFunctionChanged(loco, FuncNum + FuncBit, (FuncState & (1 << FuncBit)) != 0);
            }
            break;
        case FN_21_28:
            FuncNum = 21;
            for (FuncBit = 0; FuncBit < 8; FuncBit++) {
                printer.onLocoFunctionChanged(loco, FuncNum + FuncBit, (FuncState & (1 << FuncBit)) != 0);
            }
            break;
        default:
            break;
    }
}

void notifyDccAccTurnoutBoard(uint16_t BoardAddr, uint8_t OutputPair, uint8_t Direction, uint8_t OutputPower) {
    printer.onTurnoutChanged(BoardAddr, Direction, false);
}

void notifyDccAccTurnoutOutput(uint16_t Addr, uint8_t Direction, uint8_t OutputPower) {
    printer.onTurnoutChanged(Addr, Direction, false);
}

void notifyDccSigOutputState(uint16_t Addr, uint8_t State) {
    // xTrainAPI does not have a signal aspect event, so we will print a custom message.
    Serial.print("DCC Signal Aspect: Addr=");
    Serial.print(Addr);
    Serial.print(", State=");
    Serial.println(State);
}

void notifyFastClockTime(uint8_t WeekDay, uint8_t Hour, uint8_t Minute, uint8_t Factor, bool Update) {
    // xTrainAPI does not have a clock event, so we will print a custom message.
    Serial.print("DCC Clock: ");
    Serial.print(Hour);
    Serial.print(":");
    Serial.print(Minute);
    Serial.print(" Rate: ");
    Serial.println(Factor);
}

void notifyDccReset(uint8_t hardReset) {
    // xTrainAPI does not have a reset event, so we will print a custom message.
    Serial.println("DCC Reset");
}

void notifyDccIdle() {
    // xTrainAPI does not have an idle event, so we will print a custom message.
    Serial.println("DCC Idle");
}

void notifyDccMsg(DCC_MSG *Msg) {
    // This is a temporary parser for unsupported DCC events.
    if (Msg->Size > 0 && (Msg->Data[0] >= 0xC0 && Msg->Data[0] <= 0xDF)) {
        // Decoder Control Command
        uint16_t Addr = Msg->Data[0] & 0x3F;
        uint8_t Cmd = Msg->Data[1];
        Serial.print("Decoder Control Command: Addr=");
        Serial.print(Addr);
        Serial.print(", Cmd=");
        Serial.println(Cmd, HEX);
    } else {
        Serial.print("DCC Raw Packet: ");
        for (int i = 0; i < Msg->Size; i++) {
            Serial.print(Msg->Data[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    Dcc.init(MAN_ID_DIY, 0, 0, 0);
    Dcc.pin(digitalPinToInterrupt(2), 0);
}

void loop() {
    Dcc.process();
}
