#ifndef MOTORDEBUGGER_H
#define MOTORDEBUGGER_H

#include <QMainWindow>
#include <QWidget>
#include "iFOC/Motor.h"

namespace Ui {
class MotorDebugger;
}

class MotorDebugger : public QMainWindow
{
    Q_OBJECT

public:
    explicit MotorDebugger(QHash<QString, Motor*>& hashMap, QWidget *parent = nullptr);
    ~MotorDebugger();
public slots:
    void showWindow();
    void updateState();
private slots:
    void onSelectMotorSN();
    void onChangeMotorEnable(Qt::CheckState state);
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
    QHash<QString, Motor*>& motorHashMap;
    QMap<Motor::Mode, QString> motorModeNames;
    Motor *currentDbgMotor = nullptr;
};

#endif // MOTORDEBUGGER_H
