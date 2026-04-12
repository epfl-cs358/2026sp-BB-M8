#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>
#include "PIDController.hpp"

/**
 * RollController.h
 *
 * Receives a roll angle from the StateEstimator and drives
 * a servo to stabilize or steer the robot.
 *
 * PID output: -90..+90 degrees correction
 * Servo input: 0..180 degrees (90 = neutral)
 */
class RollController {
public:
    /**
     * @param kp, ki, kd  PID gains
     * @param servoPin    GPIO pin connected to servo signal wire
     */
    RollController(float kp, float ki, float kd, int servoPin);

    /** Call once in setup(). */
    void begin();

    /**
     * Compute PID correction and update servo.
     * @param rollDeg  Current roll from StateEstimator, in degrees
     * @param dt       Seconds since last call
     */
    void update(float rollDeg, float dt);

    /** Set desired roll angle (default: 0 = level). */
    void setTarget(float targetDeg) { _targetDeg = targetDeg; }
    float getTarget() const { return _targetDeg; }

    /** Access inner PID for tuning or telemetry. */
    PIDController& pid() { return _pid; }

private:
    PIDController _pid;
    Servo         _servo;
    int           _servoPin;
    float         _targetDeg;
};