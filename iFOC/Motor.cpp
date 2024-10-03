#include "Motor.h"

// Motor::Motor(uint32_t _SN) {}

bool Motor::findMotorInVector(QVector<slave_inputs_t*>& input, QVector<slave_outputs_t*>& output)
{
    if(input.size() != output.size()) return false;
    for(int i = 0; i < input.size(); i++)
    {
        motor_state_t* state_start = reinterpret_cast<motor_state_t*>(&input.at(i)->Motor1_State);
        motor_set_t* set_start = reinterpret_cast<motor_set_t*>(&output.at(i)->Motor1_Set);
        for(uint8_t j = 0; j < 8; j++)
        {
            if(state_start->SN == SN)
            {
                state_ptr = state_start;
                set_ptr = set_start;
                return true;
            }
            state_start++;
            set_start++;
        }
    }
    return false;
}

bool Motor::checkAlive()
{
    return state_ptr && (state_ptr->IsAlive) && (getSN() == SN);
}

void Motor::resetState()
{
    // if(set_ptr)
    {
        memset(&set, 0, sizeof(motor_set_t));
    }
}

bool Motor::setMode(Motor::Mode mode)
{
    if(!checkAlive()) return false;
    set.Mode = (uint8_t)mode;
    return true;
}

bool Motor::setState(Motor::State state)
{
    if(!checkAlive()) return false;
    set.Enable = (uint8_t)state;
    // if(state && getCurrentLimit() <= 0.0f) setCurrentLimit(0.5f);
    return true;
}

bool Motor::setSpeed(float rpm)
{
    if(!checkAlive()) return false;
    set.SetSpeed = rpm;
    return true;
}

bool Motor::setTrajAbsAngle(float deg)
{
    if(!checkAlive()) return false;
    set.SetTrajectoryAngle = deg;
    return true;
}

bool Motor::setCurrentLimit(float limit)
{
    if(!checkAlive()) return false;
    set.SetCurrentLimit = limit;
    return true;
}

bool Motor::setTorque(float Iq)
{
    if(!checkAlive()) return false;
    set.IqSet = Iq;
    return true;
}

bool Motor::setPosAbsAngle(float deg)
{
    if(!checkAlive()) return false;
    set.SetRawAngle = deg;
    return true;
}

Motor::Mode Motor::getMode()
{
    return (Motor::Mode)state_ptr->Mode;
}

Motor::State Motor::getState()
{
    return (Motor::State)state_ptr->Enable;
}

float Motor::getSpeed()
{
    return state_ptr->EstimateSpeed;
}

float Motor::getPosDeg()
{
    return state_ptr->EstimateRawAngle;
}

float Motor::getIq()
{
    return state_ptr->IqReal;
}

float Motor::getId()
{
    return state_ptr->IdReal;
}

float Motor::getVBus()
{
    return state_ptr->Vbus;
}

float Motor::getIBus()
{
    return state_ptr->Ibus;
}

uint16_t Motor::getErrorCode()
{
    return state_ptr->ErrorCode;
}

float Motor::getCurrentLimit()
{
    return state_ptr->CurrentLimit;
}

uint16_t Motor::getMCUTemp()
{
    return state_ptr->MCUTemp;
}

uint8_t Motor::getRawLimiterState()
{
    return state_ptr->LimiterState;
}

uint32_t Motor::getSN()
{
    return state_ptr->SN;
}

void Motor::applyMotorConfig()
{
    if(set_ptr) memcpy(set_ptr, &set, sizeof(motor_set_t));
}

bool Motor::isLimiterActivated()
{
    if(limiter_index == 0) return false;
    if(limiter_index == 1)
    {
        return state_ptr->LimiterState & (1 << 0);
    }
    else
    {
        return state_ptr->LimiterState & (1 << 2);
    }
}

bool Motor::hasLimiterActivated()
{
    if(limiter_index == 0) return false;
    if(limiter_index == 1)
    {
        return state_ptr->LimiterState & (1 << 1);
    }
    else
    {
        return state_ptr->LimiterState & (1 << 3);
    }
}

QSet<QString> getMotorSN(QVector<slave_inputs_t*>& input_vector)
{
    QSet<QString> result;
    for(const auto &i : input_vector)
    {
        motor_state_t* state_start = (motor_state_t*) &i->Motor1_State;
        for(uint8_t j = 0; j < 8; j++)
        {
            if(state_start->SN != 0)
            {
                result.insert(QString::number(state_start->SN));
            }
            state_start++;
        }
    }
    return result;
}
