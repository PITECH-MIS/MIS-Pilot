#include "motordebugger.h"
#include "ui_motordebugger.h"
#include <QCollator>

MotorDebugger::MotorDebugger(QHash<QString, QSharedPointer<Motor>>& hashMap, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MotorDebugger), motorHashMap(hashMap)
{
    motorModeNames[Motor::MODE_TORQUE] = "Torque Mode";
    motorModeNames[Motor::MODE_SPEED] = "Speed Mode";
    motorModeNames[Motor::MODE_TRAJECTORY] = "Traj. Mode";
    ui->setupUi(this);
    ui->motorModeComboBox->addItems(motorModeNames.values());
    setTargetSpinBoxesEnabled(false, false, false, false);
    connect(ui->motorSNComboBox, &QComboBox::currentIndexChanged, this, &MotorDebugger::onSelectMotorSN);
    connect(ui->motorEnabledCheckBox, &QCheckBox::checkStateChanged, this, &MotorDebugger::onChangeMotorEnable);
    connect(ui->motorModeComboBox, &QComboBox::currentIndexChanged, this, &MotorDebugger::onSelectMotorMode);
    connect(ui->motorTargetCurrLimitSpinBox, &QDoubleSpinBox::valueChanged, this, &MotorDebugger::onChangeMotorCurrLimit);
    connect(ui->motorTargetTorqueSpinBox, &QDoubleSpinBox::valueChanged, this, &MotorDebugger::onChangeMotorTarget);
    connect(ui->motorTargetSpeedSpinBox, &QDoubleSpinBox::valueChanged, this, &MotorDebugger::onChangeMotorTarget);
    connect(ui->motorTargetPosSpinBox, &QDoubleSpinBox::valueChanged, this, &MotorDebugger::onChangeMotorTarget);
}

void MotorDebugger::showWindow()
{
    currentDbgMotor.clear();
    ui->motorSNComboBox->clear();
    auto snList = motorHashMap.keys();
    QCollator collator;
    collator.setNumericMode(true);
    std::sort(snList.begin(), snList.end(), collator);
    ui->motorSNComboBox->addItems(snList);
    if(snList.size() > 0) onSelectMotorSN();
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    if(this->parentWidget())
    {
        auto parentX = this->parentWidget()->x();
        auto parentY = this->parentWidget()->y();
        this->move(parentX + this->parentWidget()->width(), parentY);
    }
    this->show();
}

void MotorDebugger::onSelectMotorSN()
{
    currentDbgMotor = motorHashMap.value(ui->motorSNComboBox->currentText());
    if(currentDbgMotor)
    {
        ui->motorEnabledCheckBox->setChecked(currentDbgMotor->getState());
        Motor::Mode motorMode = currentDbgMotor->getMode();
        ui->motorModeComboBox->setCurrentText(motorModeNames.value(motorMode, "Invalid Mode"));
        ui->motorTargetCurrLimitSpinBox->setValue(currentDbgMotor->getCurrentLimit());
        ui->motorTargetTorqueSpinBox->setValue(0.0f);
        float curr_spd = currentDbgMotor->getSpeed();
        if(abs(curr_spd) <= 5.0f || motorMode != Motor::Mode::MODE_SPEED) curr_spd = 0.0f;
        ui->motorTargetSpeedSpinBox->setValue(curr_spd);
        float curr_pos = currentDbgMotor->getPosDeg();
        if(abs(curr_pos) <= 5.0f || motorMode != Motor::Mode::MODE_TRAJECTORY || motorMode != Motor::Mode::MODE_POSITION) curr_pos = 0.0f;
        ui->motorTargetPosSpinBox->setValue(curr_pos);
        updateState();
        emit debugMessage("Change debug target motor: " + ui->motorSNComboBox->currentText());
    }
    else emit errorMessage("Debug target motor not exists");
}

void MotorDebugger::onChangeMotorEnable(Qt::CheckState state)
{
    if(!(currentDbgMotor && currentDbgMotor->setState((Motor::State)ui->motorEnabledCheckBox->isChecked())))
    {
        emit errorMessage("Error");
        disconnect(ui->motorEnabledCheckBox, &QCheckBox::checkStateChanged, this, &MotorDebugger::onChangeMotorEnable);
        ui->motorEnabledCheckBox->setCheckState(state == Qt::Unchecked ? Qt::Checked : Qt::Unchecked);
        connect(ui->motorEnabledCheckBox, &QCheckBox::checkStateChanged, this, &MotorDebugger::onChangeMotorEnable);
    }
}

void MotorDebugger::onSelectMotorMode()
{
    Motor::Mode setMode = motorModeNames.key(ui->motorModeComboBox->currentText(), Motor::Mode::MODE_TORQUE);
    if(!(currentDbgMotor && currentDbgMotor->setMode(setMode))) emit errorMessage("Error");
}

void MotorDebugger::onChangeMotorCurrLimit()
{
    if(!(currentDbgMotor && currentDbgMotor->setCurrentLimit(ui->motorTargetCurrLimitSpinBox->value()))) emit errorMessage("Error");
}

void MotorDebugger::onChangeMotorTarget()
{
    if(!currentDbgMotor)
    {
        emit errorMessage("Error");
        return;
    }
    switch(currentDbgMotor->getMode())
    {
    case Motor::Mode::MODE_TORQUE:
        if(currentDbgMotor->setTorque(ui->motorTargetTorqueSpinBox->value())) return;
        break;
    case Motor::Mode::MODE_SPEED:
        if(currentDbgMotor->setSpeed(ui->motorTargetSpeedSpinBox->value())) return;
        break;
    case Motor::Mode::MODE_POSITION:
        if(currentDbgMotor->setPosAbsAngle(ui->motorTargetPosSpinBox->value())) return;
        break;
    case Motor::Mode::MODE_TRAJECTORY:
        if(currentDbgMotor->setTrajAbsAngle(ui->motorTargetPosSpinBox->value())) return;
        break;
    default: break;
    }
    emit errorMessage("Error");
}

void MotorDebugger::updateState()
{
    if(currentDbgMotor)
    {
        switch(currentDbgMotor->getMode())
        {
        case Motor::Mode::MODE_TORQUE:
            setTargetSpinBoxesEnabled(true, false, false, true);
            break;
        case Motor::Mode::MODE_SPEED:
            setTargetSpinBoxesEnabled(false, true, false, true);
            break;
        case Motor::Mode::MODE_POSITION:
        case Motor::Mode::MODE_TRAJECTORY:
            setTargetSpinBoxesEnabled(false, false, true, true);
            break;
        default: break;
        }
        ui->motorStateIsAliveButton->setChecked(currentDbgMotor->checkAlive());
        ui->motorStateEnabledButton->setChecked(currentDbgMotor->getState());
        ui->motorStateModeSlider->setValue((int)currentDbgMotor->getMode());
        ui->motorStateSNEdit->setText(QString::number(currentDbgMotor->getSN()));
        ui->motorStateErrorCodeEdit->setText(QString::number(currentDbgMotor->getErrorCode()));
        ui->motorStateLimiterEdit->setText(QString::number(currentDbgMotor->getRawLimiterState()));
        ui->motorStateVBusEdit->setText(QString::number(currentDbgMotor->getVBus()));
        ui->motorStateIBusEdit->setText(QString::number(currentDbgMotor->getIBus()));
        ui->motorStateIqEdit->setText(QString::number(currentDbgMotor->getIq()));
        ui->motorStateIdEdit->setText(QString::number(currentDbgMotor->getId()));
        ui->motorStateEstSpeedEdit->setText(QString::number(currentDbgMotor->getSpeed()));
        ui->motorStateEstAngleEdit->setText(QString::number(currentDbgMotor->getPosDeg()));
        ui->motorStateCurrLimitEdit->setText(QString::number(currentDbgMotor->getCurrentLimit()));
        ui->motorStateMCUTempEdit->setText(QString::number(currentDbgMotor->getMCUTemp()));
    }
}

void MotorDebugger::setTargetSpinBoxesEnabled(bool en_torque, bool en_speed, bool en_pos, bool en_currLimit)
{
    ui->motorTargetTorqueSpinBox->setEnabled(en_torque);
    ui->motorTargetSpeedSpinBox->setEnabled(en_speed);
    ui->motorTargetPosSpinBox->setEnabled(en_pos);
    ui->motorTargetCurrLimitSpinBox->setEnabled(en_currLimit);
}

MotorDebugger::~MotorDebugger()
{
    delete ui;
}
