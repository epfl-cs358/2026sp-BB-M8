#include "Telemetry.hpp"

Telemetry* Telemetry::_instance = nullptr;

void Telemetry::begin() {
    _instance = this;

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("[Telemetry] esp_now_init failed.");
        return;
    }

    esp_now_register_recv_cb(onRecv);

    esp_now_peer_info_t peer{};
    memcpy(peer.peer_addr, HEAD_MAC, 6);
    peer.channel = ESPNOW_CHANNEL;
    peer.encrypt = false;
    if (esp_now_add_peer(&peer) != ESP_OK) {
        Serial.println("[Telemetry] Failed to add head peer.");
        return;
    }

    _peerRegistered = true;
    Serial.printf("[Telemetry] ESP-NOW ready. Body MAC: %s\n", WiFi.macAddress().c_str());
}

void Telemetry::update() {
    // ESP-NOW recv fires in the WiFi FreeRTOS task; callbacks are invoked there directly.
    // The controller setters (setTarget, setGains, etc.) are single float/bool writes —
    // atomic on Xtensa — so no additional synchronisation is needed here.
}

void Telemetry::sendTelemetry(
        float roll,       float pitch,
        float rollTarget, float rollOutput, float rollErr, float rollIntegral, float rollDerivative,
        float driveSpeed) {
    if (!_peerRegistered) return;

    TelemetryPacket pkt{};
    pkt.roll            = roll;
    pkt.pitch           = pitch;
    pkt.rollTarget      = rollTarget;
    pkt.rollOutput      = rollOutput;
    pkt.rollErr         = rollErr;
    pkt.rollIntegral    = rollIntegral;
    pkt.rollDerivative  = rollDerivative;
    pkt.driveSpeed      = driveSpeed;

    esp_now_send(HEAD_MAC, reinterpret_cast<const uint8_t*>(&pkt), sizeof(pkt));
}

void Telemetry::dispatchCommand(const CommandPacket& pkt) {
    switch (pkt.cmdType) {
        case CMD_TARGET:
            if (_onTarget) _onTarget(pkt.target);
            break;
        case CMD_ROLL_GAINS:
            if (_onRollGains) _onRollGains(pkt.rollKp, pkt.rollKi, pkt.rollKd);
            break;
        case CMD_DRIVE_SPEED:
            if (_onDriveSpeed) _onDriveSpeed(pkt.driveSpeed);
            break;
        case CMD_STOP:
            if (_onStop) _onStop(pkt.stop != 0);
            break;
        default:
            break;
    }
}

void Telemetry::onRecv(const uint8_t* /*mac*/, const uint8_t* data, int len) {
    if (!_instance || len < static_cast<int>(sizeof(CommandPacket))) return;
    const auto* pkt = reinterpret_cast<const CommandPacket*>(data);
    if (pkt->type != PacketType::COMMAND) return;
    _instance->dispatchCommand(*pkt);
}
