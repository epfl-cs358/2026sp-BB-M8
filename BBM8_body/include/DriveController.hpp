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
 *   A4988 STEP → STEP_PIN
 *   A4988 DIR  → DIR_PIN
 *   A4988 GND  → GND
 *   A4988 VDD  → 3.3V
 *   A4988 VMOT → motor supply (8-35V)
 */
class DriveController {
public:
    static constexpr int   STEPS_PER_REV    = 200;
    static constexpr float GEAR_RATIO       = 10.0f;
    static constexpr float SPHERE_DIAMETER_M = 0.35f;

    // Pitch safety limit (degrees). If |pitch| exceeds this, speed command is zeroed
    static constexpr float MAX_PITCH_DEG = 45.0f;

    /**
     * @param maxSpeed  Maximum stepper speed in steps/second
     * @param stepPin   A4988 STEP pin
     * @param dirPin    A4988 DIR pin
     */
    DriveController(float maxSpeed, int stepPin, int dirPin);

    void begin();

    /**
     * Call every control loop iteration.
     * @param pitchDeg  Current pitch from StateEstimator (degrees)
     */
    void update(float pitchDeg);

    void setTargetSpeed(float mPerSec) { _targetMPerSec = mPerSec; }
    float getTargetSpeed() const { return _targetMPerSec; }
    void forceStop() { if (_stepper) _stepper->forceStop(); }

private:
    FastAccelStepperEngine _engine;
    FastAccelStepper*      _stepper;
    int   _stepPin;
    int   _dirPin;
    float _maxSpeed;
    float _targetMPerSec;

    float stepsPerM()                        const;
    float mPerSecToSteps(float mPerSec)      const;
    float applyPitchLimiter(float speedCommand, float pitchDeg);
};
