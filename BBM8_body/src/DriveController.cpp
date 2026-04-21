#include "DriveController.hpp"

DriveController::DriveController(float kp, float ki, float kd, float maxSpeed)
    : _pid(kp, ki, kd, -maxSpeed, maxSpeed),
      _stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN),
      _maxSpeed(maxSpeed),
      _targetMPerSec(0.0f),
      _currentMPerSec(0.0f),
      _lastOutput(0.0f)
{}

void DriveController::begin() {
    _stepper.setMaxSpeed(_maxSpeed);
    // High acceleration — the pitch limiter is what actually limits acceleration
    _stepper.setAcceleration(_maxSpeed * 2.0f);
    _stepper.setSpeed(0.0f);
}

void DriveController::update(float pitchDeg, float dt) {
    // 1. Estimate current speed from stepper step rate (steps/s → mm/s)
    _currentMPerSec = stepsToMPerSec(_stepper.speed());

    // 2. Speed PID: target vs current speed
    float targetStepsPerSec = mPerSecToSteps(_targetMPerSec);
    float currentStepsPerSec = _stepper.speed();
    float rawCommand = _pid.compute(targetStepsPerSec, currentStepsPerSec, dt);

    // 3. Pitch safety limiter: scale command down as pitch grows
    float limitedCommand = applyPitchLimiter(rawCommand, pitchDeg);
    _lastOutput = limitedCommand;

    // 4. Command stepper
    _stepper.setSpeed(limitedCommand);

    // 5. Must be called every loop — generates the actual STEP pulses
    _stepper.runSpeed();
}

// ── Private ──

float DriveController::stepsPerM() const {
    float circumference = SPHERE_DIAMETER_M * M_PI;
    return (STEPS_PER_REV * GEAR_RATIO) / circumference;
}

float DriveController::stepsToMPerSec(float stepsPerSec) const {
    return stepsPerSec / stepsPerM();
}

float DriveController::mPerSecToSteps(float mPerSec) const {
    return mPerSec * stepsPerM();
}

float DriveController::applyPitchLimiter(float speedCommand, float pitchDeg) {
    // Linear scale: full command at pitch=0, zero command at pitch=MAX_PITCH_DEG
    float absPitch = abs(pitchDeg);
    float factor = constrain(1.0f - absPitch / MAX_PITCH_DEG, 0.0f, 1.0f);
    return speedCommand * factor;
}