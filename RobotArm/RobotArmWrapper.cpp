#include "RobotArmWrapper.h"
#include "utils.h"
#include <iostream>

using namespace rokae;
using namespace RtSupportedFields;

RobotArmWrapper *RobotArmWrapper::getInstance()
{
    static RobotArmWrapper wrapper;
    return &wrapper;
}

RobotArmWrapper::~RobotArmWrapper()
{
    if(robot)
    {
        try
        {
            error_code ec;
            robot->stop(ec);
            robot->setPowerState(false, ec);
            robot->disconnectFromRobot(ec);
            qDebugMessage("Robot Disconnected");
        }
        catch(const std::exception &e)
        {
            qDebugMessage(e.what());
        }
    }
}

error_code RobotArmWrapper::jog(rokae::JogOpt::Space space, JogAxis axis, double rate, double length)
{
    error_code ec;
    robot->startJog(space, rate, abs(length), static_cast<unsigned int>(axis), length >= 0.0, ec);
    return ec;
}

void RobotArmWrapper::eventLoop()
{
    try
    {
        error_code ec;
        switch(state)
        {
        case RobotState::CONNECTING:
        {
            robot = new xMateErProRobot("192.168.0.160", "192.168.0.2");
            qDebugMessage("Pass");

            auto robotinfo = robot->robotInfo(ec);
            version.controller = QString::fromStdString(robotinfo.version);
            version.type = QString::fromStdString(robotinfo.type);
            version.sdk = QString::fromStdString(robot->sdkVersion());
            qDebugMessage("Controller Version: " + QString::fromStdString(robotinfo.version) + ", Type: " + QString::fromStdString(robotinfo.type));
            qDebugMessage("xCore-SDK Version: " + version.sdk);

            state = RobotState::CONNECTED;
            emit onStateChanged();
            break;
        }
        case RobotState::CONNECTED:
        {
            joint_pos = robot->jointPos(ec);
            joint_vel = robot->jointVel(ec);
            joint_torque = robot->jointTorque(ec);
            powerState = robot->powerState(ec);
            emit onEventLoop();
            break;
        }
        case RobotState::DISCONNECTING:
        {
            robot->stop(ec);
            robot->setPowerState(false, ec);
            robot->disconnectFromRobot(ec);
            qDebugMessage("Robot Disconnected");
            version.controller = "Unknown";
            version.type = "Unknown";
            version.sdk = "Unknown";
            state = RobotState::DISCONNECTED;
            emit onStateChanged();
            break;
        }
        default: break;
        }
    }
    catch(const std::exception &e)
    {
        qDebugMessage(e.what());
    }
}

void RobotArmWrapper::disconnect()
{
    if(state == RobotState::CONNECTED) state = RobotState::DISCONNECTING;
}

void RobotArmWrapper::setMode(RobotMode m)
{
    auto lambda = [this, m]()
    {
        if(state == RobotState::CONNECTED)
        {
            error_code ec;
            switch(m)
            {
            case RobotMode::MODE_DRAG:
                robot->setOperateMode(rokae::OperateMode::manual, ec);
                robot->setPowerState(false, ec);
                robot->enableDrag(DragParameter::cartesianSpace, DragParameter::freely, ec);
                qDebugMessage("Open Drag result: " + QString::fromStdString(ec.message()));
                break;
            case RobotMode::MODE_JOG:
                robot->setMotionControlMode(rokae::MotionControlMode::NrtCommand, ec);
                robot->setOperateMode(rokae::OperateMode::manual, ec);
                break;
            case RobotMode::MODE_INIT:
                if(mode == RobotMode::MODE_DRAG) robot->disableDrag(ec);
                else if(mode == RobotMode::MODE_JOG) robot->stop(ec);
                break;
            default: break;
            }
            mode = m;
        }
    };
    spawnTask(lambda);
}

void RobotArmWrapper::connect()
{
    if(state == RobotState::DISCONNECTED)
    {
        state = RobotState::CONNECTING;
    }
    // try
    // {

        // auto model = robot->model();

        // auto flan = robot->posture(CoordinateType::flangeInBase, ec);
        // auto tcp = robot->posture(CoordinateType::endInRef, ec);
        // auto base = robot->baseFrame(ec);

        // robot->setOperateMode(rokae::OperateMode::manual, ec);
        // robot->setPowerState(false, ec);
        // robot->enableDrag(DragParameter::cartesianSpace, DragParameter::freely, ec);
        // qDebugMessage("Open Drag result: " + QString::fromStdString(ec.message()));
        // // std::cout << "打开拖动结果: " << ec.message() << std::endl;
        // auto power = robot->powerState(ec);
        // QThread::msleep(2000);
        // qDebugMessage("Power state: " + QString::number(static_cast<int>(power)));

        // QThread::msleep(10000);
        // robot->disableDrag(ec);

        // robot->setMotionControlMode(rokae::MotionControlMode::NrtCommand, ec);
        // robot->setOperateMode(rokae::OperateMode::manual, ec);
        // qDebugMessage("Please press the button");
        // QThread::msleep(3000);
        // ec = jog(JogOpt::flange, JogAxis::Z, 0.5, 50);
        // QThread::msleep(3000);
        // ec = jog(JogOpt::flange, JogAxis::Z, 0.5, -50);
        // QThread::msleep(3000);
        // ec = jog(JogOpt::flange, JogAxis::X, 0.5, -50);
        // QThread::msleep(3000);
        // ec = jog(JogOpt::flange, JogAxis::X, 0.5, 50);
        // QThread::msleep(3000);

        // robot->stop(ec);

        // robot->setPowerState(false, ec);
        // robot->disconnectFromRobot(ec);

        // robot->setMotionControlMode(MotionControlMode::NrtCommand, ec);
        // robot->startReceiveRobotState(std::chrono::seconds(1), {tcpPose_m, tau_m, jointPos_m});
        // std::array<double, 16> tcpPose{};
        // std::array<double, 6> arr6{};
        // std::atomic_bool running{true};

        // while(robot->updateRobotState(std::chrono::steady_clock::duration::zero()));
        // std::thread readState([&] {
        //     while (running) {
        //         robot->updateRobotState(std::chrono::seconds(1));
        //         robot->getStateData(tcpPose_m, tcpPose);
        //         robot->getStateData(jointPos_m, arr6);
        //         QString res = "TCP Pose: ";
        //         for(auto &i : tcpPose) res.append(QString::asprintf("%.2f, ", i));
        //         res.append(" Joint: ");
        //         for(auto &i : arr6) res.append(QString::asprintf("%.2f, ", i));
        //         qDebugMessage(res);
        //     }
        // });
        // readState.join();
    // }
}
