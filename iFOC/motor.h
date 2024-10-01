#ifndef MOTOR_H
#define MOTOR_H

#include "ecatwrapper.h"
#include <QSet>
#include <QList>

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

    Motor(QVector<slave_inputs_t*>& i, QVector<slave_outputs_t*>& o, uint32_t _SN) : input(i), output(o), SN(_SN) {};
    Motor(QVector<slave_inputs_t*>& i, QVector<slave_outputs_t*>& o, uint32_t _SN, uint8_t _limit) : input(i), output(o), SN(_SN), limiter_index(_limit) {};
    bool findMotorInVector();
    void resetState();
    bool setMode(Motor::Mode mode);
    bool setState(Motor::State state);
    bool setSpeed(float rpm);
    bool setTrajAbsAngle(float deg);
    bool setCurrentLimit(float limit);
    Motor::Mode getMode();
    Motor::State getState();
    float getSpeed();
    float getPosDeg();
    float getIq();
    float getVBus();
    uint16_t getErrorCode();
    float getCurrentLimit();
    bool isLimiterActivated();
    bool hasLimiterActivated();
    void applyMotorConfig();
private:
    QVector<slave_inputs_t*>& input;
    QVector<slave_outputs_t*>& output;
    uint32_t SN = 0;
    uint8_t limiter_index = 0;
    motor_state_t *state_ptr = nullptr;
    motor_set_t set;
    motor_set_t *set_ptr = nullptr;
    bool checkAlive();
};

QSet<QString> getMotorSN(QVector<slave_inputs_t*>& input_vector);

#endif // MOTOR_H
