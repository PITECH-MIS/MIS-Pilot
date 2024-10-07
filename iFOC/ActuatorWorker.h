#ifndef ACTUATORWORKER_H
#define ACTUATORWORKER_H

#include "Motor.h"

typedef struct motor_config_t
{
    uint8_t limiter_idx;
    float current_limit;
    float abs_pos_offset;
}motor_config_t;

enum class actuator_worker_task_enum
{
    TASK_ROTATION,
    TASK_PUSHPULL,
    TASK_LINEAR,
    TASK_POWERUP,
    TASK_HOMING
};

typedef enum actuator_worker_motor_enum
{
    MOTOR_ROTATION = 0,
    MOTOR_PUSHPULL = 1,
    MOTOR_LINEAR = 2
}actuator_worker_motor_enum;

typedef struct actuator_worker_task_t
{
    actuator_worker_task_enum taskType;
    float parameter;
    float parameter2;
    unsigned long delayToNextTaskMs;
}actuator_worker_task_t;

class ActuatorWorker : public QObject
{
    Q_OBJECT
public:
    explicit ActuatorWorker(QPair<QSharedPointer<Motor>, motor_config_t>& r,
                            QPair<QSharedPointer<Motor>, motor_config_t>& p,
                            QPair<QSharedPointer<Motor>, motor_config_t>& l,
                            QObject *parent = nullptr);
    bool taskRunning();
public slots:
    bool appendTaskList(QVector<actuator_worker_task_t>& taskList);
signals:
    void taskDone();
private:
    bool isTaskRunning = false;
    QPair<QSharedPointer<Motor>, motor_config_t>& motorRotation;
    QPair<QSharedPointer<Motor>, motor_config_t>& motorPushPull;
    QPair<QSharedPointer<Motor>, motor_config_t>& motorLinear;
};

#endif // ACTUATORWORKER_H
