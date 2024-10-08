#ifndef ACTUATOR3DOF_H
#define ACTUATOR3DOF_H

#include "Motor.h"
#include <QHash>
#include <QJsonObject>
#include <QPair>

typedef struct motor_config_t
{
    uint8_t limiter_idx;
    float current_limit;
    float abs_pos_offset;
}motor_config_t;

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
    void beginLinearHoming();
    bool getLinearHomingState();
    QString actuatorName();
    QPair<QSharedPointer<Motor>, motor_config_t> motorRotation;
    QPair<QSharedPointer<Motor>, motor_config_t> motorPushPull;
    QPair<QSharedPointer<Motor>, motor_config_t> motorLinear;
private:
    friend void setRotationDegAbs_impl(Actuator3DoF* act, float deg);
    friend void setPushPullDegAbs_impl(Actuator3DoF* act, float deg);
    friend void setLinearDegAbs_impl(Actuator3DoF* act, float deg);
    friend void initMotor_impl(Actuator3DoF* act);
    bool init();
    QString name;
    QPair<float, float> rotation_limit;
    QPair<float, float> pushpull_limit;
    QPair<float, float> linear_limit;
    float rotation_deg = 0.0f;
    float pushpull_deg = 0.0f;
    float linear_motion_deg = 0.0f;
};

#endif // ACTUATOR3DOF_H
