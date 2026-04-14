#include "Telemetry.hpp"

Telemetry* Telemetry::_instance = nullptr;

Telemetry::Telemetry(const char* ssid, const char* password, uint16_t wsPort)
    : _ssid(ssid), _password(password),
      _ws(wsPort), _http(80), _clientCount(0)
{}

void Telemetry::begin() {
    _instance = this;

    // Start WiFi Access Point
    WiFi.softAP(_ssid, _password);

    // Mount SPIFFS filesystem (where index.html lives)
    if (!SPIFFS.begin(true)) {
        Serial.println("[Telemetry] SPIFFS mount failed.");
        return;
    }

    // Serve dashboard from SPIFFS
    _http.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    _http.begin();

    // Start WebSocket server
    _ws.begin();
    _ws.onEvent(staticWsEvent);
}

void Telemetry::update() {
    _ws.loop();
}

void Telemetry::sendTelemetry(float roll, float pitch,
                               float err, float integral, float derivative) {
    if (_clientCount == 0) return;

    StaticJsonDocument<128> doc;
    doc["roll"]  = serialized(String(roll,       2));
    doc["pitch"] = serialized(String(pitch,      2));
    doc["err"]   = serialized(String(err,        2));
    doc["int"]   = serialized(String(integral,   2));
    doc["der"]   = serialized(String(derivative, 2));

    char buffer[128];
    serializeJson(doc, buffer);
    _ws.broadcastTXT(buffer);
}

void Telemetry::handleMessage(uint8_t* payload, size_t length) {
    StaticJsonDocument<128> doc;
    if (deserializeJson(doc, payload, length)) return;

    if (doc.containsKey("target") && _onTarget)
        _onTarget((float)doc["target"]);

    if (doc.containsKey("kp") && doc.containsKey("ki") &&
        doc.containsKey("kd") && _onGains)
        _onGains((float)doc["kp"], (float)doc["ki"], (float)doc["kd"]);
}

void Telemetry::webSocketEvent(uint8_t num, WStype_t type,
                                uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_CONNECTED:
            _clientCount++;
            Serial.printf("[WS] Client #%d connected\n", num);
            break;
        case WStype_DISCONNECTED:
            if (_clientCount > 0) _clientCount--;
            break;
        case WStype_TEXT:
            handleMessage(payload, length);
            break;
        default:
            break;
    }
}

void Telemetry::staticWsEvent(uint8_t num, WStype_t type,
                               uint8_t* payload, size_t length) {
    if (_instance)
        _instance->webSocketEvent(num, type, payload, length);
}