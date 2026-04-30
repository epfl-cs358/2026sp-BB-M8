#include "DriveController.hpp"

DriveController::DriveController(float maxSpeed, int stepPin, int dirPin)
    : _stepper(AccelStepper::DRIVER, stepPin, dirPin),
      _maxSpeed(maxSpeed),
      _targetMPerSec(0.0f)
{}

void DriveController::begin() {
    _stepper.setMaxSpeed(_maxSpeed);
    _stepper.setSpeed(0.0f);
}

void DriveController::update(float pitchDeg) {
    float targetStepsPerSec = mPerSecToSteps(_targetMPerSec);
    float limited = applyPitchLimiter(targetStepsPerSec, pitchDeg);
    _stepper.setSpeed(limited);
}

void DriveController::tick() {
    _stepper.runSpeed();
}

float DriveController::stepsPerM() const {
    return (STEPS_PER_REV * GEAR_RATIO) / (SPHERE_DIAMETER_M * M_PI);
}

float DriveController::mPerSecToSteps(float mPerSec) const {
    return mPerSec * stepsPerM();
}

float DriveController::applyPitchLimiter(float speedCommand, float pitchDeg) {
    float factor = constrain(1.0f - abs(pitchDeg) / MAX_PITCH_DEG, 0.0f, 1.0f);
    return speedCommand * factor;
}
