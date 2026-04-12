#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <math.h>

/**
 * StateEstimator.h
 *
 * Reads raw data from an MPU-9250 over I2C and computes:
 *   - Roll and pitch (degrees) via a complementary filter
 *     (accel for long-term accuracy, gyro for short-term precision)
 *
 * Wiring (body ESP32):
 *   VCC -> 3.3V
 *   GND -> GND
 *   SCL -> GPIO 22 (default SCL pin on ESP32)
 *   SDA -> GPIO 21 (default SDA pin on ESP32)
 */
class StateEstimator {
public:
    static constexpr uint8_t MPU_ADDR = 0x68;

    /**
     * @param alpha  Complementary filter coefficient (0–1).
     *               Higher = trust gyro more, less accel correction.
     *               0.98 is a good starting point.
     */
    explicit StateEstimator(float alpha = 0.98f);

    /**
     * Initialise I2C and wake the MPU-9250.
     * Call once in setup().
     * @return true if the sensor was found and woken successfully.
     */
    bool begin();

    /**
     * Read the IMU and update roll/pitch estimates.
     * @param dt  Seconds since last call. Must be > 0.
     */
    void update(float dt);

    // --- Getters ---
    float getRoll()  const { return _roll;  }
    float getPitch() const { return _pitch; }

    float getAccelX() const { return _ax_g; }
    float getAccelY() const { return _ay_g; }
    float getAccelZ() const { return _az_g; }

    float getGyroX() const { return _gx_ds; }
    float getGyroY() const { return _gy_ds; }
    float getGyroZ() const { return _gz_ds; }

private:
    float _alpha;   // Complementary filter weight

    // Estimated angles
    float _roll;
    float _pitch;

    // Cached raw values 
    float _ax_g, _ay_g, _az_g;       // Accel in g
    float _gx_ds, _gy_ds, _gz_ds;    // Gyro in degrees/s

    // MPU-9250 register addresses
    static constexpr uint8_t REG_PWR_MGMT_1 = 0x6B;
    static constexpr uint8_t REG_ACCEL_XOUT = 0x3B;
    static constexpr uint8_t REG_GYRO_XOUT  = 0x43;

    // Sensor scaling
    // Accel: ±2g range  -> LSB sensitivity = 16384 LSB/g
    // Gyro:  ±250°/s range -> LSB sensitivity = 131 LSB/(°/s)
    static constexpr float ACCEL_SCALE = 16384.0f;
    static constexpr float GYRO_SCALE  = 131.0f;

    void readAccel();
    void readGyro();
};