#include "PIDController.hpp"

PIDController::PIDController(float kp, float ki, float kd,
                             float outMin, float outMax)
    : _kp(kp), _ki(ki), _kd(kd),
      _outMin(outMin), _outMax(outMax),
      _integral(0.0f), _lastError(0.0f), _derivative(0.0f)
{}

float PIDController::compute(float setpoint, float measurement, float dt) {
    if (dt <= 0.0f) return 0.0f;  // Guard against bad timestep

    const float error = setpoint - measurement;

    // -- Proportional --
    const float P = _kp * error;

    // -- Integral (with anti-windup clamping) --
    _integral += error * dt;
    if (_ki != 0.0f) {
        _integral = clamp(_integral, _outMin / _ki, _outMax / _ki);
    }else{
        _integral = 0.0f;  // No integral if  ki = 0
    }
    const float I = _ki * _integral;

    // Back-calculate clamped integral state
    if (_ki != 0.0f) _integral = I / _ki;

    // -- Derivative (on measurement) --
    _derivative = (measurement - (_lastError == 0.0f ? measurement : (setpoint - _lastError))) / dt;
    
    const float D = _kd * _derivative;

    _lastError = error;

    float output = clamp(P + I + D, _outMin, _outMax);

    _lastOutput = output;

    return output;
}

void PIDController::reset() {
    _integral   = 0.0f;
    _lastError  = 0.0f;
    _derivative = 0.0f;
}

void PIDController::setGains(float kp, float ki, float kd) {
    _kp = kp;
    _ki = ki;
    _kd = kd;
}

void PIDController::setLimits(float outMin, float outMax) {
    _outMin = outMin;
    _outMax = outMax;
}

float PIDController::clamp(float value, float minVal, float maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}