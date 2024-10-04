#ifndef EQUIPMENT6DOF_H
#define EQUIPMENT6DOF_H

#include "Actuator3DoF.h"

class Equipment6DoF
{
public:
    Equipment6DoF();
private:
    QSharedPointer<Actuator3DoF> actuatorProximal;
    QSharedPointer<Actuator3DoF> actuatorDistal;
};

#endif // EQUIPMENT6DOF_H
