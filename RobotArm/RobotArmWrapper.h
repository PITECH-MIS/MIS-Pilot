#ifndef ROBOTARMWRAPPER_H
#define ROBOTARMWRAPPER_H

#include "RobotArm/xCore/Inc/robot.h"
#include <thread>
#include <atomic>

class RobotArmWrapper
{
public:
    RobotArmWrapper();
    void init();
    enum class JogAxis
    {
        X = 0,
        Y = 1,
        Z = 2,
        J4 = 3,
        Ry = 4,
        J6 = 5,
    };
    error_code jog(rokae::JogOpt::Space space, JogAxis axis, double rate, double length);
    rokae::xMateErProRobot *robot;
};

#endif // ROBOTARMWRAPPER_H
