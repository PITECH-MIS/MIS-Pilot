#include "utils.h"
#include <QDateTime>

QStringList filterEthWords = {"Miniport", "Virtual", "PPPoE", "Wi-Fi", "WiFi", "Bluetooth", "Tunnel", "VPN", "TEST", "loop"};

bool isUsableEth(QString& origin)
{
    for(const QString &word : filterEthWords)
    {
        if(origin.contains(word, Qt::CaseInsensitive)) return false;
    }
    return true;
}

void qDebugMessage(QString msg)
{
    QDateTime currentDt = QDateTime::currentDateTime();
    QString result = "[" + currentDt.toString("yyyy-MM-dd hh:mm:ss.zzz") + "] [DEBUG] " + msg;
    qDebug() << result;
}
