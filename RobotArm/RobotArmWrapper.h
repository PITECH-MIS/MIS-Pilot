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
    rokae::xMateErProRobot *robot;
};

#endif // ROBOTARMWRAPPER_H
