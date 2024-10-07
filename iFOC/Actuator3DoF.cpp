#include "Actuator3DoF.h"

Actuator3DoF::Actuator3DoF(QString n) : name(n)
{
    worker = new ActuatorWorker(motorRotation, motorPushPull, motorLinear, this);
    workerThread = new QThread(this);
    worker->moveToThread(workerThread);
    connect(this, &Actuator3DoF::appendTaskList, worker, &ActuatorWorker::appendTaskList);
    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(worker, &ActuatorWorker::taskDone, this, &Actuator3DoF::onWorkerDone);
    workerThread->start();
}

Actuator3DoF::~Actuator3DoF()
{
    qDebugMessage("Actuator3DoF " + name + " destroyed");
    workerThread->quit();
    workerThread->wait();
}

void Actuator3DoF::setRotationDegAbs(float deg)
{
    rotation_deg = _constrain(deg, rotation_limit.first, rotation_limit.second);
    motorRotation.first.get()->setTrajAbsAngle(rotation_deg + motorRotation.second.abs_pos_offset);
    motorPushPull.first.get()->setTrajAbsAngle(rotation_deg + pushpull_deg + motorPushPull.second.abs_pos_offset);
    motorRotation.first.get()->setMode(Motor::MODE_TRAJECTORY);
    motorPushPull.first.get()->setMode(Motor::MODE_TRAJECTORY);
    motorRotation.first.get()->setState(Motor::STATE_ON);
    motorPushPull.first.get()->setState(Motor::STATE_ON);
    // if(worker && !worker->taskRunning())
    // {
    //     rotation_deg = _constrain(deg, rotation_limit.first, rotation_limit.second);
    //     workerTaskList.clear();
    //     actuator_worker_task_t task;
    //     task.taskType = actuator_worker_task_enum::TASK_ROTATION;
    //     task.parameter = rotation_deg;
    //     task.parameter2 = pushpull_deg;
    //     task.delayToNextTaskMs = 0;
    //     workerTaskList.append(task);
    //     emit appendTaskList(workerTaskList);
    // }
}

void Actuator3DoF::setPushPullDegAbs(float deg)
{
    pushpull_deg = _constrain(deg, pushpull_limit.first, pushpull_limit.second);
    motorPushPull.first.get()->setTrajAbsAngle(rotation_deg + pushpull_deg + motorPushPull.second.abs_pos_offset);
    motorPushPull.first.get()->setMode(Motor::MODE_TRAJECTORY);
    motorPushPull.first.get()->setState(Motor::STATE_ON);
}

void Actuator3DoF::setLinearDegAbs(float deg)
{
    linear_motion_deg = _constrain(deg, linear_limit.first, linear_limit.second);
    motorLinear.first.get()->setTrajAbsAngle(linear_motion_deg + motorLinear.second.abs_pos_offset);
    motorLinear.first.get()->setMode(Motor::MODE_TRAJECTORY);
    motorLinear.first.get()->setState(Motor::STATE_ON);
}

void Actuator3DoF::onWorkerDone()
{
    worker = nullptr;
}

bool Actuator3DoF::init()
{
    motorRotation.first.get()->setCurrentLimit(motorRotation.second.current_limit);
    motorPushPull.first.get()->setCurrentLimit(motorPushPull.second.current_limit);
    motorLinear.first.get()->setCurrentLimit(motorLinear.second.current_limit);
    motorRotation.first.get()->setTrajAbsAngle(motorRotation.second.abs_pos_offset);
    motorPushPull.first.get()->setTrajAbsAngle(motorPushPull.second.abs_pos_offset);
    motorLinear.first.get()->setTrajAbsAngle(motorLinear.second.abs_pos_offset);
    motorRotation.first.get()->setMode(Motor::MODE_TRAJECTORY);
    motorPushPull.first.get()->setMode(Motor::MODE_TRAJECTORY);
    motorLinear.first.get()->setMode(Motor::MODE_TRAJECTORY);
    motorRotation.first.get()->setState(Motor::STATE_ON);
    motorPushPull.first.get()->setState(Motor::STATE_ON);
    motorLinear.first.get()->setState(Motor::STATE_ON);
    return true;
}

bool Actuator3DoF::parseJsonFromObject(const QJsonObject& object, QHash<QString, QSharedPointer<Motor>>& hash)
{
    int motorErrorCount = 0;
    for(auto i = object.constBegin(); i != object.constEnd(); ++i)
    {
        QString key = i.key().toLower();
        if(i.value().isObject())
        {
            QJsonObject motorObject = i.value().toObject();
            if(key != "rotation" && key != "pushpull" && key != "linear")
            {
                qDebugMessage("Received unsupported motor name(supports Rotation/PushPull/Linear): " + i.key());
                motorErrorCount++;
                continue;
            }
            QString snString = QString::number(motorObject.value("motor_sn").toInteger(-1));
            if(!hash.contains(snString))
            {
                qDebugMessage("Target SN number not found in active motors (-1 if unspecified SN): " + i.key() + ", " + snString);
                motorErrorCount++;
                continue;
            }
            motor_config_t *cfg_ptr = nullptr;
            if(key == "rotation")
            {
                motorRotation.first = hash.value(snString);
                cfg_ptr = &motorRotation.second;
            }
            else if(key == "pushpull")
            {
                motorPushPull.first = hash.value(snString);
                cfg_ptr = &motorPushPull.second;
            }
            else if(key == "linear")
            {
                motorLinear.first = hash.value(snString);
                cfg_ptr = &motorLinear.second;
            }
            if(cfg_ptr)
            {
                memset(cfg_ptr, 0, sizeof(motor_config_t));
                if(key == "rotation") cfg_ptr->abs_pos_offset = motorRotation.first.get()->getPosDeg();
                else if(key == "pushpull") cfg_ptr->abs_pos_offset = motorPushPull.first.get()->getPosDeg();
                else if(key == "linear") cfg_ptr->abs_pos_offset = motorLinear.first.get()->getPosDeg();
                cfg_ptr->limiter_idx = motorObject.value("limiter_idx").toInteger(0);
                cfg_ptr->current_limit = motorObject.value("current_limit").toDouble(0.5f);
                qDebugMessage("Motor " + i.key() + QString::asprintf(" limit_idx: %d, curr_lim: %.2f, abs_offset: %.2f",
                                                                     cfg_ptr->limiter_idx,
                                                                     cfg_ptr->current_limit,
                                                                     cfg_ptr->abs_pos_offset));
            }
            else qDebugMessage("Unknown Error while parsing motor");
        }
        else if(i.value().isDouble())
        {
            float target = i.value().toDouble(0.0f);
            if(key == "rotation_abs_angle_limit")
            {
                rotation_limit = QPair<float, float>(-target, target);
            }
            else if(key == "rotation_neg_angle_limit")
            {
                if(target <= std::min(rotation_limit.first, rotation_limit.second)) rotation_limit.first = target;
            }
            else if(key == "rotation_pos_angle_limit")
            {
                if(target >= std::max(rotation_limit.first, rotation_limit.second)) rotation_limit.second = target;
            }
            else if(key == "pushpull_abs_angle_limit")
            {
                pushpull_limit = QPair<float, float>(-target, target);
            }
            else if(key == "pushpull_neg_angle_limit")
            {
                if(target <= std::min(pushpull_limit.first, pushpull_limit.second)) pushpull_limit.first = target;
            }
            else if(key == "pushpull_pos_angle_limit")
            {
                if(target >= std::max(pushpull_limit.first, pushpull_limit.second)) pushpull_limit.second = target;
            }
            else if(key == "linear_abs_angle_limit")
            {
                linear_limit = QPair<float, float>(-target, target);
            }
            else if(key == "linear_neg_angle_limit")
            {
                if(target <= std::min(linear_limit.first, linear_limit.second)) linear_limit.first = target;
            }
            else if(key == "linear_pos_angle_limit")
            {
                if(target >= std::max(linear_limit.first, linear_limit.second)) linear_limit.second = target;
            }
            else qDebugMessage("Unknown key: " + key);
        }
    }
    if(motorErrorCount > 0) return false;
    else
    {
        // qDebugMessage("Initialize actuator...");
        if(init()) qDebugMessage("Actuator " + name + " successfully initialized");
        else qDebugMessage("Actuator " + name + " failed to initialize");
    }
    return true;
}

float Actuator3DoF::getRotationState()
{
    // return rotation_deg; // desired value
    return (motorRotation.first->getPosDeg() - motorRotation.second.abs_pos_offset);
}

float Actuator3DoF::getPushPullState()
{
    // return pushpull_deg; // desired value
    return (motorPushPull.first->getPosDeg() - motorPushPull.second.abs_pos_offset - getRotationState());
}

float Actuator3DoF::getLinearState()
{
    // return linear_motion_deg; // desired value
    return (motorLinear.first->getPosDeg() - motorLinear.second.abs_pos_offset);
}

QString Actuator3DoF::actuatorName()
{
    return name;
}
