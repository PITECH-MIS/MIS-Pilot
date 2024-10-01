#ifndef MIS_EQUIPMENT_H
#define MIS_EQUIPMENT_H

#include "motor.h"

class MIS_Equipment
{
public:
    // load from JSON + QMap<QString, Motor*>
    MIS_Equipment(Motor* p_r, Motor* p_p, Motor* p_l, Motor* d_r, Motor* d_p, Motor *d_l) :
        motorProxRotation(p_r), motorProxPushPull(p_p), motorProxLinear(p_l),
        motorDistRotation(d_r), motorDistPushPull(d_p), motorDistLinear(d_l) {};
    void init();
    void proximalBending(float angle);
    void proximalRotation(float angle);
    void distalBending(float angle);
    void distalRotation(float angle);
private:
    Motor* motorProxRotation;
    Motor* motorProxPushPull;
    Motor* motorProxLinear;
    Motor* motorDistRotation;
    Motor* motorDistPushPull;
    Motor* motorDistLinear;
};

#endif // MIS_EQUIPMENT_H
