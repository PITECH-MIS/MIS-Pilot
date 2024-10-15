#include "Equipment6DoF.h"

Equipment6DoF::~Equipment6DoF()
{
    qDebugMessage("Equipment6DoF " + name + " destroyed");
}

bool Equipment6DoF::isAllReady()
{
    return (
            actuatorProximal &&
               actuatorProximal->linear_ready
               // && actuatorProximal->rotation_ready
               // && actuatorProximal->pushpull_ready
        ) &&
           (
            actuatorDistal &&
               actuatorDistal->linear_ready
               // && actuatorDistal->rotation_ready
               // && actuatorDistal->pushpull_ready
               );
}

void Equipment6DoF::setProximalAct(actuation_t& act)
{
    if(actuatorProximal)
    {
        actuatorProximal->setLinearLength(act.translation);
        actuatorProximal->setPushPullLength(act.pull);
        actuatorProximal->setRotationDegAbs(RAD2DEG(act.rotation_angle));
    }
}

void Equipment6DoF::setDistalAct(actuation_t& act)
{
    if(actuatorDistal)
    {
        actuatorDistal->setLinearLength(act.translation);
        actuatorDistal->setPushPullLength(act.pull);
        actuatorDistal->setRotationDegAbs(RAD2DEG(act.rotation_angle));
    }
}

Point3D* Equipment6DoF::getProximalTarget()
{
    if(actuatorProximal) return &actuatorProximal->targetPoint;
    return nullptr;
}

Point3D* Equipment6DoF::getDistalTarget()
{
    if(actuatorDistal) return &actuatorDistal->targetPoint;
    return nullptr;
}

bool Equipment6DoF::parseJsonFromObject(const QJsonObject& object, QHash<QString, QSharedPointer<Motor>>& hash)
{
    int actuatorErrorCount = 0;
    for(auto i = object.constBegin(); i != object.constEnd(); ++i)
    {
        QSharedPointer<Actuator3DoF> *target;
        if(i.key().toLower() == "proximal")
        {
            actuatorProximal = QSharedPointer<Actuator3DoF>(new Actuator3DoF(i.key()));
            target = &actuatorProximal;
        }
        else if(i.key().toLower() == "distal")
        {
            actuatorDistal = QSharedPointer<Actuator3DoF>(new Actuator3DoF(i.key()));
            target = &actuatorDistal;
        }
        else
        {
            qDebugMessage("Received unsupported actuator name(supports Proximal/Distal): " + i.key());
            actuatorErrorCount++;
            continue;
        }
        qDebugMessage("Parsing Actuator: " + target->get()->actuatorName());
        if(!target->get()->parseJsonFromObject(i.value().toObject(), hash))
        {
            qDebugMessage("Error parsing Actuator: " + target->get()->actuatorName());
            actuatorErrorCount++;
        }
    }
    if(actuatorErrorCount > 0) return false;
    else qDebugMessage("Equipment " + name + " successfully initialized");
    return true;
}

QString& Equipment6DoF::equipmentName()
{
    return name;
}

QWeakPointer<Actuator3DoF> Equipment6DoF::getProximal()
{
    return actuatorProximal.toWeakRef();
}

QWeakPointer<Actuator3DoF> Equipment6DoF::getDistal()
{
    return actuatorDistal.toWeakRef();
}
