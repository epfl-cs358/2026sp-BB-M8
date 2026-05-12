#include "Telemetry.hpp"

Telemetry* Telemetry::_instance = nullptr;

Telemetry::Telemetry(uint16_t wsPort)
    : _ws(wsPort), _http(80)
{}

void Telemetry::begin() {
    _instance = this;

    // softAP must be started BEFORE esp_now_init so both share the same channel.
    WiFi.mode(WIFI_STA);
    WiFi.softAP(AP_SSID, AP_PASS, ESPNOW_CHANNEL);
    Serial.printf("[Telemetry] AP up — SSID: %s  IP: %s\n",
                  AP_SSID, WiFi.softAPIP().toString().c_str());

    if (esp_now_init() != ESP_OK) {
        Serial.println("[Telemetry] esp_now_init failed.");
        return;
    }

    if (!SPIFFS.begin(true)) {
        Serial.println("[Telemetry] SPIFFS mount failed.");
        return;
    }

    _http.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    _http.begin();

    _ws.begin();
    _ws.onEvent(staticWsEvent);
}

void Telemetry::update() {
    _ws.loop();
}

void Telemetry::forwardTelemetry(const TelemetryPacket& pkt) {
    if (_clientCount == 0) return;

    StaticJsonDocument<256> doc;

    JsonObject state = doc.createNestedObject("state");
    state["roll"]  = serialized(String(pkt.roll,  2));
    state["pitch"] = serialized(String(pkt.pitch, 2));

    JsonObject pid = doc.createNestedObject("pid");
    pid["target"] = serialized(String(pkt.rollTarget,    2));
    pid["output"] = serialized(String(pkt.rollOutput,    2));
    pid["err"]    = serialized(String(pkt.rollErr,       2));
    pid["int"]    = serialized(String(pkt.rollIntegral,  2));
    pid["der"]    = serialized(String(pkt.rollDerivative,2));

    JsonObject drive = doc.createNestedObject("drive");
    drive["speed"] = serialized(String(pkt.driveSpeed, 2));

    char buffer[256];
    serializeJson(doc, buffer);
    _ws.broadcastTXT(buffer);
}

void Telemetry::handleMessage(uint8_t* payload, size_t length) {
    StaticJsonDocument<192> doc;
    if (deserializeJson(doc, payload, length)) return;

    if (doc.containsKey("target") && _onTarget)
        _onTarget((float)doc["target"]);

    if (doc.containsKey("rollKp") && doc.containsKey("rollKi") &&
        doc.containsKey("rollKd") && _onRollGains)
        _onRollGains((float)doc["rollKp"], (float)doc["rollKi"], (float)doc["rollKd"]);

    if (doc.containsKey("driveSpeed") && _onDriveSpeed)
        _onDriveSpeed((float)doc["driveSpeed"]);

    if (doc.containsKey("stop") && _onStop)
        _onStop((bool)doc["stop"]);

    if (doc.containsKey("headStep") && _onHeadStep)   // ← add here
        _onHeadStep((int)doc["headStep"]);
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
