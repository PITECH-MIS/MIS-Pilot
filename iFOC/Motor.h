#ifndef MOTOR_H
#define MOTOR_H

#include "ECATWrapper.h"
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
typedef struct motor_state_t
{
    uint8_t IsAlive;
    uint8_t Enable;
    uint8_t Mode;
    uint8_t MotorTemp;
    uint8_t MCUTemp;
    uint8_t LimiterState;
    uint16_t ErrorCode;
    uint32_t SN;
    float IqReal;
    float IdReal;
    float IqSet;
    float IdSet;
    float Vbus;
    float Ibus;
    float EstimateSpeed;
    float EstimateRawAngle;
    float CurrentLimit;
}motor_state_t;

typedef struct motor_set_t
{
    uint8_t Enable;
    uint8_t Identify;
    uint8_t Mode;
    float IqSet;
    float IdSet;
    float SetSpeed;
    float SetRawAngle;
    float SetTrajectoryAngle;
    float SetCurrentLimit;
}motor_set_t;

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
    bool findMotorInVector(QVector<slave_inputs_t*>& input, QVector<slave_outputs_t*>& output);
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

QSet<QString> getMotorSN(QVector<slave_inputs_t*>& input_vector);
float normalizeRad(float radian);
float getIncAngleByRPM(float rpm, float dt);

#endif // MOTOR_H
