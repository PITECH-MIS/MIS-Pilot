#ifndef ROBOTARMWRAPPER_H
#define ROBOTARMWRAPPER_H

#include "RobotArm/xCore/Inc/robot.h"
#include <thread>
#include <atomic>
#include <QObject>

class RobotArmWrapper : public QObject
{
    Q_OBJECT
public:
    RobotArmWrapper() = default;
    ~RobotArmWrapper();
    void connect();
    void disconnect();
    void eventLoop();
    enum class JogAxis
    {
        X = 0,
        Y = 1,
        Z = 2,
        Rx = 3,
        Ry = 4,
        Rz = 5,
    };
    enum class RobotState : uint8_t
    {
        DISCONNECTED = 0,
        CONNECTING   = 1,
        CONNECTED    = 2,
        DISCONNECTING = 3,
    };
    struct RobotVersion
    {
        QString controller;
        QString type;
        QString sdk;
    } version = {"Unknown", "Unknown", "Unknown"};
    enum class RobotMode : uint8_t
    {
        MODE_INIT,
        MODE_DRAG,
        MODE_JOG,
    };
    std::array<double, 7> joint_pos;
    std::array<double, 7> joint_vel;
    std::array<double, 7> joint_torque;
    rokae::PowerState powerState;

    error_code jog(rokae::JogOpt::Space space, JogAxis axis, double rate, double length);
    rokae::xMateErProRobot *robot;

    // Singleton
    static RobotArmWrapper *getInstance();
    RobotArmWrapper(const RobotArmWrapper &) = delete;
    RobotArmWrapper &operator=(const RobotArmWrapper &) = delete;

signals:
    void onStateChanged();
    void onEventLoop();
private:
    RobotState state = RobotState::DISCONNECTED;
    RobotMode mode = RobotMode::MODE_INIT;
public:
    RobotState getState() { return state; }
    RobotMode getMode() { return mode; }
    void setMode(RobotMode m);
};

#define DEG2RAD(degree) ((float)(degree) * 0.01745329251994329576923690768489f)
#define RAD2DEG(radian) ((float)(radian) * 57.295779513082320876798154814105f)

#endif // ROBOTARMWRAPPER_H
