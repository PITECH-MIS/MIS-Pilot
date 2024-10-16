#include "utils.h"
#include <QDateTime>
#include <QGuiApplication>
#include <QScreen>
#include <QCursor>
#include <QWidget>

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

void centerOnCursorScreen(QWidget* widget)
{
    auto screen = QGuiApplication::screenAt(QCursor::pos());
    auto resolution = screen->geometry();
    auto x = (resolution.width() - widget->width()) * 0.5f + screen->availableGeometry().left();
    auto y = (resolution.height() - widget->height()) * 0.5f + screen->availableGeometry().top();
    widget->move(x, y);
    widget->setGeometry(x, y, widget->width(), widget->height());
}
