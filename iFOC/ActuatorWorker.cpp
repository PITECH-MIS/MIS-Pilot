#include "ActuatorWorker.h"

ActuatorWorker::ActuatorWorker(QPair<QSharedPointer<Motor>, motor_config_t>& r,
                            QPair<QSharedPointer<Motor>, motor_config_t>& p,
                            QPair<QSharedPointer<Motor>, motor_config_t>& l,
                            QObject *parent)
    : QObject(parent), motorRotation(r), motorPushPull(p), motorLinear(l)
{

}

bool ActuatorWorker::appendTaskList(QVector<actuator_worker_task_t>& taskList)
{
    if(taskRunning()) return false;
    isTaskRunning = true;
    for(auto i = taskList.constBegin(); i != taskList.constEnd(); ++i)
    {
        switch(i->taskType)
        {
        case actuator_worker_task_enum::TASK_ROTATION: // rotation, pushpull
            motorRotation.first.get()->setTrajAbsAngle(i->parameter + motorRotation.second.abs_pos_offset);
            motorPushPull.first.get()->setTrajAbsAngle(i->parameter + i->parameter2 + motorPushPull.second.abs_pos_offset);
            motorRotation.first.get()->setMode(Motor::MODE_TRAJECTORY);
            motorPushPull.first.get()->setMode(Motor::MODE_TRAJECTORY);
            motorRotation.first.get()->setState(Motor::STATE_ON);
            motorPushPull.first.get()->setState(Motor::STATE_ON);
            break;
        case actuator_worker_task_enum::TASK_PUSHPULL: // rotation, pushpull
            motorPushPull.first.get()->setTrajAbsAngle(i->parameter + i->parameter2 + motorPushPull.second.abs_pos_offset);
            motorPushPull.first.get()->setMode(Motor::MODE_TRAJECTORY);
            motorPushPull.first.get()->setState(Motor::STATE_ON);
            break;
        case actuator_worker_task_enum::TASK_LINEAR:
            motorLinear.first.get()->setTrajAbsAngle(i->parameter + motorLinear.second.abs_pos_offset);
            motorLinear.first.get()->setMode(Motor::MODE_TRAJECTORY);
            motorLinear.first.get()->setState(Motor::STATE_ON);
            break;
        default: break;
        }
        QThread::msleep((unsigned long)i->delayToNextTaskMs);
    }
    emit taskDone();
    isTaskRunning = false;
    return true;
}

bool ActuatorWorker::taskRunning()
{
    return isTaskRunning;
}
