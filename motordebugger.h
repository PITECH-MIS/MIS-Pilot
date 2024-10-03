#ifndef MOTORDEBUGGER_H
#define MOTORDEBUGGER_H

#include <QWidget>

namespace Ui {
class MotorDebugger;
}

class MotorDebugger : public QWidget
{
    Q_OBJECT

public:
    explicit MotorDebugger(QWidget *parent = nullptr);
    ~MotorDebugger();

private:
    Ui::MotorDebugger *ui;
};

#endif // MOTORDEBUGGER_H
