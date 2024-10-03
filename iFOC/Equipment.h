#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "ActuatorSet.h"

class Equipment
{
public:
    Equipment();
private:
    QSharedPointer<ActuatorSet> actuatorProximal;
    QSharedPointer<ActuatorSet> actuatorDistal;
};

#endif // EQUIPMENT_H
