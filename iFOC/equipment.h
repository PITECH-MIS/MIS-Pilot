#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "actuator_set.h"

class Equipment
{
public:
    Equipment();
private:
    QSharedPointer<ActuatorSet> actuatorProximal;
    QSharedPointer<ActuatorSet> actuatorDistal;
};

#endif // EQUIPMENT_H
