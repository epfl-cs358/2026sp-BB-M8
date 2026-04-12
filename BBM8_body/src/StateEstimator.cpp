#include "StateEstimator.hpp"

StateEstimator::StateEstimator(float alpha)
    : _alpha(alpha),
      _roll(0.0f), _pitch(0.0f),
      _ax_g(0.0f), _ay_g(0.0f), _az_g(0.0f),
      _gx_ds(0.0f), _gy_ds(0.0f), _gz_ds(0.0f)
{}

bool StateEstimator::begin() {
    Wire.begin();  // Uses default SDA=21, SCL=22 on ESP32

    // Check sensor is present on the bus
    Wire.beginTransmission(MPU_ADDR);
    if (Wire.endTransmission() != 0) {
        return false;  // Nothing responded at that address
    }

    // Wake up MPU-9250 
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(REG_PWR_MGMT_1);
    Wire.write(0x00);
    Wire.endTransmission(true);

    // Short settle time after wake-up
    delay(100);

    // Seed filter with static accel reading so first update() doesn't start from 0 degrees
    readAccel();
    _roll  = atan2(_ay_g, sqrt(_ax_g * _ax_g + _az_g * _az_g)) * 180.0f / M_PI;
    _pitch = atan2(_ax_g, sqrt(_ay_g * _ay_g + _az_g * _az_g)) * 180.0f / M_PI;

    return true;
}

void StateEstimator::update(float dt) {
    if (dt <= 0.0f) return;

    readAccel();
    readGyro();

    // Accelerometer-only angle estimates (noisy but drift-free)
    float accelRoll  = atan2(_ay_g, sqrt(_ax_g * _ax_g + _az_g * _az_g)) * 180.0f / M_PI;
    float accelPitch = atan2(_ax_g, sqrt(_ay_g * _ay_g + _az_g * _az_g)) * 180.0f / M_PI;

    // Complementary filter:
    // trust gyro short-term, correct long-term drift with accel
    _roll  = _alpha * (_roll  + _gx_ds * dt) + (1.0f - _alpha) * accelRoll;
    _pitch = _alpha * (_pitch + _gy_ds * dt) + (1.0f - _alpha) * accelPitch;
}

// --- Private: raw reads ---

void StateEstimator::readAccel() {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(REG_ACCEL_XOUT);  // 0x3B - same as your working code
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 6, true);  // matches your working code signature

    int16_t ax = Wire.read() << 8 | Wire.read();
    int16_t ay = Wire.read() << 8 | Wire.read();
    int16_t az = Wire.read() << 8 | Wire.read();

    _ax_g = ax / ACCEL_SCALE;
    _ay_g = ay / ACCEL_SCALE;
    _az_g = az / ACCEL_SCALE;
}

void StateEstimator::readGyro() {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(REG_GYRO_XOUT);  // 0x43 - gyro registers follow same layout
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 6, true);

    int16_t gx = Wire.read() << 8 | Wire.read();
    int16_t gy = Wire.read() << 8 | Wire.read();
    int16_t gz = Wire.read() << 8 | Wire.read();

    _gx_ds = gx / GYRO_SCALE;
    _gy_ds = gy / GYRO_SCALE;
    _gz_ds = gz / GYRO_SCALE;
}