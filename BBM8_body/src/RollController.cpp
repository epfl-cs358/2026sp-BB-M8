#include "RollController.hpp"

RollController::RollController(float kp, float ki, float kd, int servoPin)
    : _pid(kp, ki, kd, -90.0f, 90.0f),
      _servoPin(servoPin),
      _targetDeg(0.0f)
{}

void RollController::begin() {
    _servo.attach(_servoPin);
    _servo.write(90);  // Neutral position on startup
}

void RollController::update(float rollDeg, float dt) {
    float correction = _pid.compute(_targetDeg, rollDeg, dt);

    // PID output is -90..+90 deg correction
    // servo.write() expects 0..180 (90 = neutral)
    int servoAngle = constrain(90 + (int)correction, 0, 180);
    _servo.write(servoAngle);
}