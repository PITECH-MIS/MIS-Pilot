#ifndef CONTROLLERWINDOW_H
#define CONTROLLERWINDOW_H

#include <QMainWindow>
#include <QJoysticks.h>
#include <QStringList>
#include <QMap>
#include <QCloseEvent>
#include "motordebugger.h"
#include "ecatwrapper.h"
#include "iFOC/motor.h"

namespace Ui {
class ControllerWindow;
}

class ControllerWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ControllerWindow(ECATWrapper& w, QMap<QString, QJoystickDevice*>& j, QWidget *parent = nullptr);
    ~ControllerWindow();
    ECATWrapper& wrapper;
    QMap<QString, QJoystickDevice*>& joysticks;
    void closeEvent(QCloseEvent *event) override;
public slots:
    void showWindow();
    void controlLoop();
private slots:
    void onPOVEvent(const QJoystickPOVEvent &event);
    void onAxisEvent(const QJoystickAxisEvent &event);
    void onButtonEvent(const QJoystickButtonEvent &event);
    void onEnableMotorDebugger();
signals:
    void debugMessage(QString msg);
    void infoMessage(QString msg);
    void errorMessage(QString msg);
    void closed();
private:
    Ui::ControllerWindow *ui;
    MotorDebugger* debuggerWindow = nullptr;
    QSet<QString> motorSNSet;
    QHash<QString, QSharedPointer<Motor>> motorHashMap;
};

#endif // CONTROLLERWINDOW_H
