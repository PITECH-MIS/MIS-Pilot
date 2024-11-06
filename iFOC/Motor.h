#ifndef MOTOR_H
#define MOTOR_H

#include "../EtherCAT/ECATWrapper.h"
#include <QSet>
#include <QList>
#include <QSharedPointer>

#define _constrain(v,low,high) ((v)<(low)?(low):((v)>(high)?(high):(v)))
#ifndef PI
#define PI 3.141592653589793238462643383279f
#endif
#define PI2 6.283185307179586476925286766559f
#define DEG2RAD(degree) ((float)(degree) * 0.01745329251994329576923690768489f)
#define RAD2DEG(radian) ((float)(radian) * 57.295779513082320876798154814105f)

#pragma pack(push, 1)
class motor_state_t
{
public:
    uint8_t IsAlive = 0;
    uint8_t Enable = 0;
    uint8_t NodeID = 0;
    uint8_t Mode = 0;
    uint8_t MotorTemp = 0;
    uint8_t MCUTemp = 0;
    uint8_t LimiterState = 0;
    uint16_t ErrorCode = 0;
    uint32_t SN = 0;
    float IqReal = 0.0f;
    float IdReal = 0.0f;
    float Vbus = 0.0f;
    float Ibus = 0.0f;
    float EstimateSpeed = 0.0f;
    float EstimateRawAngle = 0.0f;
    float CurrentLimit = 0.0f;
    bool operator==(const motor_state_t& rhs) const { return memcmp(&IsAlive, &rhs.IsAlive, sizeof(motor_state_t)) == 0; }
    bool operator!=(const motor_state_t& rhs) const { return !(*this == rhs); }
};

class motor_set_t
{
public:
    uint8_t Enable = 0;
    uint8_t Mode = 0;
    float IqSet = 0.0f;
    float SetSpeed = 0.0f;
    float SetRawAngle = 0.0f;
    float SetTrajectoryAngle = 0.0f;
    float SetCurrentLimit = 0.0f;
    bool operator==(const motor_set_t& rhs) const { return memcmp(&Enable, &rhs.Enable, sizeof(motor_set_t)) == 0; }
    bool operator!=(const motor_set_t& rhs) const { return !(*this == rhs); }
};

#pragma pack(pop)

class Motor
{
public:
    enum Mode
    {
        MODE_TORQUE = 0,
        MODE_SPEED,
        MODE_POSITION,
        MODE_TRAJECTORY,
    };
    enum State
    {
        STATE_OFF,
        STATE_ON,
    };

    Motor(uint32_t _SN) : SN(_SN) {};
    Motor(uint32_t _SN, uint8_t _limit) : SN(_SN), limiter_index(_limit) {};
    ~Motor();
    bool findMotor(QHash<uint16_t, ECATSlave*>& slaves);
    void resetState();
    bool setMode(Motor::Mode mode);
    bool setState(Motor::State state);
    bool setSpeed(float rpm);
    bool setTrajAbsAngle(float deg);
    bool setCurrentLimit(float limit);
    bool setTorque(float Iq);
    bool setPosAbsAngle(float deg);
    Motor::Mode getMode();
    Motor::State getState();
    float getSpeed();
    float getPosDeg();
    float getIq();
    float getId();
    float getVBus();
    float getIBus();
    uint16_t getErrorCode();
    uint32_t getSN();
    float getCurrentLimit();
    uint16_t getMCUTemp();
    bool isLimiterActivated();
    bool hasLimiterActivated();
    uint8_t getRawLimiterState();
    bool checkAlive();
    void applyMotorConfig();
private:
    uint32_t SN = 0;
public:
    uint8_t limiter_index = 0;
private:
    motor_state_t *state_ptr = nullptr;
    motor_set_t set;
    motor_set_t *set_ptr = nullptr;
};

QSet<QString> getMotorSN(QHash<uint16_t, ECATSlave*>& slaves);
float normalizeRad(float radian);
float getIncAngleByRPM(float rpm, float dt);

#endif // MOTOR_H
