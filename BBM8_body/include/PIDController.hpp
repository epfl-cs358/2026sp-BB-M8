#pragma once

/**
 * PIDController.hpp
 * Generic, timestep-aware PID controller.
 *
 * Usage:
 *   PIDController pid(kp, ki, kd, outMin, outMax);
 *   float output = pid.compute(setpoint, measurement, dt);
 */
class PIDController {
public:
    /**
     * @param kp      Proportional gain
     * @param ki      Integral gain
     * @param kd      Derivative gain
     * @param outMin  Minimum clamped output
     * @param outMax  Maximum clamped output
     */
    PIDController(float kp, float ki, float kd,
                  float outMin = -90.0f, float outMax = 90.0f);

    /**
     * Compute one PID step.
     * @param setpoint    Desired value
     * @param measurement Measured value
     * @param dt          Time elapsed since last call (in seconds)
     * @return            Control output (clamped to [outMin, outMax])
     */
    float compute(float setpoint, float measurement, float dt);

    /** Reset integrator and derivative state (call on re-enable). */
    void reset();

    /** Live gain tuning without losing integrator state. */
    void setGains(float kp, float ki, float kd);

    /** Adjust output limits at runtime. */
    void setLimits(float outMin, float outMax);

    // --- Getters ---
    float getLastError()      const { return _lastError; }
    float getIntegral()       const { return _integral;  }
    float getLastDerivative() const { return _derivative; }

private:
    float _kp, _ki, _kd;
    float _outMin, _outMax;
    float _integral;
    float _lastError;
    float _derivative;

    float clamp(float value, float minVal, float maxVal);
};