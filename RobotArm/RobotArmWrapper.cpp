#include "RobotArmWrapper.h"
#include "utils.h"

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
        robot->setMotionControlMode(MotionControlMode::NrtCommand, ec);
        robot->startReceiveRobotState(std::chrono::seconds(1), {tcpPose_m, tau_m, jointPos_m});
        std::array<double, 16> tcpPose{};
        std::array<double, 6> arr6{};
        std::atomic_bool running{true};

        while(robot->updateRobotState(std::chrono::steady_clock::duration::zero()));
        std::thread readState([&] {
            while (running) {
                robot->updateRobotState(std::chrono::seconds(1));
                robot->getStateData(tcpPose_m, tcpPose);
                robot->getStateData(jointPos_m, arr6);
                QString res = "TCP Pose: ";
                for(auto &i : tcpPose) res.append(QString::asprintf("%.2f, ", i));
                res.append(" Joint: ");
                for(auto &i : arr6) res.append(QString::asprintf("%.2f, ", i));
                qDebugMessage(res);
            }
        });
        readState.join();
    }
    catch(const std::exception &e)
    {
        qDebugMessage(e.what());
    }
}
