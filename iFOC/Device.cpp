#include "Device.h"

bool Device::parseJsonFromFile(const QString& absPath)
{
    QFile file(absPath);
    if(!file.open(QIODevice::ReadOnly)) return false;
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if(!doc.isObject()) return false;
    QJsonObject rootObject = doc.object();
    return true;
}
