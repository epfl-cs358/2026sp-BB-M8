#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <WebSocketsServer.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

/**
 * Telemetry.h
 *
 * Manages a WiFi Access Point, an HTTP server (serves the dashboard
 * from SPIFFS), and a WebSocket server (bidirectional data exchange).
 *
 * The dashboard HTML lives in data/index.html — upload it separately
 * via PlatformIO: Tasks > Upload Filesystem Image (or pio run -t uploadfs)
 *
 * Connect to hotspot, open http://192.168.4.1 in a browser.
 * WebSocket runs on ws://192.168.4.1:81
 *
 * Messages ESP32 -> browser (JSON):
 *   {"roll":1.2,"pitch":0.3,"err":0.5,"int":0.0,"der":0.1}
 *
 * Messages browser -> ESP32 (JSON):
 *   {"target":10.0}
 *   {"kp":1.5,"ki":0.0,"kd":0.05}
 */
class Telemetry {
public:
    Telemetry(const char* ssid, const char* password, uint16_t wsPort = 81);

    /**
     * Call once in setup().
     * Mounts SPIFFS, starts the HTTP server and WebSocket server.
     */
    void begin();

    /**
     * Call every loop iteration — processes incoming WebSocket messages.
     */
    void update();

    void sendTelemetry(float roll, float pitch,
                       float err, float integral, float derivative);

    void onTargetChanged(void (*callback)(float targetDeg)) {
        _onTarget = callback;
    }
    void onGainsChanged(void (*callback)(float kp, float ki, float kd)) {
        _onGains = callback;
    }

    bool isConnected() const { return _clientCount > 0; }

private:
    const char*      _ssid;
    const char*      _password;
    WebSocketsServer _ws;
    AsyncWebServer   _http;
    uint8_t          _clientCount;

    void (*_onTarget)(float targetDeg)             = nullptr;
    void (*_onGains)(float kp, float ki, float kd) = nullptr;

    void handleMessage(uint8_t* payload, size_t length);
    void webSocketEvent(uint8_t num, WStype_t type,
                        uint8_t* payload, size_t length);

    static Telemetry* _instance;
    static void staticWsEvent(uint8_t num, WStype_t type,
                              uint8_t* payload, size_t length);
};