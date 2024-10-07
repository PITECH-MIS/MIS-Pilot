#include "Actuator3DoF.h"
#include <QApplication>

static void setRotationDegAbs_impl(Actuator3DoF* act, float deg)
{
    act->rotation_deg = _constrain(deg, act->rotation_limit.first, act->rotation_limit.second);
    act->motorRotation.first->setTrajAbsAngle(act->rotation_deg + act->motorRotation.second.abs_pos_offset);
    act->motorPushPull.first->setTrajAbsAngle(act->rotation_deg + act->pushpull_deg + act->motorPushPull.second.abs_pos_offset);
    act->motorRotation.first->setMode(Motor::MODE_TRAJECTORY);
    act->motorPushPull.first->setMode(Motor::MODE_TRAJECTORY);
    act->motorRotation.first->setState(Motor::STATE_ON);
    act->motorPushPull.first->setState(Motor::STATE_ON);
}

static void setPushPullDegAbs_impl(Actuator3DoF* act, float deg)
{
    act->pushpull_deg = _constrain(deg, act->pushpull_limit.first, act->pushpull_limit.second);
    act->motorPushPull.first->setTrajAbsAngle(act->rotation_deg + act->pushpull_deg + act->motorPushPull.second.abs_pos_offset);
    act->motorPushPull.first->setMode(Motor::MODE_TRAJECTORY);
    act->motorPushPull.first->setState(Motor::STATE_ON);
}

static void setLinearDegAbs_impl(Actuator3DoF* act, float deg)
{
    act->linear_motion_deg = _constrain(deg, act->linear_limit.first, act->linear_limit.second);
    act->motorLinear.first->setTrajAbsAngle(act->linear_motion_deg + act->motorLinear.second.abs_pos_offset);
    act->motorLinear.first->setMode(Motor::MODE_TRAJECTORY);
    act->motorLinear.first->setState(Motor::STATE_ON);
}

static void initMotor_impl(Actuator3DoF* act)
{
    act->motorRotation.first->setCurrentLimit(act->motorRotation.second.current_limit);
    act->motorPushPull.first->setCurrentLimit(act->motorPushPull.second.current_limit);
    act->motorLinear.first->setCurrentLimit(act->motorLinear.second.current_limit);
    act->motorRotation.first->setTrajAbsAngle(act->motorRotation.second.abs_pos_offset);
    act->motorPushPull.first->setTrajAbsAngle(act->motorPushPull.second.abs_pos_offset);
    act->motorLinear.first->setTrajAbsAngle(act->motorLinear.second.abs_pos_offset);
    act->motorRotation.first->setMode(Motor::MODE_TRAJECTORY);
    act->motorPushPull.first->setMode(Motor::MODE_TRAJECTORY);
    act->motorLinear.first->setMode(Motor::MODE_TRAJECTORY);
    act->motorRotation.first->setState(Motor::STATE_ON);
    QThread::msleep(100);
    act->motorPushPull.first->setState(Motor::STATE_ON);
    QThread::msleep(100);
    act->motorLinear.first->setState(Motor::STATE_ON);
}

static void linearHoming_impl(Actuator3DoF* act)
{
    float homing_speed = 90.0f;
    uint32_t timeout_set_move = 600000;      // 60s
    uint16_t timeout_set_curr_limit = 500;   // 0.5s
    bool isSuccess = false;
    uint8_t limiter_stage = 0;
    // Initialize Linear Motor
    act->motorLinear.first->setCurrentLimit(act->motorLinear.second.current_limit);
    act->motorLinear.first->setSpeed(0.0f);
    act->motorLinear.first->setMode(Motor::MODE_SPEED);
    act->motorLinear.first->setState(Motor::STATE_ON);
    QThread::msleep(100);
    float front_barrier = 0.0f;
    float back_barrier = 0.0f;
    float limiter_side_1 = 0.0f;
    float limiter_side_2 = 0.0f;
    // Forward till timeout or current over current_limit for a period of time
    uint32_t timeout_move = timeout_set_move;
    uint16_t timeout_curr_limit = timeout_set_curr_limit;
    while(timeout_move)
    {
        act->motorLinear.first->setSpeed(homing_speed);
        if(std::abs(act->motorLinear.first->getIq()) >= 0.95 * act->motorLinear.second.current_limit)
        {
            timeout_curr_limit -= 10;
            if(timeout_curr_limit == 0)
            {
                front_barrier = act->motorLinear.first->getPosDeg();
                break;
            }
        }
        else timeout_curr_limit = timeout_set_curr_limit;
        timeout_move -= 10;
        QThread::msleep(10);
    }
    if(!timeout_move) goto CLEAN;
    else qDebugMessage(QString::asprintf("Find linear front barrier: %.2f", front_barrier));

    // In current situation, limiter switch should not be activated.
    if(act->motorLinear.first->isLimiterActivated())
    {
        qDebugMessage("Limiter shouldn't be activated when we got front barrier");
        goto CLEAN;
    }

    // Backward to search limiter side 1/2
    act->motorLinear.first->setSpeed(-homing_speed); // backward 0.3s first
    QThread::msleep(300);
    timeout_move = timeout_set_move;
    timeout_curr_limit = timeout_set_curr_limit;
    while(timeout_move)
    {
        act->motorLinear.first->setSpeed(-homing_speed);
        if(std::abs(act->motorLinear.first->getIq()) >= 0.95 * act->motorLinear.second.current_limit)
        {
            timeout_curr_limit -= 5;
            if(timeout_curr_limit == 0) // Error: Reached barrier when searching for limiter
            {
                qDebugMessage("Reached barrier when searching for limiter");
                goto CLEAN;
            }
        }
        else timeout_curr_limit = timeout_set_curr_limit;
        if(limiter_stage == 0)
        {
            if(act->motorLinear.first->isLimiterActivated())
            {
                limiter_side_1 = act->motorLinear.first->getPosDeg();
                limiter_stage = 1;
                qDebugMessage(QString::asprintf("Found linear limiter side_1: %.2f", limiter_side_1));
                continue;
            }
        }
        else
        {
            if(!act->motorLinear.first->isLimiterActivated())
            {
                limiter_side_2 = act->motorLinear.first->getPosDeg();
                limiter_stage = 2;
                qDebugMessage(QString::asprintf("Found linear limiter side_2: %.2f", limiter_side_2));
                act->motorLinear.first->setSpeed(0.0f);
                break;
            }
        }
        timeout_move -= 5;
        QThread::msleep(5);
    }

    if(limiter_stage != 2)
    {
        qDebugMessage("Error in finding limiter side");
        goto CLEAN;
    }

    act->motorLinear.first->setState(Motor::STATE_OFF);
    QThread::msleep(300);

    // Calculate ABS_POS_OFFSET with limiter side_1/2
    act->motorLinear.second.abs_pos_offset = (limiter_side_1 + limiter_side_2) * 0.5f;

    // goto zero
    act->setLinearDegAbs(0.0f);

    qDebugMessage(QString::asprintf("Linear homing successful with new pos_offset: %.2f", act->motorLinear.second.abs_pos_offset));

    isSuccess = true;

    // Backward till timeout or current over current_limit for a period of time
    // act->motorLinear.first->setSpeed(-homing_speed); // backward 0.5s first
    // QThread::msleep(500);
    // timeout_move = timeout_set_move;
    // timeout_curr_limit = timeout_set_curr_limit;
    // while(timeout_move)
    // {
    //     act->motorLinear.first->setSpeed(-homing_speed);
    //     if(std::abs(act->motorLinear.first->getIq()) >= 0.95 * act->motorLinear.second.current_limit)
    //     {
    //         timeout_curr_limit -= 10;
    //         if(timeout_curr_limit == 0)
    //         {
    //             back_barrier = act->motorLinear.first->getPosDeg();
    //             break;
    //         }
    //     }
    //     else timeout_curr_limit = timeout_set_curr_limit;
    //     timeout_move -= 10;
    //     QThread::msleep(10);
    // }
    // if(!timeout_move) goto CLEAN;
    // else qDebugMessage(QString::asprintf("Find linear back barrier: %.2f", back_barrier));

    // // if NO limiter switch has been activated though the entire front-back process, raise an exception
    // if(!act->motorLinear.first->hasLimiterActivated())
    // {
    //     qDebugMessage("Linear limiter has not been activated, exiting...");
    //     goto CLEAN;
    // }

CLEAN:
    if(!isSuccess)
    {
        qDebugMessage("Error in linearHoming()");
        act->motorLinear.first->setSpeed(0.0f);
        act->motorLinear.first->setMode(Motor::MODE_SPEED);
        act->motorLinear.first->setState(Motor::STATE_OFF);
        return;
    }
}

Actuator3DoF::Actuator3DoF(QString n) : name(n)
{

}

Actuator3DoF::~Actuator3DoF()
{
    qDebugMessage("Actuator3DoF " + name + " destroyed");
}

void Actuator3DoF::beginLinearHoming()
{
    if(motorLinear.second.limiter_idx > 0)
    {
        QFuture<void> future = QtConcurrent::run(linearHoming_impl, this);
        while(!future.isFinished()) QApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void Actuator3DoF::setPushPullDegAbs(float deg)
{
    QFuture<void> future = QtConcurrent::run(setPushPullDegAbs_impl, this, deg);
    while(!future.isFinished()) QApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Actuator3DoF::setLinearDegAbs(float deg)
{
    QFuture<void> future = QtConcurrent::run(setLinearDegAbs_impl, this, deg);
    while(!future.isFinished()) QApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Actuator3DoF::setRotationDegAbs(float deg)
{
    QFuture<void> future = QtConcurrent::run(setRotationDegAbs_impl, this, deg);
    while(!future.isFinished()) QApplication::processEvents(QEventLoop::AllEvents, 100);
}

bool Actuator3DoF::init()
{
    QFuture<void> future = QtConcurrent::run(initMotor_impl, this);
    while(!future.isFinished()) QApplication::processEvents(QEventLoop::AllEvents, 100);
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
                cfg_ptr->limiter_idx = motorObject.value("limiter_idx").toInteger(0);
                if(key == "rotation")
                {
                    cfg_ptr->abs_pos_offset = motorRotation.first->getPosDeg();
                    motorRotation.first->limiter_index = cfg_ptr->limiter_idx;
                }
                else if(key == "pushpull")
                {
                    cfg_ptr->abs_pos_offset = motorPushPull.first->getPosDeg();
                    motorPushPull.first->limiter_index = cfg_ptr->limiter_idx;
                }
                else if(key == "linear")
                {
                    cfg_ptr->abs_pos_offset = motorLinear.first->getPosDeg();
                    motorLinear.first->limiter_index = cfg_ptr->limiter_idx;
                }
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
                target = std::abs(target);
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
                target = std::abs(target);
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
                target = std::abs(target);
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
