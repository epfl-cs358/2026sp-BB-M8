#include "DriveController.hpp"

DriveController::DriveController(float maxSpeed, int stepPin, int dirPin, int enablePin)
    : _engine(), _stepper(nullptr),
      _stepPin(stepPin), _dirPin(dirPin), _enablePin(enablePin),
      _maxSpeed(maxSpeed), _targetMPerSec(0.0f)
{}

void DriveController::begin() {
    _engine.init();
    _stepper = _engine.stepperConnectToPin(_stepPin);
    if (_stepper == nullptr) {
        Serial.println("[ERROR] DriveController: stepperConnectToPin failed");
        return;
    }
    _stepper->setDirectionPin(_dirPin);
    if (_enablePin >= 0) {
        _stepper->setEnablePin(static_cast<uint8_t>(_enablePin)); // active LOW (A4988 default)
        _stepper->setAutoEnable(true); // energize on move, de-energize when stopped
    }
    _stepper->setAcceleration(1500);
    _stepper->setSpeedInHz(0); // Intialized with zero speed
}

void DriveController::update(float pitchDeg) {
    if (_stepper == nullptr) return;

    float targetStepsPerSec = mPerSecToSteps(_targetMPerSec);
    // Apply pitch limiter
    float limited = applyPitchLimiter(targetStepsPerSec, pitchDeg);
    // Constrain to max speed
    if (limited >  _maxSpeed) limited =  _maxSpeed;
    if (limited < -_maxSpeed) limited = -_maxSpeed;

    if (fabs(limited) <= 20.0f) {
        _stepper->stopMove();
        return;
    }

    uint32_t absHz = static_cast<uint32_t>(fabsf(limited));
    _stepper->setSpeedInHz(absHz);
    if (limited > 0.0f)
        _stepper->runForward();
    else
        _stepper->runBackward();
}

float DriveController::stepsPerM() const {
    return (STEPS_PER_REV * GEAR_RATIO) / (SPHERE_DIAMETER_M * M_PI);
}

float DriveController::mPerSecToSteps(float mPerSec) const {
    return mPerSec * stepsPerM();
}

float DriveController::applyPitchLimiter(float speedCommand, float pitchDeg) {
    if (!_pitchLimiterEnabled) return speedCommand;
    float factor = constrain(1.0f - abs(pitchDeg) / _maxPitchDeg, 0.0f, 1.0f);
    return speedCommand * factor;
}