#include <Arduino.h>
#include "StateEstimator.hpp"
#include "RollController.hpp"
#include "Telemetry.hpp"

// ---- Config ----
constexpr int      SERVO_PIN = 18;      // GPIO pin connected to servo signal wire
constexpr uint32_t LOOP_MS   = 20;      // 50 Hz control loop

// ---- PID gains (!!! TO TUNE !!!) ---- 
constexpr float KP_ROLL = 1.0f;
constexpr float KI_ROLL = 0.0f;
constexpr float KD_ROLL = 0.05f;

// ---- Complementary filter coefficient (!!! TO TUNE !!!) ----
// 0.98 = trust gyro 98% short-term, correct with accel 2% long-term
constexpr float ALPHA = 0.98f;

// ---- WiFi hotspot credentials ----
constexpr char WIFI_SSID[] = "BBM8-body";
constexpr char WIFI_PASS[] = "starwars";

// ---- Module instances ----
StateEstimator state(ALPHA);
RollController rollCtrl(KP_ROLL, KI_ROLL, KD_ROLL, SERVO_PIN);
Telemetry      telemetry(WIFI_SSID, WIFI_PASS);

uint32_t lastTime = 0;

void setup() {
    Serial.begin(115200);

    if (!state.begin()) {
        Serial.println("[ERROR] MPU-9250 not found. Check wiring.");
        while (true) delay(1000);  // Halt
    }
    Serial.println("[OK] MPU-9250 initialised.");

    rollCtrl.begin();
    Serial.println("[OK] RollController ready.");

    // Register callbacks for incoming commands
    telemetry.onTargetChanged([](float targetDeg) {
        rollCtrl.setTarget(targetDeg);
    });
    telemetry.onGainsChanged([](float kp, float ki, float kd) {
        rollCtrl.pid().setGains(kp, ki, kd);
    });
    // Start the WiFi only after registering the callbacks
    telemetry.begin();

    lastTime = millis();
}

void loop() {
    // Handle incoming WebSocket messages
    telemetry.update();

    uint32_t now     = millis();
    uint32_t elapsed = now - lastTime;

    if (elapsed >= LOOP_MS) {
        float dt = elapsed / 1000.0f;
        lastTime = now;

        // Estimate state
        state.update(dt);

        // Control
        rollCtrl.update(state.getRoll(), dt);

        // Telemetry over WebSocket
        telemetry.sendTelemetry(
            state.getRoll(),
            state.getPitch(),
            rollCtrl.pid().getLastError(),
            rollCtrl.pid().getIntegral(),
            rollCtrl.pid().getLastDerivative()
        );
    }
}