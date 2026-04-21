#include <Arduino.h>
#include "StateEstimator.hpp"
#include "RollController.hpp"
#include "DriveController.hpp"
#include "Telemetry.hpp"

// ---- Config ----
constexpr int SERVO_PIN = 18; // GPIO pin connected to servo signal wire
constexpr int STEP_PIN = 25; // GPIO pin connected to A4988 STEP
constexpr int DIR_PIN  = 26; // GPIO pin connected to A4988 DIR
constexpr uint32_t LOOP_MS = 20; // 50 Hz control loop

// ---- Roll PID gains (!!! TO TUNE !!!) ---- 
constexpr float KP_ROLL = 1.0f;
constexpr float KI_ROLL = 0.0f;
constexpr float KD_ROLL = 0.05f;

// ---- Drive PID gains (!!! TO TUNE !!!) ---- 
constexpr float KP_DRIVE = 1.0f;
constexpr float KI_DRIVE = 0.0f;
constexpr float KD_DRIVE = 0.05f;

// Max stepper speed in steps/second
// max robot speed = MAX_STEPPER_SPEED * 0.35m * PI / (200 (steps/rev) * 10 (gear ratio))
constexpr float MAX_STEPPER_SPEED = 400.0f;

// ---- Complementary filter coefficient (!!! TO TUNE !!!) ----
// 0.98 = trust gyro 98% short-term, correct with accel 2% long-term
constexpr float ALPHA = 0.98f;

// ---- WiFi hotspot credentials ----
constexpr char WIFI_SSID[] = "BBM8-body";
constexpr char WIFI_PASS[] = "starwars";

// ---- Module instances ----
StateEstimator state(ALPHA);
RollController rollCtrl(KP_ROLL, KI_ROLL, KD_ROLL, SERVO_PIN);
DriveController driveCtrl(KP_DRIVE, KI_DRIVE, KD_DRIVE, MAX_STEPPER_SPEED, STEP_PIN, DIR_PIN);
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
    driveCtrl.begin();
    Serial.println("[OK] DriveController ready.");

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
        driveCtrl.update(state.getPitch(), dt);

        // Telemetry over WebSocket
        telemetry.sendTelemetry(
            // State Estimator
            state.getRoll(),
            state.getPitch(),
            // PID
            rollCtrl.getTarget(),
            rollCtrl.pid().getLastOutput(),
            rollCtrl.pid().getLastError(),
            rollCtrl.pid().getIntegral(),
            rollCtrl.pid().getLastDerivative()
        );

        // Serial print for debugging
        // Serial.printf("Roll: %6.2f°, Pitch: %6.2f°, Err: %6.2f, Int: %6.2f, Der: %6.2f\n",
        //               state.getRoll(), state.getPitch(),
        //               rollCtrl.pid().getLastError(),
        //               rollCtrl.pid().getIntegral(),
        //               rollCtrl.pid().getLastDerivative());
    }
}