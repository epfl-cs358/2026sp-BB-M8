#pragma once
#include <Arduino.h>
#include <FastAccelStepper.h>

/**
 * DriveController.hpp
 *
 * Controls forward/backward motion of the sphere using a NEMA 17
 * driven by an A4988 via FastAccelStepper (interrupt-driven).
 *
 * Target speed (m/s) is converted directly to steps/s.
 * Pitch is used as a safety limiter to prevent the body from rotating around the main axis.
 *
 * Wiring:
 *   A4988 STEP   → STEP_PIN
 *   A4988 DIR    → DIR_PIN
 *   A4988 ENABLE → ENABLE_PIN (active LOW — HIGH disables coils)
 *   A4988 GND    → GND
 *   A4988 VDD    → 3.3V
 *   A4988 VMOT   → motor supply (8-35V)
 */
class DriveController {
public:
    static constexpr int   STEPS_PER_REV    = 200;
    static constexpr float GEAR_RATIO       = 10.0f / 1.5f;
    static constexpr float SPHERE_DIAMETER_M = 0.35f;

    /**
     * @param maxSpeed  Maximum stepper speed in steps/second
     * @param stepPin   A4988 STEP pin
     * @param dirPin    A4988 DIR pin
     */
    DriveController(float maxSpeed, int stepPin, int dirPin, int enablePin = -1);

    void begin();

    /**
     * Call every control loop iteration.
     * @param pitchDeg  Current pitch from StateEstimator (degrees)
     */
    void update(float pitchDeg);

    void setTargetSpeed(float mPerSec) { _targetMPerSec = mPerSec; }
    float getTargetSpeed() const { return _targetMPerSec; }
    void stopMove() { if (_stepper) _stepper->stopMove(); }
    void runForward() { if (_stepper) _stepper->runForward(); }

    void setPitchLimiterEnabled(bool en) { _pitchLimiterEnabled = en; }
    void setMaxPitchDeg(float deg)       { _maxPitchDeg = deg; }

private:
    FastAccelStepperEngine _engine;
    FastAccelStepper*      _stepper;
    int   _stepPin;
    int   _dirPin;
    int   _enablePin;
    float _maxSpeed;
    float _targetMPerSec;
    float _maxPitchDeg         = 20.0f;
    bool  _pitchLimiterEnabled = false;

    float stepsPerM()                        const;
    float mPerSecToSteps(float mPerSec)      const;
    float applyPitchLimiter(float speedCommand, float pitchDeg);
};