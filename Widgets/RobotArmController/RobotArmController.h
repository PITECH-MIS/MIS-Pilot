#ifndef ROBOTARMCONTROLLER_H
#define ROBOTARMCONTROLLER_H

#include <QWidget>
#include <QThread>
#include <QTimer>
#include "../RobotArm/RobotArmWrapper.h"

namespace Ui {
class RobotArmController;
}

class RobotArmController : public QWidget
{
    Q_OBJECT
public:
    explicit RobotArmController(QWidget *parent = nullptr);
    ~RobotArmController();
private slots:
    void onConnectToRobotArm();
    void onStateChanged();
    void onEventLoop();
private:
    void eventLoop();
    RobotArmWrapper::RobotMode getSelectedMode();
    Ui::RobotArmController *ui;
    RobotArmWrapper *wrapper = RobotArmWrapper::getInstance();
    QThread *armThread = nullptr;
    QTimer *armTimer = nullptr;
};

#endif // ROBOTARMCONTROLLER_H
