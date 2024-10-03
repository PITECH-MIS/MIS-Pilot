#ifndef CONTROLLERWINDOW_H
#define CONTROLLERWINDOW_H

#include <QMainWindow>
#include <QJoysticks.h>
#include <QStringList>
#include <QMap>
#include <QCloseEvent>
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
    void onPOVEvent(const QJoystickPOVEvent &event);
    void onAxisEvent(const QJoystickAxisEvent &event);
    void onButtonEvent(const QJoystickButtonEvent &event);
    void onClickDebugEnable();
    void onSelectDebugMode();
    void onSendDebugAction();
    void onSelectDebugMotor();
    void showWindow();
    void controlLoop();
signals:
    void debugMessage(QString msg);
    void infoMessage(QString msg);
    void errorMessage(QString msg);
    void closed();
private:
    Ui::ControllerWindow *ui;
    QSet<QString> motorSNSet;
    QMap<QString, Motor*> motorMap;
    Motor* currentDebuggingMotor = nullptr;
};

#endif // CONTROLLERWINDOW_H
