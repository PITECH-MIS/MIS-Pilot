#ifndef MOTORDEBUGGER_H
#define MOTORDEBUGGER_H

#include <QMainWindow>
#include <QWidget>
#include "../../iFOC/Motor.h"

namespace Ui {
class MotorDebugger;
}

class MotorDebugger : public QMainWindow
{
    Q_OBJECT

public:
    explicit MotorDebugger(QHash<QString, QSharedPointer<Motor>>& hashMap, QWidget *parent = nullptr);
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
    void onCloseWindow();
private:
    void setTargetSpinBoxesEnabled(bool en_torque, bool en_speed, bool en_pos, bool en_currLimit);
    Ui::MotorDebugger *ui;
    QHash<QString, QSharedPointer<Motor>>& motorHashMap;
    QMap<Motor::Mode, QString> motorModeNames;
    QSharedPointer<Motor> currentDbgMotor = nullptr;
    // QTimer *timer;
    int m_index = 0;
};

#endif // MOTORDEBUGGER_H
