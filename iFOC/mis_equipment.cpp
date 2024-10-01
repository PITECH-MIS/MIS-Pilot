#include "mis_equipment.h"

void MIS_Equipment::init()
{
    motorProxRotation->setCurrentLimit(0.5f);
    motorProxPushPull->setCurrentLimit(0.5f);
    motorProxLinear->setCurrentLimit(0.5f);
    motorDistRotation->setCurrentLimit(0.5f);
    motorDistPushPull->setCurrentLimit(0.5f);
    motorDistLinear->setCurrentLimit(0.5f);

}
