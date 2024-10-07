#include "Device.h"

Device::~Device()
{
    qDebugMessage("Device " + name + " destroyed");
}

bool Device::parseJsonFromFile(const QString& absPath, QHash<QString, QSharedPointer<Motor>> hash)
{
    QFile file(absPath);
    if(!file.open(QIODevice::ReadOnly)) return false;
    QByteArray data = file.readAll();
    QJsonParseError result;
    QJsonDocument doc = QJsonDocument::fromJson(data, &result);
    if(doc.isNull() || !doc.isObject() || result.error != QJsonParseError::NoError)
    {
        qDebugMessage("Error while parsing JSON: " + result.errorString());
        return false;
    }
    QJsonObject rootObject = doc.object();
    if(rootObject.contains("name")) name = rootObject.value("name").toString();
    qDebugMessage("Device name: " + name);
    int equipmentErrorCount = 0;
    if(rootObject.contains("equipments"))
    {
        QJsonObject equipmentsObject = rootObject.value("equipments").toObject();
        for(auto i = equipmentsObject.constBegin(); i != equipmentsObject.constEnd(); ++i)
        {
            QSharedPointer<Equipment6DoF> equipment = QSharedPointer<Equipment6DoF>(new Equipment6DoF(i.key()));
            qDebugMessage("Parsing Equipment: " + equipment->equipmentName());
            if(equipment->parseJsonFromObject(i.value().toObject(), hash))
            {
                equipmentHashMap.insert(equipment->equipmentName(), equipment);
            }
            else
            {
                qDebugMessage("Error parsing Equipment: " + equipment->equipmentName());
                equipment.clear();
                equipmentErrorCount++;
            }
        }
    }
    else return false;
    if(equipmentErrorCount > 0)
    {
        qDebugMessage("Not every Equipment of Device: " + name + ", filePath: " + absPath + " is successfully initialized");
        return false;
    }
    // else qDebugMessage("Device " + name + " successfully initialized");
    return true;
}

QWeakPointer<Equipment6DoF> Device::getEquipmentByName(QString name)
{
    return equipmentHashMap.value(name, nullptr).toWeakRef();
}

QStringList Device::equipmentNames()
{
    QStringList raw = equipmentHashMap.keys();
    raw.sort(Qt::CaseInsensitive);
    return raw;
}

QString& Device::deviceName()
{
    return name;
}

int Device::availbleEquipmentCount()
{
    return equipmentHashMap.size();
}
