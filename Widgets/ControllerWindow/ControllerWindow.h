#ifndef CONTROLLERWINDOW_H
#define CONTROLLERWINDOW_H

#include <QMainWindow>
#include <QJoysticks.h>
#include <QStringList>
#include <QMap>
#include <QCloseEvent>
#include "Kinematics/KinematicsBase.h"
#include "../MotorDebugger/MotorDebugger.h"
#include "../../EtherCAT/ECATWrapper.h"
#include "../../iFOC/Motor.h"
#include "../../iFOC/Device.h"
#include "../EndoscopeView/EndoscopeView.h"

namespace Ui {
class ControllerWindow;
}

class ControllerWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ControllerWindow(ECATWrapper* w, QMap<QString, QJoystickDevice*>& j, QWidget *parent = nullptr);
    ~ControllerWindow();
    ECATWrapper* wrapper;
    QMap<QString, QJoystickDevice*>& joysticks;
public slots:
    void showWindow();
    void controlLoop();
private slots:
    void onPOVEvent(const QJoystickPOVEvent &event);
    void onAxisEvent(const QJoystickAxisEvent &event);
    void onButtonEvent(const QJoystickButtonEvent &event);
    void onSelectDescJSONPath();
    void onEnableMotorDebugger();
    void onSelectDevice();
    void onSelectEquipment();
    void onSelectActuator();
signals:
    void debugMessage(QString msg);
    void infoMessage(QString msg);
    void errorMessage(QString msg);
    void onCloseWindow();
private:
    void readRecordJson();  // for aligning-record
    void writeRecordJson();
    void updatePanelStatus();
    void updateControlCoord();
    void contextMenuEvent(QContextMenuEvent *event) final;
    void showEvent(QShowEvent *event);
    void updateCameraRotation(EndoscopeView *view);
    Ui::ControllerWindow *ui;
    MotorDebugger* debuggerWindow = nullptr;
    QHash<QString, QSharedPointer<Device>> deviceHashMap;
    QSet<QString> motorSNSet;
    QHash<QString, QSharedPointer<Motor>> motorHashMap;
    QSharedPointer<Actuator3DoF> panelActuator;
    QString currentPath;
    QTimer* speedTimer;
    QString leftEquipmentName = "";
    QString rightEquipmentName = "";
    QJsonObject recordJsonObject;
    const float desiredLinSpeed = 2.0f;
    float leftProxLinSpeed = 0.0f;
    float leftDistLinSpeed = 0.0f;
    float rightProxLinSpeed = 0.0f;
    float rightDistLinSpeed = 0.0f;
    Vector2D leftPOVSpeed;
    Vector2D rightPOVSpeed;
    bool isOverrideReady = false;
};

#endif // CONTROLLERWINDOW_H
