#ifndef ACTUATOR3DOF_H
#define ACTUATOR3DOF_H

#include "Motor.h"

class Actuator3DoF
{
public:
    Actuator3DoF();

private:
    QSharedPointer<Motor> motorRotation;
    QSharedPointer<Motor> motorPushPull;
    QSharedPointer<Motor> motorLinear;
};

#endif // ACTUATOR3DOF_H
