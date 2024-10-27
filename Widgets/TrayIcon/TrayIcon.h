#ifndef TRAYICON_H
#define TRAYICON_H

#include <QObject>
#include <QWidget>
#include <QSystemTrayIcon>

class TrayIcon : public QWidget
{
    Q_OBJECT
public:
    explicit TrayIcon(QIcon& icon, QWidget *parent = nullptr);

public slots:
    void debugMessage(QString msg);
    void infoMessage(QString msg);
    void errorMessage(QString msg);

private:
    void showPopupIcon(QSystemTrayIcon::MessageIcon icon, QString title, QString body, int msecs = 1000);
    QSystemTrayIcon *trayIcon;
    QIcon& icon;
};

#endif // TRAYICON_H
