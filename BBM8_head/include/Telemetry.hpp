#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <WebSocketsServer.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <esp_now.h>
#include "EspNowPackets.hpp"
#include "EspNowConfig.hpp"

/**
 * Telemetry (head side)
 *
 * Owns the WiFi Access Point, HTTP dashboard (SPIFFS), and WebSocket server.
 * ESP-NOW is initialised here (softAP must come first to fix the channel).
 *
 * main.cpp wires the four on* callbacks to functions that send CommandPackets
 * to the body via ESP-NOW. When an ESP-NOW TelemetryPacket arrives from the
 * body, main.cpp calls forwardTelemetry() which re-serialises it to JSON and
 * broadcasts to all WebSocket clients.
 *
 * Connect to "BBM8" hotspot, open http://192.168.4.1 in a browser.
 * WebSocket: ws://192.168.4.1:81
 */
class Telemetry {
public:
    explicit Telemetry(uint16_t wsPort = 81);

    void begin();
    void update();  // call every loop — drives _ws.loop()

    // Called by main.cpp ESP-NOW recv handler with a packet from the body.
    void forwardTelemetry(const TelemetryPacket& pkt);

    void onHeadStepChanged(void (*cb)(int steps)) { _onHeadStep = cb; }

    void onTargetChanged(void (*cb)(float targetDeg))                   { _onTarget    = cb; }
    void onRollGainsChanged(void (*cb)(float kp, float ki, float kd))   { _onRollGains = cb; }
    void onDriveSpeedChanged(void (*cb)(float mPerSec))                 { _onDriveSpeed= cb; }
    void onStopChanged(void (*cb)(bool stop))                           { _onStop      = cb; }

    bool isConnected() const { return _clientCount > 0; }

private:
    WebSocketsServer _ws;
    AsyncWebServer   _http;
    uint8_t          _clientCount = 0;

    void (*_onHeadStep)(int) = nullptr;

    void (*_onTarget)(float)                    = nullptr;
    void (*_onRollGains)(float, float, float)   = nullptr;
    void (*_onDriveSpeed)(float)                = nullptr;
    void (*_onStop)(bool)                       = nullptr;

    void handleMessage(uint8_t* payload, size_t length);
    void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);

    static Telemetry* _instance;
    static void staticWsEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
};
