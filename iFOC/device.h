#ifndef DEVICE_H
#define DEVICE_H

#include "equipment.h"
#include <QPair>
#include <QVector>
#include <QString>

class Device
{
public:
    Device();
private:
    QVector<QPair<QString, QSharedPointer<Equipment>>> equipmentVector;
};

#endif // DEVICE_H
