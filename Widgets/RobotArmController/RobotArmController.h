#ifndef ROBOTARMCONTROLLER_H
#define ROBOTARMCONTROLLER_H

#include <QWidget>

namespace Ui {
class RobotArmController;
}

class RobotArmController : public QWidget
{
    Q_OBJECT

public:
    explicit RobotArmController(QWidget *parent = nullptr);
    ~RobotArmController();

private:
    Ui::RobotArmController *ui;
};

#endif // ROBOTARMCONTROLLER_H
