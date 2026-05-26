#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "EspNowPackets.hpp"
#include "EspNowConfig.hpp"

/**
 * Telemetry (body side)
 *
 * Sends TelemetryPackets to the head ESP32-CAM via ESP-NOW at 10 Hz.
 * Receives CommandPackets from the head and dispatches them via callbacks.
 *
 * WiFi is set to STA mode (no AP, no association) — the head owns the AP.
 * ESP-NOW and the head AP must be on the same channel (ESPNOW_CHANNEL).
 *
 */
class Telemetry {
public:
    Telemetry() = default;

    void begin();
    void update();  // no-op — ESP-NOW recv fires in a FreeRTOS callback

    void sendTelemetry(
        float roll,       float pitch,
        float rollTarget, float rollOutput, float rollErr, float rollIntegral, float rollDerivative,
        float driveSpeed);

    void onTargetChanged(void (*cb)(float targetDeg))                  { _onTarget        = cb; }
    void onRollGainsChanged(void (*cb)(float kp, float ki, float kd))  { _onRollGains     = cb; }
    void onDriveSpeedChanged(void (*cb)(float mPerSec))                { _onDriveSpeed    = cb; }
    void onStopChanged(void (*cb)(bool stop))                          { _onStop          = cb; }
    void onPitchLimiterChanged(void (*cb)(bool enabled, float maxDeg)) { _onPitchLimiter  = cb; }

    bool isConnected() const { return _peerRegistered; }

private:
    bool _peerRegistered = false;

    void (*_onTarget)(float)                    = nullptr;
    void (*_onRollGains)(float, float, float)   = nullptr;
    void (*_onDriveSpeed)(float)                = nullptr;
    void (*_onStop)(bool)                       = nullptr;
    void (*_onPitchLimiter)(bool, float)        = nullptr;

    void dispatchCommand(const CommandPacket& pkt);

    static Telemetry* _instance;
    static void onRecv(const uint8_t* mac, const uint8_t* data, int len);
};
