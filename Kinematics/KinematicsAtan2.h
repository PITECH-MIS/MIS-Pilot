#ifndef KINEMATICSATAN2_H
#define KINEMATICSATAN2_H

#include "KinematicsBase.h"
#include "cstdint"

class KinematicsAtan2 : public KinematicsBase
{
public:
    // KinematicsAtan2(kinematics_params_t& prox, kinematics_params_t& dist) : KinematicsBase(prox, dist) {};
    void calculate(Point3D& prox, Point3D& dist) final;
    void proximalReset() final;
    void distalReset() final;
private:
    uint8_t getXYDimension(Point3D &a);
    float lastProxAngle = 0.0f;
    float lastDistAngle = 0.0f;
};

#endif // KINEMATICSATAN2_H
