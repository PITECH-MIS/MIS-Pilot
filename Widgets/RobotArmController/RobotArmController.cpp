#include "RobotArmController.h"
#include "ui_RobotArmController.h"

RobotArmController::RobotArmController(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RobotArmController)
{
    ui->setupUi(this);
    connect(ui->connectButton, &QPushButton::clicked, this, &RobotArmController::onConnectToRobotArm);
    ui->modeComboBox->addItem("DRAG");
    ui->modeComboBox->addItem("JOG");
    connect(ui->modeButton, &QPushButton::clicked, this, [this](){
        if(wrapper->getState() == RobotArmWrapper::RobotState::CONNECTED)
        {
            if(ui->modeButton->text() == "Enable")
            {
                wrapper->setMode(getSelectedMode());
                ui->modeButton->setText("Disable");
                ui->modeComboBox->setEnabled(false);
            }
            else
            {
                wrapper->setMode(RobotArmWrapper::RobotMode::MODE_INIT);
                ui->modeButton->setText("Enable");
                ui->modeComboBox->setEnabled(true);
            }
        }
    });
    connect(ui->xPosButton, &QPushButton::clicked, this, [this](){
        if(wrapper->getMode() == RobotArmWrapper::RobotMode::MODE_JOG) wrapper->jog(rokae::JogOpt::flange, RobotArmWrapper::JogAxis::X, 0.75, (float)ui->linearStepSpinBox->value());
    });
    connect(ui->xNegButton, &QPushButton::clicked, this, [this](){
        if(wrapper->getMode() == RobotArmWrapper::RobotMode::MODE_JOG) wrapper->jog(rokae::JogOpt::flange, RobotArmWrapper::JogAxis::X, 0.75, -(float)ui->linearStepSpinBox->value());
    });
    connect(ui->yPosButton, &QPushButton::clicked, this, [this](){
        if(wrapper->getMode() == RobotArmWrapper::RobotMode::MODE_JOG) wrapper->jog(rokae::JogOpt::flange, RobotArmWrapper::JogAxis::Y, 0.75, (float)ui->linearStepSpinBox->value());
    });
    connect(ui->yNegButton, &QPushButton::clicked, this, [this](){
        if(wrapper->getMode() == RobotArmWrapper::RobotMode::MODE_JOG) wrapper->jog(rokae::JogOpt::flange, RobotArmWrapper::JogAxis::Y, 0.75, -(float)ui->linearStepSpinBox->value());
    });
    connect(ui->zPosButton, &QPushButton::clicked, this, [this](){
        if(wrapper->getMode() == RobotArmWrapper::RobotMode::MODE_JOG) wrapper->jog(rokae::JogOpt::flange, RobotArmWrapper::JogAxis::Z, 0.75, (float)ui->linearStepSpinBox->value());
    });
    connect(ui->zNegButton, &QPushButton::clicked, this, [this](){
        if(wrapper->getMode() == RobotArmWrapper::RobotMode::MODE_JOG) wrapper->jog(rokae::JogOpt::flange, RobotArmWrapper::JogAxis::Z, 0.75, -(float)ui->linearStepSpinBox->value());
    });
    connect(ui->aPosButton, &QPushButton::clicked, this, [this](){
        if(wrapper->getMode() == RobotArmWrapper::RobotMode::MODE_JOG) wrapper->jog(rokae::JogOpt::flange, RobotArmWrapper::JogAxis::Rx, 0.75, (float)ui->rotationStepSpinBox->value());
    });
    connect(ui->aNegButton, &QPushButton::clicked, this, [this](){
        if(wrapper->getMode() == RobotArmWrapper::RobotMode::MODE_JOG) wrapper->jog(rokae::JogOpt::flange, RobotArmWrapper::JogAxis::Rx, 0.75, -(float)ui->rotationStepSpinBox->value());
    });
    connect(ui->bPosButton, &QPushButton::clicked, this, [this](){
        if(wrapper->getMode() == RobotArmWrapper::RobotMode::MODE_JOG) wrapper->jog(rokae::JogOpt::flange, RobotArmWrapper::JogAxis::Ry, 0.75, (float)ui->rotationStepSpinBox->value());
    });
    connect(ui->bNegButton, &QPushButton::clicked, this, [this](){
        if(wrapper->getMode() == RobotArmWrapper::RobotMode::MODE_JOG) wrapper->jog(rokae::JogOpt::flange, RobotArmWrapper::JogAxis::Ry, 0.75, -(float)ui->rotationStepSpinBox->value());
    });
    connect(ui->cPosButton, &QPushButton::clicked, this, [this](){
        if(wrapper->getMode() == RobotArmWrapper::RobotMode::MODE_JOG) wrapper->jog(rokae::JogOpt::flange, RobotArmWrapper::JogAxis::Rz, 0.75, (float)ui->rotationStepSpinBox->value());
    });
    connect(ui->cNegButton, &QPushButton::clicked, this, [this](){
        if(wrapper->getMode() == RobotArmWrapper::RobotMode::MODE_JOG) wrapper->jog(rokae::JogOpt::flange, RobotArmWrapper::JogAxis::Rz, 0.75, -(float)ui->rotationStepSpinBox->value());
    });
    connect(wrapper, &RobotArmWrapper::onStateChanged, this, &RobotArmController::onStateChanged);
}

RobotArmController::~RobotArmController()
{
    if(armThread)
    {
        armThread->quit();
        armThread->wait();
    }
    delete ui;
}

RobotArmWrapper::RobotMode RobotArmController::getSelectedMode()
{
    if(ui->modeComboBox->currentText() == "DRAG") return RobotArmWrapper::RobotMode::MODE_DRAG;
    else if(ui->modeComboBox->currentText() == "JOG") return RobotArmWrapper::RobotMode::MODE_JOG;
    return RobotArmWrapper::RobotMode::MODE_INIT;
}

void RobotArmController::onStateChanged()
{
    auto state = wrapper->getState();
    if(state == RobotArmWrapper::RobotState::CONNECTED)
    {
        ui->connectButton->setText("Disconnect");
        ui->controllerVersionLabel->setText("Controller Version: " + wrapper->version.controller);
        ui->robotTypeLabel->setText("Robot Type: " + wrapper->version.type);
        ui->sdkVersionLabel->setText("xCore SDK Version: " + wrapper->version.sdk);
        ui->stateLabel->setText("Connect State: True");
    }
    else if(state == RobotArmWrapper::RobotState::DISCONNECTED)
    {
        ui->connectButton->setText("Connect");
        ui->controllerVersionLabel->setText("Controller Version: " + wrapper->version.controller);
        ui->robotTypeLabel->setText("Robot Type: " + wrapper->version.type);
        ui->sdkVersionLabel->setText("xCore SDK Version: " + wrapper->version.sdk);
        ui->stateLabel->setText("Connect State: False");
    }
}

void RobotArmController::onEventLoop()
{
    if(wrapper->getState() == RobotArmWrapper::RobotState::CONNECTED)
    {
        ui->J1PosEdit->setText(QString::number(RAD2DEG(wrapper->joint_pos[0]), 'f', 2));
        ui->J1VelEdit->setText(QString::number(RAD2DEG(wrapper->joint_vel[0]), 'f', 2));
        ui->J1TorqueEdit->setText(QString::number(wrapper->joint_torque[0], 'f', 2));

        ui->J2PosEdit->setText(QString::number(RAD2DEG(wrapper->joint_pos[1]), 'f', 2));
        ui->J2VelEdit->setText(QString::number(RAD2DEG(wrapper->joint_vel[1]), 'f', 2));
        ui->J2TorqueEdit->setText(QString::number(wrapper->joint_torque[1], 'f', 2));

        ui->J3PosEdit->setText(QString::number(RAD2DEG(wrapper->joint_pos[2]), 'f', 2));
        ui->J3VelEdit->setText(QString::number(RAD2DEG(wrapper->joint_vel[2]), 'f', 2));
        ui->J3TorqueEdit->setText(QString::number(wrapper->joint_torque[2], 'f', 2));

        ui->J4PosEdit->setText(QString::number(RAD2DEG(wrapper->joint_pos[3]), 'f', 2));
        ui->J4VelEdit->setText(QString::number(RAD2DEG(wrapper->joint_vel[3]), 'f', 2));
        ui->J4TorqueEdit->setText(QString::number(wrapper->joint_torque[3], 'f', 2));

        ui->J5PosEdit->setText(QString::number(RAD2DEG(wrapper->joint_pos[4]), 'f', 2));
        ui->J5VelEdit->setText(QString::number(RAD2DEG(wrapper->joint_vel[4]), 'f', 2));
        ui->J5TorqueEdit->setText(QString::number(wrapper->joint_torque[4], 'f', 2));

        ui->J6PosEdit->setText(QString::number(RAD2DEG(wrapper->joint_pos[5]), 'f', 2));
        ui->J6VelEdit->setText(QString::number(RAD2DEG(wrapper->joint_vel[5]), 'f', 2));
        ui->J6TorqueEdit->setText(QString::number(wrapper->joint_torque[5], 'f', 2));

        ui->J7PosEdit->setText(QString::number(RAD2DEG(wrapper->joint_pos[6]), 'f', 2));
        ui->J7VelEdit->setText(QString::number(RAD2DEG(wrapper->joint_vel[6]), 'f', 2));
        ui->J7TorqueEdit->setText(QString::number(wrapper->joint_torque[6], 'f', 2));

        switch((uint8_t)wrapper->powerState)
        {
        case 0:
            ui->powerStateLabel->setText("ON");
            break;
        case 1:
            ui->powerStateLabel->setText("OFF");
            break;
        case 2:
            ui->powerStateLabel->setText("ESTOP");
            break;
        case 3:
            ui->powerStateLabel->setText("GSTOP");
            break;
        default:
            ui->powerStateLabel->setText("ERR");
            break;
        }
    }
}

void RobotArmController::eventLoop()
{
    wrapper->eventLoop();
}

void RobotArmController::onConnectToRobotArm()
{
    if(armTimer == nullptr)
    {
        if(armThread == nullptr) armThread = new QThread(this);
        armTimer = new QTimer();
        armTimer->moveToThread(armThread);
        armTimer->setTimerType(Qt::PreciseTimer);
        armTimer->setInterval(10);
        connect(armTimer, &QTimer::timeout, this, &RobotArmController::eventLoop, Qt::DirectConnection);
        connect(armThread, &QThread::finished, armTimer, &QTimer::stop);
        connect(armThread, &QThread::started, armTimer, QOverload<>::of(&QTimer::start));
        connect(wrapper, &RobotArmWrapper::onEventLoop, this, &RobotArmController::onEventLoop);
    }
    if(ui->connectButton->text() == "Connect")
    {
        wrapper->connect();
        armThread->start();
    }
    else if(ui->connectButton->text() == "Disconnect")
    {
        wrapper->disconnect();
    }
}
