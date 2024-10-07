#ifndef ACTUATOR3DOF_H
#define ACTUATOR3DOF_H

#include "Motor.h"
#include "ActuatorWorker.h"
#include <QHash>
#include <QJsonObject>
#include <QPair>
#include <QThread>

class Actuator3DoF : public QObject
{
    Q_OBJECT
public:
    Actuator3DoF(QString n);
    ~Actuator3DoF();
    bool parseJsonFromObject(const QJsonObject& object, QHash<QString, QSharedPointer<Motor>>& hash);
    float getRotationState();
    float getPushPullState();
    float getLinearState();
    void setRotationDegAbs(float deg);
    void setPushPullDegAbs(float deg);
    void setLinearDegAbs(float deg);
    QString actuatorName();
private slots:
    void onWorkerDone();
signals:
    bool appendTaskList(QVector<actuator_worker_task_t>& taskList);
private:
    bool init();
    QString name;
    QPair<QSharedPointer<Motor>, motor_config_t> motorRotation;
    QPair<QSharedPointer<Motor>, motor_config_t> motorPushPull;
    QPair<QSharedPointer<Motor>, motor_config_t> motorLinear;
    QPair<float, float> rotation_limit;
    QPair<float, float> pushpull_limit;
    QPair<float, float> linear_limit;
    float rotation_deg = 0.0f;
    float pushpull_deg = 0.0f;
    float linear_motion_deg = 0.0f;
    QThread *workerThread = nullptr;
    ActuatorWorker *worker = nullptr;
    QVector<actuator_worker_task_t> workerTaskList;
};

#endif // ACTUATOR3DOF_H
