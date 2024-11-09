#include "RobotArmWrapper.h"
#include "utils.h"
#include <iostream>

using namespace rokae;

RobotArmWrapper::RobotArmWrapper() {}

void RobotArmWrapper::init()
{
    try
    {
        using namespace RtSupportedFields;
        robot = new xMateErProRobot("192.168.0.160", "192.168.0.2");
        qDebugMessage("Pass");
        error_code ec;

        auto robotinfo = robot->robotInfo(ec);
        qDebugMessage("Controller Version: " + QString::fromStdString(robotinfo.version) + ", Type: " + QString::fromStdString(robotinfo.type));
        qDebugMessage("xCore-SDK Version: " + QString::fromStdString(robot->sdkVersion()));
        auto model = robot->model();

        // auto joint_pos = robot->jointPos(ec);
        // auto joint_vel = robot->jointVel(ec);
        // auto joint_torque = robot->jointTorque(ec);

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

        robot->setMotionControlMode(rokae::MotionControlMode::NrtCommand, ec);
        robot->setOperateMode(rokae::OperateMode::manual, ec);
        qDebugMessage("Please press the button");
        QThread::msleep(5000);
        robot->startJog(JogOpt::world, 0.5, 100, 2, false, ec); // Neg. Direction 100mm, 50% speed
        QThread::msleep(5000);
        robot->startJog(JogOpt::world, 0.5, 100, 2, true, ec); // Pos. Direction
        robot->stop(ec);

        robot->setPowerState(false, ec);
        robot->disconnectFromRobot(ec);

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
    }
    catch(const std::exception &e)
    {
        qDebugMessage(e.what());
    }
}
