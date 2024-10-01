#include "utils.h"

QStringList filterEthWords = {"Miniport", "Virtual", "PPPoE", "Wi-Fi", "WiFi", "Bluetooth", "Tunnel", "VPN", "TEST", "loop"};

bool isUsableEth(QString& origin)
{
    for(const QString &word : filterEthWords)
    {
        if(origin.contains(word, Qt::CaseInsensitive)) return false;
    }
    return true;
}
