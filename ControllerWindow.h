#ifndef CONTROLLERWINDOW_H
#define CONTROLLERWINDOW_H

#include <QMainWindow>
#include <QJoysticks.h>
#include <QStringList>
#include <QMap>
#include <QCloseEvent>
#include "Kinematics/KinematicsBase.h"
#include "MotorDebugger.h"
#include "ECATWrapper.h"
#include "iFOC/Motor.h"
#include "iFOC/Device.h"

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
    void updatePanelStatus();
    void updateControlCoord();
    void contextMenuEvent(QContextMenuEvent *event) final;
    Ui::ControllerWindow *ui;
    MotorDebugger* debuggerWindow = nullptr;
    QHash<QString, QSharedPointer<Device>> deviceHashMap;
    QSet<QString> motorSNSet;
    QHash<QString, QSharedPointer<Motor>> motorHashMap;
    QSharedPointer<Actuator3DoF> panelActuator;
    QString currentPath;
    QTimer* speedTimer;
    QSharedPointer<Equipment6DoF> leftEquipment;
    QSharedPointer<Equipment6DoF> rightEquipment;
    KinematicsBase *leftKinematics;
    KinematicsBase *rightKinematics;
    Vector2D leftPOVSpeed;
    Vector2D rightPOVSpeed;
    Point3D leftProxCoord;
    Point3D leftDistCoord;
    Point3D rightProxCoord;
    Point3D rightDistCoord;
};

#endif // CONTROLLERWINDOW_H
