#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "Telemetry.hpp"
#include "EspNowPackets.hpp"
#include "EspNowConfig.hpp"
#include "Camera.hpp"

Telemetry telemetry;

// ── ESP-NOW: send a CommandPacket to the body ────────────────────────────────
static void sendCommand(const CommandPacket& cmd) {
    esp_now_send(BODY_MAC, reinterpret_cast<const uint8_t*>(&cmd), sizeof(cmd));
}

// ── ESP-NOW: receive TelemetryPacket from body ───────────────────────────────
void onEspNowRecv(const uint8_t* /*mac*/, const uint8_t* data, int len) {
    if (len < static_cast<int>(sizeof(TelemetryPacket))) return;
    const auto* pkt = reinterpret_cast<const TelemetryPacket*>(data);
    if (pkt->type != PacketType::TELEMETRY) return;
    telemetry.forwardTelemetry(*pkt);
}

void setup() {
    Serial.begin(115200);

    // Wire browser commands → ESP-NOW sends to body.
    telemetry.onTargetChanged([](float t) {
        CommandPacket cmd{};
        cmd.cmdType = CMD_TARGET;
        cmd.target  = t;
        sendCommand(cmd);
    });
    telemetry.onRollGainsChanged([](float kp, float ki, float kd) {
        CommandPacket cmd{};
        cmd.cmdType = CMD_ROLL_GAINS;
        cmd.rollKp  = kp;
        cmd.rollKi  = ki;
        cmd.rollKd  = kd;
        sendCommand(cmd);
    });
    telemetry.onDriveSpeedChanged([](float v) {
        CommandPacket cmd{};
        cmd.cmdType    = CMD_DRIVE_SPEED;
        cmd.driveSpeed = v;
        sendCommand(cmd);
    });
    telemetry.onStopChanged([](bool s) {
        CommandPacket cmd{};
        cmd.cmdType = CMD_STOP;
        cmd.stop    = s ? 1u : 0u;
        sendCommand(cmd);
    });

    // begin() starts softAP on ESPNOW_CHANNEL, then esp_now_init, then HTTP + WebSocket.
    telemetry.begin();

    if (camera_init())
        camera_stream_begin();

    // Register body as ESP-NOW peer (for outgoing CommandPackets).
    esp_now_peer_info_t peer{};
    memcpy(peer.peer_addr, BODY_MAC, 6);
    peer.channel = ESPNOW_CHANNEL;
    peer.encrypt = false;
    if (esp_now_add_peer(&peer) != ESP_OK) {
        Serial.println("[HEAD] Failed to add body peer.");
    }

    // Register callback for incoming TelemetryPackets from body.
    esp_now_register_recv_cb(onEspNowRecv);

    Serial.printf("[HEAD] AP MAC: %s\n", WiFi.softAPmacAddress().c_str());
    Serial.println("[HEAD] Ready.");
}

void loop() {
    telemetry.update();
}
