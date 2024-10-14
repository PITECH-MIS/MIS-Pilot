#ifndef EQUIPMENT6DOF_H
#define EQUIPMENT6DOF_H

#include "Actuator3DoF.h"
#include <QSharedPointer>

class Equipment6DoF
{
public:
    Equipment6DoF(QString n) : name(n) {};
    ~Equipment6DoF();
    bool parseJsonFromObject(const QJsonObject& object, QHash<QString, QSharedPointer<Motor>>& hash);
    QString& equipmentName();
    QWeakPointer<Actuator3DoF> getProximal();
    QWeakPointer<Actuator3DoF> getDistal();
    bool isAllReady();
    void setProximalAct(actuation_t& act);
    void setDistalAct(actuation_t& act);
    Point3D* getProximalTarget();
    Point3D* getDistalTarget();
    KinematicsBase *kinematics = nullptr;
private:
    QString name;
    QSharedPointer<Actuator3DoF> actuatorProximal = nullptr;
    QSharedPointer<Actuator3DoF> actuatorDistal = nullptr;
};

#endif // EQUIPMENT6DOF_H
