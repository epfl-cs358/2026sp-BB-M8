#include <Arduino.h>
#include "StateEstimator.hpp"
#include "RollController.hpp"
#include "DriveController.hpp"
#include "Telemetry.hpp"

// ---- Config ----
constexpr int SERVO_PIN = 18;
constexpr int STEP_PIN = 25;
constexpr int DIR_PIN  = 26;
constexpr int ENABLE_PIN = 17;
constexpr uint32_t CONTROL_LOOP_MS = 50;       // 20 Hz control loop
constexpr uint32_t TELEMETRY_INTERVAL_MS = 100; // 10 Hz telemetry rate

// ---- Roll PID gains (!!! TO TUNE !!!) ----
constexpr float KP_ROLL = 0.9f;
constexpr float KI_ROLL = 0.0f;
constexpr float KD_ROLL = 0.0f;

// Max stepper speed in steps/second
// max robot speed = MAX_STEPPER_SPEED / ((STEPS_PER_REV * GEAR_RATIO) / (SPHERE_DIAMETER_M * PI))
//                 = 1300 / ((200 * 6.667) / (0.35 * PI)) ≈ 1.07 m/s
constexpr float MAX_STEPPER_SPEED = 1300.0f;

// ---- Complementary filter coefficient (!!! TO TUNE !!!) ----
// 0.98 = trust gyro 98% short-term, correct with accel 2% long-term
constexpr float ALPHA = 0.98f;

// ---- Safety ----
volatile bool STOP = true; // Start in stopped mode for safe start

// ---- Module instances ----
StateEstimator state(ALPHA);
RollController rollCtrl(KP_ROLL, KI_ROLL, KD_ROLL, SERVO_PIN);
DriveController driveCtrl(MAX_STEPPER_SPEED, STEP_PIN, DIR_PIN, ENABLE_PIN);
Telemetry      telemetry;

// ---- Telemetry snapshot (Core 1 writes, Core 0 reads) ----
struct TelSnap {
    float roll, pitch, rollTarget, rollOutput, rollErr, rollIntegral, rollDerivative, driveSpeed;
} telSnap;
portMUX_TYPE snapMux = portMUX_INITIALIZER_UNLOCKED;

// ---- Core 1: real-time control (20 Hz) ----
void controlTask(void*) {
    TickType_t lastWake = xTaskGetTickCount();
    constexpr float dt = CONTROL_LOOP_MS / 1000.0f;

    while (true) {
        state.update(dt);

        if (!STOP) {
            rollCtrl.update(state.getRoll(), dt);
            driveCtrl.update(state.getPitch());
        } else {
            driveCtrl.setTargetSpeed(0.0f);
            driveCtrl.stopMove();
            // Serial.println("---- STOPPED ----");
        }

        portENTER_CRITICAL(&snapMux);
        telSnap = {
            state.getRoll(),
            state.getPitch(),
            rollCtrl.getTarget(),
            rollCtrl.pid().getLastOutput(),
            rollCtrl.pid().getLastError(),
            rollCtrl.pid().getIntegral(),
            rollCtrl.pid().getLastDerivative(),
            driveCtrl.getTargetSpeed()
        };
        portEXIT_CRITICAL(&snapMux);

        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(CONTROL_LOOP_MS));
    }
}

// ---- Core 0: telemetry send (10 Hz), co-located with WiFi stack ----
void telemetryTask(void*) {
    TickType_t lastWake = xTaskGetTickCount();

    while (true) {
        TelSnap snap;
        portENTER_CRITICAL(&snapMux);
        snap = telSnap;
        portEXIT_CRITICAL(&snapMux);

        Serial.println("---- sending telemetry ----");
        telemetry.sendTelemetry(
            snap.roll, snap.pitch,
            snap.rollTarget, snap.rollOutput,
            snap.rollErr, snap.rollIntegral,
            snap.rollDerivative, snap.driveSpeed
        );

        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(TELEMETRY_INTERVAL_MS));
    }
}

void setup() {
    Serial.begin(115200);

    while (!state.begin()) {
        Serial.println("[ERROR] MPU-9250 not found. Check wiring.");
    }
    Serial.println("[OK] MPU-9250 initialised.");

    rollCtrl.begin();
    Serial.println("[OK] RollController ready.");
    driveCtrl.begin();
    Serial.println("[OK] DriveController ready.");

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
    telemetry.onPitchLimiterChanged([](bool enabled, float maxDeg) {
        driveCtrl.setPitchLimiterEnabled(enabled);
        driveCtrl.setMaxPitchDeg(maxDeg);
    });
    telemetry.begin();

    xTaskCreatePinnedToCore(controlTask,   "ControlTask",   4096, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(telemetryTask, "TelemetryTask", 4096, NULL, 3, NULL, 0);
    vTaskDelete(NULL); // delete Arduino init task — tasks are now running
}

void loop() {}
