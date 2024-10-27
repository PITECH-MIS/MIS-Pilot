#include "TrayIcon.h"

TrayIcon::TrayIcon(QIcon& icon, QWidget *parent)
    : QWidget{parent}, icon(icon)
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    if(parent) trayIcon->setToolTip(parent->whatsThis());
    trayIcon->show();
}

void TrayIcon::debugMessage(QString msg)
{
    showPopupIcon(QSystemTrayIcon::Information, "Debug", msg, 1000);
}

void TrayIcon::infoMessage(QString msg)
{

}

void TrayIcon::errorMessage(QString msg)
{
    showPopupIcon(QSystemTrayIcon::Warning, "Error", msg, 1000);
}

void TrayIcon::showPopupIcon(QSystemTrayIcon::MessageIcon icon, QString title, QString body, int msecs)
{
    trayIcon->showMessage(title, body, icon, msecs);
}
