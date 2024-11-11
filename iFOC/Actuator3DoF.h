#ifndef ACTUATOR3DOF_H
#define ACTUATOR3DOF_H

#include "Motor.h"
#include <QHash>
#include <QJsonObject>
#include <QPair>
#include "../Kinematics/KinematicsBase.h"
#include "PDOEndpointAccess.h"

typedef struct motor_config_t
{
    uint8_t limiter_idx;
    float current_limit;
    float abs_pos_offset;
    float limiter_pos_offset;
    float pitch;                // length(mm) -> round
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
    void setPushPullLength(float mm);
    void setLinearLength(float mm);
    void beginLinearHoming();
    void beginRotationCalibrate();
    void beginRotationHoming();
    void beginPushPullHoming();
    void beginPreInstallHoming();
    void beginPostInstallHoming();
    void setRotationHome();
    void setPushPullHome();
    void setLinearHome();
    bool getLinearHomingState();
    int8_t checkRotationLimit();
    int8_t checkPushPullLimit();
    int8_t checkLinearLimit();
    QString actuatorName();
    QPair<QSharedPointer<Motor>, motor_config_t> motorRotation;
    QPair<QSharedPointer<Motor>, motor_config_t> motorPushPull;
    QPair<QSharedPointer<Motor>, motor_config_t> motorLinear;
    bool rotation_ready = false;
    bool pushpull_ready = false;
    bool linear_ready = false;
    bool preInstall_ready = false;
    bool postInstall_ready = false;
    bool is_rotation_homing = false;
    bool is_pushpull_homing = false;
    bool is_linear_homing = false;
    kinematics_params_t kineParams;
    Point3D targetPoint;
private:
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
