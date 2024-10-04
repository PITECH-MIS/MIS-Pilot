#ifndef DEVICE_H
#define DEVICE_H

#include "Equipment6DoF.h"
#include <QPair>
#include <QVector>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

class Device
{
public:
    Device();
private:
    bool parseJsonFromFile(const QString& absPath);
    QVector<QPair<QString, QSharedPointer<Equipment6DoF>>> equipmentVector;
};

#endif // DEVICE_H
