#include "Actuator3DoF.h"

Actuator3DoF::Actuator3DoF(QString n) : name(n)
{

}

Actuator3DoF::~Actuator3DoF()
{
    qDebugMessage("Actuator3DoF " + name + " destroyed");
}

void Actuator3DoF::beginLinearHoming()
{
    if(motorLinear.second.limiter_idx == 0) return;
    auto lambda = [this]()
    {
        float homing_speed = 90.0f;
        uint32_t timeout_set_move = 600000;      // 60s
        uint16_t timeout_set_curr_limit = 500;   // 0.5s
        bool isSuccess = false;
        uint8_t limiter_stage = 0;
        // Initialize Linear Motor
        motorLinear.first->setCurrentLimit(motorLinear.second.current_limit);
        motorLinear.first->setSpeed(0.0f);
        motorLinear.first->setMode(Motor::MODE_SPEED);
        motorLinear.first->setState(Motor::STATE_ON);
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
            motorLinear.first->setSpeed(homing_speed);
            if(std::abs(motorLinear.first->getIq()) >= 0.95 * motorLinear.second.current_limit)
            {
                timeout_curr_limit -= 10;
                if(timeout_curr_limit == 0)
                {
                    front_barrier = motorLinear.first->getPosDeg();
                    break;
                }
            }
            else timeout_curr_limit = timeout_set_curr_limit;
            timeout_move -= 10;
            QThread::msleep(10);
        }
        if(!timeout_move) goto CLEAN;
        // else qDebugMessage(QString::asprintf("Find linear front barrier: %.2f", front_barrier));

        // In current situation, limiter switch should not be activated.
        if(motorLinear.first->isLimiterActivated())
        {
            qDebugMessage("ERROR: Limiter shouldn't be activated when we got front barrier");
            goto CLEAN;
        }

        // Backward to search limiter side 1/2
        motorLinear.first->setSpeed(-homing_speed); // backward 0.3s first
        QThread::msleep(300);
        timeout_move = timeout_set_move;
        timeout_curr_limit = timeout_set_curr_limit;
        while(timeout_move)
        {
            motorLinear.first->setSpeed(-homing_speed);
            if(std::abs(motorLinear.first->getIq()) >= 0.95 * motorLinear.second.current_limit)
            {
                timeout_curr_limit -= 5;
                if(timeout_curr_limit == 0) // Error: Reached barrier when searching for limiter
                {
                    qDebugMessage("ERROR: Reached barrier when searching for limiter");
                    goto CLEAN;
                }
            }
            else timeout_curr_limit = timeout_set_curr_limit;
            if(limiter_stage == 0)
            {
                if(motorLinear.first->isLimiterActivated())
                {
                    limiter_side_1 = motorLinear.first->getPosDeg();
                    limiter_stage = 1;
                    // qDebugMessage(QString::asprintf("Found linear limiter side_1: %.2f", limiter_side_1));
                    continue;
                }
            }
            else
            {
                if(!motorLinear.first->isLimiterActivated())
                {
                    limiter_side_2 = motorLinear.first->getPosDeg();
                    limiter_stage = 2;
                    // qDebugMessage(QString::asprintf("Found linear limiter side_2: %.2f", limiter_side_2));
                    motorLinear.first->setSpeed(0.0f);
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

        motorLinear.first->setState(Motor::STATE_OFF);
        QThread::msleep(300);

        // Calculate ABS_POS_OFFSET with limiter side_1/2
        motorLinear.second.abs_pos_offset = (limiter_side_1 + limiter_side_2) * 0.5f;

        // goto zero
        setLinearDegAbs(0.0f);

        qDebugMessage(QString::asprintf("Linear homing successful with new pos_offset: %.2f", motorLinear.second.abs_pos_offset));

        isSuccess = true;
        linear_ready = true;
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
            motorLinear.first->setSpeed(0.0f);
            motorLinear.first->setMode(Motor::MODE_SPEED);
            motorLinear.first->setState(Motor::STATE_OFF);
            return;
        }
    };
    spawnTask(lambda);
}

// From current state, rotation 2 rounds forward, then another 2 rounds backward
void Actuator3DoF::beginPushPullHoming()
{
    if(motorPushPull.second.limiter_idx == 0) return;
    auto lambda = [this]()
    {
        float homing_speed = 20.0f;
        float abs_rotation_limit = (2.0f * 360.0f);
        uint16_t timeout_set_curr_limit = 500;   // 0.5s
        uint16_t timeout_curr_limit = 500;
        float start_pp_deg = getPushPullState();
        float curr_pp_deg = 0.0f;
        float target_pp_deg = start_pp_deg;
        uint8_t stage = 0; // 0 - forward, 1 - backward, 2 - success
        float result = 0.0f;
        float limiter_side_1 = 0.0f;
        float limiter_side_2 = 0.0f;
        bool limiter_state[2] = {false, false}; // [out->in, in->out]
        bool is_start_inside_limiter = false;
        bool isSuccess = false;
        if(motorPushPull.first->isLimiterActivated()) is_start_inside_limiter = true;
        while(stage != 2)
        {
            if(stage == 0) target_pp_deg += getIncAngleByRPM(homing_speed, 0.001f);
            else if(stage == 1) target_pp_deg -= getIncAngleByRPM(homing_speed, 0.001f);
            curr_pp_deg = getPushPullState() - start_pp_deg;
            if(std::abs(target_pp_deg) >= abs_rotation_limit)
            {
                if(stage == 0)
                {
                    stage = 1;
                }
                else
                {
                    qDebugMessage("Error in pushPullHoming(): index not found");
                    goto CLEAN;
                }
            }
            setPushPullDegAbs(target_pp_deg);
            if(std::abs(motorPushPull.first->getIq()) >= 0.95 * motorPushPull.second.current_limit)
            {
                timeout_curr_limit -= 1;
                if(timeout_curr_limit == 0)
                {
                    qDebugMessage("ERROR: Reached current_limit at pushPullHoming()");
                    goto CLEAN;
                }
            }
            else timeout_curr_limit = timeout_set_curr_limit;
            if(is_start_inside_limiter) // in->out first, out->in second
            {
                if(!motorPushPull.first->isLimiterActivated() && !limiter_state[1])
                {
                    limiter_state[1] = true;
                    limiter_side_2 = curr_pp_deg;
                    if(stage == 0) stage = 1; // START REVERSAL IMMEDIATELY
                }
                if(limiter_state[1])
                {
                    if(motorPushPull.first->isLimiterActivated() && !limiter_state[0])
                    {
                        limiter_state[0] = true;
                        limiter_side_1 = curr_pp_deg;
                        break;
                    }
                }
            }
            else // out->in first, in->out second
            {
                if(motorPushPull.first->isLimiterActivated() && !limiter_state[0])
                {
                    limiter_state[0] = true;
                    limiter_side_1 = curr_pp_deg;
                    // KEEP FORWARD/BACKWARD
                }
                if(limiter_state[0])
                {
                    if(!motorPushPull.first->isLimiterActivated() && !limiter_state[1])
                    {
                        limiter_state[1] = true;
                        limiter_side_2 = curr_pp_deg;
                        break;
                    }
                }
            }
            QThread::msleep(1);
        }
        if(!(limiter_state[0] && limiter_state[1]))
        {
            qDebugMessage("ERROR: Cannot find two barriers for pushPullHoming limiter");
            goto CLEAN;
        }
        isSuccess = true;
        result = (limiter_side_1 + limiter_side_2) * 0.5f;
        // result + start_pp_deg;
        motorPushPull.second.abs_pos_offset += result + start_pp_deg;
        setPushPullDegAbs(0.0f);
        pushpull_ready = true;
    CLEAN:
        if(!isSuccess)
        {
            qDebugMessage("Error in pushPullHoming()");
            motorPushPull.first->setState(Motor::STATE_OFF);
            return;
        }
    };
    spawnTask(lambda);
}

// Find Rotation limiter position first, then move -limiter_pos_offset
void Actuator3DoF::beginRotationHoming()
{
    auto lambda = [this]()
    {
        float homing_speed = 10.0f;
        uint16_t timeout_set_curr_limit = 500;   // 0.5s
        uint16_t timeout_curr_limit = 500;
        float start_rotation_deg = getRotationState();
        float curr_rotation_deg = 0.0f;
        float target_rotation_deg = start_rotation_deg;
        float result = 0.0f;
        float limiter_side_1 = 0.0f;
        float limiter_side_2 = 0.0f;
        bool limiter_state[2] = {false, false}; // [out->in, in->out]
        bool is_start_inside_limiter = false;
        bool isSuccess = false;
        if(motorRotation.first->isLimiterActivated()) is_start_inside_limiter = true;
        while(std::abs(curr_rotation_deg) <= 360.0f)
        {
            target_rotation_deg += getIncAngleByRPM(homing_speed, 0.001f);
            setRotationDegAbs(target_rotation_deg);
            curr_rotation_deg = getRotationState() - start_rotation_deg;
            if(std::abs(motorRotation.first->getIq()) >= 0.95 * motorRotation.second.current_limit ||
                std::abs(motorPushPull.first->getIq()) >= 0.95 * motorPushPull.second.current_limit)
            {
                timeout_curr_limit -= 1;
                if(timeout_curr_limit == 0)
                {
                    qDebugMessage("ERROR: Reached current_limit at rotationHoming");
                    goto CLEAN;
                }
            }
            else timeout_curr_limit = timeout_set_curr_limit;
            if(is_start_inside_limiter) // in->out first, out->in second
            {
                if(!motorRotation.first->isLimiterActivated() && !limiter_state[1])
                {
                    limiter_state[1] = true;
                    limiter_side_2 = curr_rotation_deg;
                }
                if(limiter_state[1])
                {
                    if(motorRotation.first->isLimiterActivated() && !limiter_state[0])
                    {
                        limiter_state[0] = true;
                        limiter_side_1 = curr_rotation_deg;
                        break;
                    }
                }
            }
            else // out->in first, in->out second
            {
                if(motorRotation.first->isLimiterActivated() && !limiter_state[0])
                {
                    limiter_state[0] = true;
                    limiter_side_1 = curr_rotation_deg;
                }
                if(limiter_state[0])
                {
                    if(!motorRotation.first->isLimiterActivated() && !limiter_state[1])
                    {
                        limiter_state[1] = true;
                        limiter_side_2 = curr_rotation_deg;
                        break;
                    }
                }
            }
            QThread::msleep(1);
        }
        if(!(limiter_state[0] && limiter_state[1]))
        {
            qDebugMessage("ERROR: Cannot find two barriers for rotationHoming limiter");
            goto CLEAN;
        }
        isSuccess = true;
        if(limiter_side_1 + limiter_side_2 > 360.0f)
        {
            if(limiter_side_1 > 180.0f) limiter_side_1 -= 360.0f;
            if(limiter_side_2 > 180.0f) limiter_side_2 -= 360.0f;
        }
        result = (limiter_side_1 + limiter_side_2) * 0.5f;
        // setRotationDegAbs(result + start_rotation_deg), rotation_deg = result + start_rotation_deg.
        // (rotation/pushpull) motor.abs_offset += result + start_rotation_deg, rotation_deg can be zero.
        // (rotation/pushpull) motor.abs_offset -= motorRotation.second.limiter_pos_offset.
        motorRotation.second.abs_pos_offset += result + start_rotation_deg - motorRotation.second.limiter_pos_offset;
        motorPushPull.second.abs_pos_offset += result + start_rotation_deg - motorRotation.second.limiter_pos_offset;
        // (rotation) current Trajectory target: abs_pos_offset. current Trajectory set: motorRotation.posDeg()
        if(motorRotation.first->getPosDeg() >= motorRotation.second.abs_pos_offset)
        {
            while((motorRotation.first->getPosDeg() - motorRotation.second.abs_pos_offset) >= 180.0f) motorRotation.second.abs_pos_offset += 360.0f;
        }
        else while((motorRotation.second.abs_pos_offset - motorRotation.first->getPosDeg()) >= 180.0f) motorRotation.second.abs_pos_offset -= 360.0f;
        // (pushpull) current Trajectory target: pushpull_deg + abs_pos_offset. currentTrajectory set: motorPushPull.posDeg()
        if(motorPushPull.first->getPosDeg() >= (motorPushPull.second.abs_pos_offset + pushpull_deg))
        {
            while((motorPushPull.first->getPosDeg() - (motorPushPull.second.abs_pos_offset + pushpull_deg)) >= 180.0f) motorPushPull.second.abs_pos_offset += 360.0f;
        }
        else while(((motorPushPull.second.abs_pos_offset + pushpull_deg) - motorPushPull.first->getPosDeg()) >= 180.0f) motorPushPull.second.abs_pos_offset -= 360.0f;
        setRotationDegAbs(0.0f);
        rotation_ready = true;
    CLEAN:
        if(!isSuccess)
        {
            qDebugMessage("Error in rotationHoming()");
            motorRotation.first->setState(Motor::STATE_OFF);
            motorPushPull.first->setState(Motor::STATE_OFF);
            return;
        }
    };
    spawnTask(lambda);
}

// Manually set the rotation gear position first
void Actuator3DoF::beginRotationCalibrate()
{
    if(motorRotation.second.limiter_idx == 0) return;
    auto lambda = [this]()
    {
        float homing_speed = 6.0f; // RPM
        uint16_t timeout_set_curr_limit = 500;   // 0.5s
        uint16_t timeout_curr_limit = 500;
        float start_rotation_deg = 0.0f;
        float curr_rotation_deg = 0.0f;
        float target_rotation_deg = 0.0f;
        float result = 0.0f;
        float limiter_side_1 = 0.0f;
        float limiter_side_2 = 0.0f;
        bool limiter_state[2] = {false, false}; // [out->in, in->out]
        bool is_start_inside_limiter = false;
        bool isSuccess = false;
        motorRotation.first->setMode(Motor::MODE_TRAJECTORY);
        motorRotation.first->setState(Motor::STATE_ON);
        QThread::msleep(100);
        start_rotation_deg = getRotationState();
        target_rotation_deg = start_rotation_deg;
        if(motorRotation.first->isLimiterActivated()) is_start_inside_limiter = true;
        while(std::abs(curr_rotation_deg) <= 360.0f)
        {
            target_rotation_deg += getIncAngleByRPM(homing_speed, 0.001f);
            motorRotation.first->setTrajAbsAngle(target_rotation_deg + motorRotation.second.abs_pos_offset);
            curr_rotation_deg = getRotationState() - start_rotation_deg;
            if(std::abs(motorRotation.first->getIq()) >= 0.95 * motorRotation.second.current_limit)
            {
                timeout_curr_limit -= 1;
                if(timeout_curr_limit == 0)
                {
                    qDebugMessage("ERROR: Reached current_limit at rotationCalibrate");
                    goto CLEAN;
                }
            }
            else timeout_curr_limit = timeout_set_curr_limit;
            if(is_start_inside_limiter) // in->out first, out->in second
            {
                if(!motorRotation.first->isLimiterActivated() && !limiter_state[1])
                {
                    limiter_state[1] = true;
                    limiter_side_2 = curr_rotation_deg;
                    // qDebugMessage(QString::asprintf("Found in->out barrier: %.2f", limiter_side_2));
                }
                if(limiter_state[1])
                {
                    if(motorRotation.first->isLimiterActivated() && !limiter_state[0])
                    {
                        limiter_state[0] = true;
                        limiter_side_1 = curr_rotation_deg;
                        // qDebugMessage(QString::asprintf("Found out->in barrier: %.2f", limiter_side_1));
                        break;
                    }
                }
            }
            else // out->in first, in->out second
            {
                if(motorRotation.first->isLimiterActivated() && !limiter_state[0])
                {
                    limiter_state[0] = true;
                    limiter_side_1 = curr_rotation_deg;
                    // qDebugMessage(QString::asprintf("Found out->in barrier: %.2f", limiter_side_1));
                }
                if(limiter_state[0])
                {
                    if(!motorRotation.first->isLimiterActivated() && !limiter_state[1])
                    {
                        limiter_state[1] = true;
                        limiter_side_2 = curr_rotation_deg;
                        // qDebugMessage(QString::asprintf("Found in->out barrier: %.2f", limiter_side_2));
                        break;
                    }
                }
            }
            QThread::msleep(1);
        }
        if(!(limiter_state[0] && limiter_state[1]))
        {
            qDebugMessage("ERROR: Cannot find two barriers for rotationCalibrate limiter");
            goto CLEAN;
        }
        isSuccess = true;
        if(limiter_side_1 + limiter_side_2 > 360.0f)
        {
            if(limiter_side_1 > 180.0f) limiter_side_1 -= 360.0f;
            if(limiter_side_2 > 180.0f) limiter_side_2 -= 360.0f;
        }
        result = (limiter_side_1 + limiter_side_2) * 0.5f;
        qDebugMessage(QString::asprintf("Calculated limiter result: %.2f", result));
        motorRotation.second.limiter_pos_offset = result;
    CLEAN:
        if(!isSuccess)
        {
            qDebugMessage("Error in rotationCalibrate()");
            motorRotation.first->setState(Motor::STATE_OFF);
            return;
        }
    };
    spawnTask(lambda);
}

void Actuator3DoF::setPushPullDegAbs(float deg)
{
    auto lambda = [this, &deg]()
    {
        pushpull_deg = _constrain(deg, pushpull_limit.first, pushpull_limit.second);
        motorPushPull.first->setTrajAbsAngle(rotation_deg + pushpull_deg + motorPushPull.second.abs_pos_offset);
        motorPushPull.first->setMode(Motor::MODE_TRAJECTORY);
        motorPushPull.first->setState(Motor::STATE_ON);
    };
    spawnTask(lambda);
}

void Actuator3DoF::setLinearDegAbs(float deg)
{
    auto lambda = [this, &deg]()
    {
        linear_motion_deg = _constrain(deg, linear_limit.first, linear_limit.second);
        motorLinear.first->setTrajAbsAngle(linear_motion_deg + motorLinear.second.abs_pos_offset);
        motorLinear.first->setMode(Motor::MODE_TRAJECTORY);
        motorLinear.first->setState(Motor::STATE_ON);
    };
    spawnTask(lambda);
}

void Actuator3DoF::setRotationDegAbs(float deg)
{
    auto lambda = [this, &deg]()
    {
        rotation_deg = _constrain(deg, rotation_limit.first, rotation_limit.second);
        motorRotation.first->setTrajAbsAngle(rotation_deg + motorRotation.second.abs_pos_offset);
        motorPushPull.first->setTrajAbsAngle(rotation_deg + pushpull_deg + motorPushPull.second.abs_pos_offset);
        motorRotation.first->setMode(Motor::MODE_TRAJECTORY);
        motorPushPull.first->setMode(Motor::MODE_TRAJECTORY);
        motorRotation.first->setState(Motor::STATE_ON);
        motorPushPull.first->setState(Motor::STATE_ON);
    };
    spawnTask(lambda);
}

bool Actuator3DoF::init()
{
    auto lambda = [this]()
    {
        motorRotation.first->setCurrentLimit(motorRotation.second.current_limit);
        motorPushPull.first->setCurrentLimit(motorPushPull.second.current_limit);
        motorLinear.first->setCurrentLimit(motorLinear.second.current_limit);
        motorRotation.first->setTrajAbsAngle(motorRotation.second.abs_pos_offset);
        motorPushPull.first->setTrajAbsAngle(motorPushPull.second.abs_pos_offset);
        motorLinear.first->setTrajAbsAngle(motorLinear.second.abs_pos_offset);
        motorRotation.first->setMode(Motor::MODE_TRAJECTORY);
        motorPushPull.first->setMode(Motor::MODE_TRAJECTORY);
        motorLinear.first->setMode(Motor::MODE_TRAJECTORY);
        motorRotation.first->setState(Motor::STATE_ON);
        QThread::msleep(100);
        motorPushPull.first->setState(Motor::STATE_ON);
        QThread::msleep(100);
        motorLinear.first->setState(Motor::STATE_ON);
    };
    spawnTask(lambda);
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
                cfg_ptr->limiter_pos_offset = motorObject.value("limiter_offset").toDouble(0.0f);
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
