#ifndef DEVICE_H
#define DEVICE_H

#include "Equipment6DoF.h"
#include <QString>
#include <QJsonDocument>
#include <QFile>

class Device
{
public:
    Device() {};
    ~Device();
    // copy of hashmap, enabling removing motor pointers that are already assigned.
    bool parseJsonFromFile(const QString& absPath, QHash<QString, QSharedPointer<Motor>> hash);
    QString& deviceName();
    QWeakPointer<Equipment6DoF> getEquipmentByName(QString name);
    QStringList equipmentNames();
    int availbleEquipmentCount();
private:
    QString name = "";
    QHash<QString, QSharedPointer<Equipment6DoF>> equipmentHashMap;
};

#endif // DEVICE_H
