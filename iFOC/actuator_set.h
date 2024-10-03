#ifndef ACTUATOR_SET_H
#define ACTUATOR_SET_H

#include "motor.h"

class ActuatorSet
{
public:
    ActuatorSet();

private:
    QSharedPointer<Motor> motorRotation;
    QSharedPointer<Motor> motorPushPull;
    QSharedPointer<Motor> motorLinear;
};

#endif // ACTUATOR_SET_H
