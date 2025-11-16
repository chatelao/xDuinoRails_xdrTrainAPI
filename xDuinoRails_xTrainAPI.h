/**
 * @file UnifiedTrainAPI.h
 * @brief Abstract Interface for a Unified Model Train Control System.
 * @author ModelRail Architecture
 * @version 2.3 (Extended Sync Edition)
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <variant>

namespace ModelRail {

    // =============================================================
    // 1. DATA TYPES & ENUMS (The Vocabulary)
    // =============================================================

    /**
     * @brief List of supported digital protocols.
     * See NMRA, RCN, and Manufacturer specs for details.
     */
    enum class Protocol : uint8_t {
        DCC,            ///< NMRA DCC (S-9.1, S-9.2, RCN-210). Worldwide standard.
        MM_I,           ///< Marklin Motorola I (Old). Register mode, 14 steps, unidirectional.
        MM_II,          ///< Marklin Motorola II (New). Adds direction and functions F1-F4.
        MFX,            ///< mfx / M4. Bidirectional, auto-registration (UID), 128 steps.
        SELECTRIX,      ///< Selectrix (SX1). Synchronous bus (NEM 680/681).
        SX2,            ///< Selectrix 2 (SX2). RCN extension for multiprotocol & config.
        LOCONET,        ///< Digitrax LocoNet. Peer-to-Peer, OpCode based.
        BIDIB,          ///< BiDiB. High-speed, tree-topology, CRC protected.
        XPRESSNET,      ///< Lenz XpressNet. RS-485 Master-Slave.
        CAN_GENERIC     ///< Generic CAN (e.g., OpenLCB/LCC or Marklin CS2/CS3).
    };

    // --- Basic State Definitions ---

    /** @brief Logical driving direction. */
    enum class Direction : uint8_t {
        REVERSE = 0,    ///< Backward movement.
        FORWARD = 1,    ///< Forward movement.
        UNKNOWN = 2     ///< State is undetermined.
    };

    /** @brief Global track power status. */
    enum class PowerState : uint8_t {
        OFF            = 0, ///< System idle / No power to tracks.
        ON             = 1, ///< Normal operation.
        EMERGENCY_STOP = 2, ///< Logical stop (broadcast speed 0), power may remain.
        SHORT_CIRCUIT  = 3  ///< Hardware trip / Power cut.
    };

    // --- RCN-217 / RailCom Definitions ---

    /** @brief Physical orientation of the decoder on the track (RCN-217). */
    enum class DecoderOrientation : uint8_t {
        NORMAL   = 0,      ///< Decoder "Forward" matches Track "Positive" (Chimney first).
        INVERTED = 1,      ///< Decoder "Forward" matches Track "Negative" (Tender first).
        UNKNOWN  = 2       ///< Orientation detection failed.
    };

    /** @brief External influence on movement (ABC, HLU, etc.). */
    enum class ExternalState : uint8_t {
        FREE_RUN              = 0, ///< No restrictions.
        STOPPED_BY_ABC_SIGNAL = 1, ///< Stopped by Lenz ABC Asymmetry.
        STOPPED_BY_DC_BRAKE   = 2, ///< Stopped by Marklin DC Brake Module.
        STOPPED_BY_HLU        = 3, ///< Stopped by Zimo HLU signal.
        RESTRICTED_SPEED_HLU  = 4  ///< Speed limit enforced by Zimo HLU.
    };

    /** @brief Telemetry data types (RailCom Ch2 / BiDiB). */
    enum class TelemetryType : uint8_t {
        SPEED_KMH,          ///< Actual measured speed (km/h).
        LOAD_PERCENT,       ///< Motor load (0-100%).
        VOLTAGE_TRACK,      ///< Decoder input voltage (V).
        CURRENT_AMP,        ///< Motor current (A).
        FUEL_LEVEL,         ///< Simulated supply level (Coal/Water/Diesel %).
        TEMP_CELSIUS,       ///< Internal temperature (°C).
        QOS_ERROR_RATE,     ///< Packet error rate / QoS.
        ODOMETER_VALUE,     ///< Total distance travelled (cm/m).
        POSITION_CONFIDENCE ///< Localization confidence (0-100%).
    };

    // --- Real-Time Synchronization (Added in v2.3) ---

    /** * @brief Types of hardware/mechanical sync events.
     * Used to couple mechanical movement (Motor/Wheels) with Acoustics (Sound) or Effects.
     */
    enum class SyncType : uint8_t {
        CAM_PULSE,          ///< Wheel rotation pulse (e.g., for steam chuff generation).
        CYLINDER_CYCLE,     ///< Full cylinder cycle completed.
        GEAR_CHANGE_UP,     ///< Gearbox shifted up (for diesel/hydraulic transmission).
        GEAR_CHANGE_DOWN,   ///< Gearbox shifted down.
        BRAKE_SQUEAL_START, ///< Mechanical brake engaged (triggers squeal sound).
        DOOR_MOVEMENT       ///< Door opening/closing detected.
    };

    // --- Consisting / Management ---

    /** @brief Linking method for multi-traction. */
    enum class ConsistType : uint8_t {
        ADVANCED_DCC,   ///< Decoder-based (CV19).
        UNIVERSAL_HOST, ///< Software-based (Command Station manages commands).
        MU_LOCONET      ///< LocoNet specific slot linking.
    };

    /** @brief Core Identifier for a Locomotive. */
    struct LocoHandle {
        uint16_t address;       ///< Primary address (e.g., 3, 9999).
        Protocol protocol;      ///< Protocol used (DCC, MFX, etc.).
        uint32_t mfxUid;        ///< Unique ID (takes precedence if non-zero).
    };

    // =============================================================
    // 2. THE UNIFIED INTERFACE (Abstract Base Class)
    // =============================================================

    class IUnifiedModelTrainListener {
    public:
        virtual ~IUnifiedModelTrainListener() = default;

        // -------------------------------------------------------------
        // GROUP A: TRAFFIC & DISPATCHING
        // -------------------------------------------------------------
        
        /**
         * @brief Standard speed control (RCN-212).
         * @param loco Target locomotive.
         * @param speedPercent 0.0f (Stop) to 100.0f (Max).
         * @param direction Requested direction.
         * @param speedSteps Step mode (14, 28, 128).
         */
        virtual void onLocoSpeedChange(const LocoHandle& loco, float speedPercent, Direction direction, int speedSteps) = 0;

        /**
         * @brief Binary function control (F0-F68+).
         * @param fIndex Function index (0=Light, 1=F1...).
         * @param isActive True=ON, False=OFF.
         */
        virtual void onLocoFunctionChange(const LocoHandle& loco, int fIndex, bool isActive)                            = 0;

        /**
         * @brief Analog function value (Pressure-sensitive / Analog features).
         * @param value Analog value (0-255).
         */
        virtual void onLocoFunctionAnalogChange(const LocoHandle& loco, int fIndex, uint8_t value)                        = 0;

        /**
         * @brief Slot Management (LocoNet/XpressNet).
         * @param isAcquired True=Controlled by throttle, False=Released.
         * @param ownerId ID of the controlling device.
         */
        virtual void onLocoDispatchStateChange(const LocoHandle& loco, bool isAcquired, std::string ownerId)            = 0;

        /**
         * @brief Consist Linking report.
         * @param type Linking method (CV19 vs Software).
         * @param inverted True if slave is reversed relative to master.
         */
        virtual void onConsistLink(const LocoHandle& master, const LocoHandle& slave, ConsistType type, bool inverted)   = 0;

        /**
         * @brief Dissolve a multi-traction link.
         * @param slave The locomotive being removed from the consist.
         */
        virtual void onConsistUnlink(const LocoHandle& slave)                                                            = 0;

        // -------------------------------------------------------------
        // GROUP B: INFRASTRUCTURE (ACCESSORIES)
        // -------------------------------------------------------------

        /**
         * @brief Standard Turnout (Binary).
         * @param isThrown True=Diverging, False=Straight.
         * @param isFeedback True=Real hardware confirmation.
         */
        virtual void onTurnoutChange(uint16_t address, bool isThrown, bool isFeedback)                 = 0;

        /**
         * @brief Extended Signal Aspects (0-255).
         * @param aspectId Aspect ID (Hp0, Hp1, etc.).
         */
        virtual void onSignalAspectChange(uint16_t address, uint8_t aspectId, bool isFeedback)         = 0;
        
        /**
         * @brief Direct analog control (Servos, Dimmers).
         * @param value0to1 Normalized float (0.0 - 1.0).
         */
        virtual void onAccessoryAnalogValue(uint16_t address, float value0to1)                          = 0;

        /**
         * @brief Hardware Diagnostic Error.
         * @param errorId 1=OpenLoad, 2=Short, 3=Stall.
         */
        virtual void onAccessoryError(uint16_t address, uint8_t errorId, std::string errorMsg)          = 0;

        /**
         * @brief Occupancy / Feedback Sensors.
         * @param sensorId Unique 32-bit ID.
         * @param isActive True=Occupied.
         */
        virtual void onSensorStateChange(uint32_t sensorId, bool isActive)                             = 0;

        // -------------------------------------------------------------
        // GROUP C: SYSTEM, TIME & TOPOLOGY
        // -------------------------------------------------------------

        /**
         * @brief Report global track power status change.
         * @param state The new power state (ON, OFF, EMERGENCY_STOP).
         */
        virtual void onTrackPowerChange(PowerState state)                                                              = 0;

        /**
         * @brief Model Time / Fast Clock (RCN-211).
         * @param modelTimeUnix Unix Timestamp (int64).
         * @param factor Acceleration factor.
         */
        virtual void onFastClockUpdated(int64_t modelTimeUnix, float factor)                                            = 0;

        /**
         * @brief New Hardware Node Found (BiDiB / LNet).
         * @param booster True if node supplies power.
         */
        virtual void onHardwareNodeAttached(std::string nodeUid, std::string productName, bool booster) = 0;

        /**
         * @brief A hardware node has disconnected from the bus.
         * @param nodeUid Unique identifier of the lost node.
         */
        virtual void onHardwareNodeLost(std::string nodeUid)                                            = 0;

        /**
         * @brief Generic system message or log entry.
         * @param source The component originating the message (e.g., "PowerManager", "BiDiB-Stack").
         * @param message The content of the message.
         */
        virtual void onSystemMessage(std::string source, std::string message)                           = 0;

        // -------------------------------------------------------------
        // GROUP D: TELEMETRY & IDENTIFICATION
        // -------------------------------------------------------------

        /**
         * @brief Train Location & Orientation Detection (RCN-217 Ch1).
         * @param orientation Chimney vs Tender first.
         */
        virtual void onLocoDetectedOnBlock(uint32_t sensorId, const LocoHandle& loco, DecoderOrientation orientation)    = 0;
        virtual void onLocoTelemetryData(const LocoHandle& loco, TelemetryType type, float value)                        = 0; 

        /**
         * @brief External State Change (ABC / HLU).
         * Loco stopped by track module despite throttle > 0.
         */
        virtual void onLocoExternalStateChange(const LocoHandle& loco, ExternalState state)                             = 0;

        /**
         * @brief Raw/Proprietary RailCom Data.
         * @param appId RailCom Application ID.
         */
        virtual void onLocoRailComRawData(const LocoHandle& loco, uint8_t appId, const std::vector<uint8_t>& data)       = 0;

        /**
         * @brief Discovery (mfx / RailComPlus).
         * @param icon Suggested icon filename.
         */
        virtual void onNewLocoDiscovered(const LocoHandle& loco, const std::string& name, const std::string& icon)       = 0;

        // -------------------------------------------------------------
        // GROUP E: CONFIGURATION & MASS DATA
        // -------------------------------------------------------------

        /**
         * @brief A request to read a CV from a decoder has been issued.
         * @param loco The target locomotive.
         * @param cvNumber The Configuration Variable number to read.
         */
        virtual void onCvReadRequest(const LocoHandle& loco, int cvNumber)                                               = 0;

        /**
         * @brief A request to write a CV to a decoder has been issued.
         * @param loco The target locomotive.
         * @param cvNumber The Configuration Variable number to write.
         * @param value The 8-bit value to write.
         */
        virtual void onCvWriteRequest(const LocoHandle& loco, int cvNumber, uint8_t value)                                = 0;

        /**
         * @brief Result of a CV read operation.
         * @param loco The target locomotive.
         * @param cvNumber The CV that was read.
         * @param value The 8-bit value returned by the decoder.
         * @param success True if the read was successful (acknowledged).
         */
        virtual void onCvReadResult(const LocoHandle& loco, int cvNumber, uint8_t value, bool success)                   = 0;

        /**
         * @brief Result of reading a SUSI configuration register.
         * @param loco The target locomotive.
         * @param bankIndex The memory bank (e.g., for different sound profiles).
         * @param susiIndex The specific SUSI register index.
         * @param value The value read from the register.
         */
        virtual void onSusiConfigRead(const LocoHandle& loco, uint8_t bankIndex, uint8_t susiIndex, uint8_t value)       = 0;
        
        /**
         * @brief Mass Data Transfer.
         * @param domain Data type (e.g., "ICON", "MFX_CONFIG").
         */
        virtual void onConfigBlockLoad(const LocoHandle& loco, std::string domain, const std::vector<uint8_t>& data)   = 0;

        /**
         * @brief Feedback on the progress of a long-running operation.
         * @param operation A string identifying the task (e.g., "CV_READ_ALL", "FIRMWARE_UPDATE").
         * @param percent The progress from 0.0 to 100.0.
         */
        virtual void onProgressUpdate(std::string operation, float percent)                                              = 0;

        // -------------------------------------------------------------
        // GROUP F: REAL-TIME SYNCHRONIZATION (Added in v2.3)
        // -------------------------------------------------------------

        /**
         * @brief Triggered when a mechanical synchronization event occurs.
         * Critical for "Puff-Chuff" sync (Steam) or Gear Changes (Diesel).
         * @param loco The locomotive handle.
         * @param type The type of mechanical event (e.g., CAM_PULSE).
         * @param value An optional value (e.g., new gear number).
         */
        virtual void onLocoEventSync(const LocoHandle& loco, SyncType type, uint32_t value) = 0;
    };
}
