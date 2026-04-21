#pragma once
#include <Arduino.h>
#include <AccelStepper.h>
#include "PIDController.hpp"

/**
 * DriveController.h
 *
 * Controls forward/backward motion of the sphere using a NEMA 17
 * driven by an A4988 via AccelStepper.
 *
 * Speed is controlled by a PID on step velocity.
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
    // TODO: set to match the A4988 MS1/MS2/MS3 jumper config
    // Full=200, Half=400, Quarter=800, Eighth=1600, Sixteenth=3200
    static constexpr int STEPS_PER_REV = 200;
    static constexpr float GEAR_RATIO = 10.0f;

    static constexpr float SPHERE_DIAMETER_M = 0.35f;

    // Pitch safety limit (degrees). If |pitch| exceeds this, speed command is zeroed
    static constexpr float MAX_PITCH_DEG = 15.0f;

    /**
     * @param kp, ki, kd   Speed PID gains
     * @param maxSpeed     Maximum stepper speed in steps/second
     */
    DriveController(float kp, float ki, float kd, float maxSpeed, int stepPin, int dirPin);

    /** Call once in setup(). */
    void begin();

    /**
     * Call every control loop iteration.
     * @param pitchDeg   Current pitch from StateEstimator (degrees)
     * @param dt         Seconds since last call
     */
    void update(float pitchDeg, float dt);

    /** Set desired forward speed in m/s. Negative = backward. */
    void setTargetSpeed(float mPerSec) { _targetMPerSec = mPerSec; }

    float getTargetSpeed()  const { return _targetMPerSec; }
    float getCurrentSpeed() const { return _currentMPerSec; }
    float getLastOutput()   const { return _lastOutput; }

    PIDController& pid() { return _pid; }

private:
    PIDController _pid;
    AccelStepper  _stepper;

    int _stepPin;
    int _dirPin;
    float _maxSpeed;         // steps/second
    float _targetMPerSec;
    float _currentMPerSec;
    float _lastOutput;       // last speed command after pitch limiting (steps/s)

    // Conversion helpers
    float stepsPerM()          const;
    float stepsToMPerSec(float stepsPerSec) const;
    float mPerSecToSteps(float mPerSec)      const;

    float applyPitchLimiter(float speedCommand, float pitchDeg);
};