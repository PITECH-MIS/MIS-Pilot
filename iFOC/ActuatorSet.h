#ifndef ACTUATORSET_H
#define ACTUATORSET_H

#include "Motor.h"

class ActuatorSet
{
public:
    ActuatorSet();

private:
    QSharedPointer<Motor> motorRotation;
    QSharedPointer<Motor> motorPushPull;
    QSharedPointer<Motor> motorLinear;
};

#endif // ACTUATORSET_H
