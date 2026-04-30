#include <Arduino.h>
#include "StateEstimator.hpp"
#include "RollController.hpp"
#include "DriveController.hpp"
#include "Telemetry.hpp"

// ---- Config ----
constexpr int SERVO_PIN = 18; // GPIO pin connected to servo signal wire
constexpr int STEP_PIN = 25; // GPIO pin connected to A4988 STEP
constexpr int DIR_PIN  = 26; // GPIO pin connected to A4988 DIR
constexpr uint32_t CONTROL_LOOP_MS = 20; // 50 Hz control loop
constexpr uint32_t TELEMETRY_INTERVAL_MS = 100; // 10 Hz telemetry rate

// ---- Roll PID gains (!!! TO TUNE !!!) ---- 
constexpr float KP_ROLL = 0.9f;
constexpr float KI_ROLL = 0.05f;
constexpr float KD_ROLL = 0.1f;

// Max stepper speed in steps/second
// max robot speed = MAX_STEPPER_SPEED * 0.35m * PI / (200 (steps/rev) * 10 (gear ratio))
constexpr float MAX_STEPPER_SPEED = 2000.0f;

// ---- Complementary filter coefficient (!!! TO TUNE !!!) ----
// 0.98 = trust gyro 98% short-term, correct with accel 2% long-term
constexpr float ALPHA = 0.98f;

// ---- WiFi hotspot credentials ----
constexpr char WIFI_SSID[] = "BBM8-body";
constexpr char WIFI_PASS[] = "starwars";

// ---- Safety ----
bool STOP = false; // Set to true to stop the robot

// ---- Module instances ----
StateEstimator state(ALPHA);
RollController rollCtrl(KP_ROLL, KI_ROLL, KD_ROLL, SERVO_PIN);
DriveController driveCtrl(MAX_STEPPER_SPEED, STEP_PIN, DIR_PIN);
Telemetry      telemetry(WIFI_SSID, WIFI_PASS);

uint32_t lastControlTime = 0;
uint32_t lastTelemetryTime = 0;

void setup() {
    Serial.begin(115200);

    while (!state.begin()){
        Serial.println("[ERROR] MPU-9250 not found. Check wiring.");
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
    telemetry.onRollGainsChanged([](float kp, float ki, float kd) {
        rollCtrl.pid().setGains(kp, ki, kd);
    });
    telemetry.onDriveSpeedChanged([](float mPerSec) {
        driveCtrl.setTargetSpeed(mPerSec);
    });
    telemetry.onStopChanged([](bool stop) {
        STOP = stop;
    });
    // Start the WiFi only after registering the callbacks
    telemetry.begin();

    lastControlTime = millis();
    lastTelemetryTime = millis();
}

void loop() {
    // Handle incoming WebSocket messages
    telemetry.update();

    // Step the motor on every loop iteration for smooth motion
    driveCtrl.tick();

    uint32_t now     = millis();

    uint32_t elapsedControl = now - lastControlTime;
    // Control loop 
    if (elapsedControl >= CONTROL_LOOP_MS && !STOP) {
        float dt = elapsedControl / 1000.0f;
        lastControlTime = now;

        // Estimate state
        state.update(dt);

        // Control
        rollCtrl.update(state.getRoll(), dt);
        driveCtrl.update(state.getPitch());

    } else if (STOP) {
        Serial.println("---- STOPPED ----");
        lastControlTime = now;
    }

    uint32_t elapsedTelemetry = now - lastTelemetryTime;
    // Telemetry loop
    if (elapsedTelemetry >= TELEMETRY_INTERVAL_MS) {
        lastTelemetryTime = now;
        Serial.println("---- sending telemetry ----");
        // Telemetry over WebSocket
        telemetry.sendTelemetry(
            state.getRoll(),
            state.getPitch(),
            rollCtrl.getTarget(),
            rollCtrl.pid().getLastOutput(),
            rollCtrl.pid().getLastError(),
            rollCtrl.pid().getIntegral(),
            rollCtrl.pid().getLastDerivative(),
            driveCtrl.getTargetSpeed()
        );

        // Serial print for debugging
        // Serial.printf("Roll: %6.2f°, Pitch: %6.2f°, Err: %6.2f, Int: %6.2f, Der: %6.2f\n",
        //               state.getRoll(), state.getPitch(),
        //               rollCtrl.pid().getLastError(),
        //               rollCtrl.pid().getIntegral(),
        //               rollCtrl.pid().getLastDerivative());
    }
}