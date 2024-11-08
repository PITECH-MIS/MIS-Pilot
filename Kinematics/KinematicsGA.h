#ifndef KINEMATICSGA_H
#define KINEMATICSGA_H

#include "KinematicsBase.h"
#include <cstdint>

class KinematicsGA : public KinematicsBase
{
public:
    // KinematicsGA();
    void calculate(Point3D& prox, Point3D& dist) final;
    void proximalReset() final;
    void distalReset() final;
    void testGA();
};

#endif // KINEMATICSGA_H
