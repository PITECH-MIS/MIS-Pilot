#ifndef MOTORDEBUGGER_H
#define MOTORDEBUGGER_H

#include <QWidget>
#include "iFOC/motor.h"

namespace Ui {
class MotorDebugger;
}

class MotorDebugger : public QWidget
{
    Q_OBJECT

public:
    explicit MotorDebugger(QMap<QString, QSharedPointer<Motor>>& map, QWidget *parent = nullptr);
    ~MotorDebugger();
public slots:
    void showWindow();
    void updateState();
private slots:
    void onSelectMotorSN();
    void onChangeMotorEnable();
    void onSelectMotorMode();
    void onChangeMotorTarget();
    void onChangeMotorCurrLimit();
signals:
    void debugMessage(QString msg);
    void infoMessage(QString msg);
    void errorMessage(QString msg);
private:
    void setTargetSpinBoxesEnabled(bool en_torque, bool en_speed, bool en_pos, bool en_currLimit);
    Ui::MotorDebugger *ui;
    QMap<QString, QSharedPointer<Motor>>& motorMap;
    QMap<QString, Motor::Mode> motorModeNames;
    QSharedPointer<Motor> currentDbgMotor = nullptr;
};

#endif // MOTORDEBUGGER_H
