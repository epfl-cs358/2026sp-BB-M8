#pragma once
#include <stdint.h>

enum class PacketType : uint8_t { TELEMETRY = 0x01, COMMAND = 0x02 };

// Body → Head at 10 Hz 
struct __attribute__((packed)) TelemetryPacket {
    PacketType type = PacketType::TELEMETRY;
    float roll, pitch;
    float rollTarget, rollOutput, rollErr, rollIntegral, rollDerivative;
    float driveSpeed;
};

// Head → Body on browser command 
struct __attribute__((packed)) CommandPacket {
    PacketType type    = PacketType::COMMAND;
    uint8_t    cmdType;
    float      target;
    float      rollKp, rollKi, rollKd;
    float      driveSpeed;
    uint8_t    stop;
};

constexpr uint8_t CMD_TARGET        = 0;
constexpr uint8_t CMD_ROLL_GAINS    = 1;
constexpr uint8_t CMD_DRIVE_SPEED   = 2;
constexpr uint8_t CMD_STOP          = 3;
// Pitch limiter: stop field = enabled (0/1), target field = maxPitchDeg
constexpr uint8_t CMD_PITCH_LIMITER = 4;
