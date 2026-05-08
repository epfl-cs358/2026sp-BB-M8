#pragma once

// Body ESP32 STA MAC address.
static const uint8_t BODY_MAC[6] = { 0xE4, 0x65, 0xB8, 0x76, 0xD0, 0x50 };

// WiFi AP channel — ESP-NOW on both boards must use this channel
constexpr uint8_t ESPNOW_CHANNEL = 1;

// Dashboard WiFi AP credentials
constexpr char AP_SSID[] = "BBM8";
constexpr char AP_PASS[] = "starwars";
