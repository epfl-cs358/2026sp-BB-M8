#pragma once

// Head ESP32-CAM STA MAC address.
static const uint8_t HEAD_MAC[6] = { 0x30, 0xC6, 0xF7, 0x51, 0x76, 0x50 };

// Must match the softAP channel used by the head.
constexpr uint8_t ESPNOW_CHANNEL = 1;
