#include "ControllerWindow.h"
#include "ui_ControllerWindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QElapsedTimer>
#include "Kinematics/KinematicsAtan2.h"

ControllerWindow::ControllerWindow(ECATWrapper& w, QMap<QString, QJoystickDevice*>& j, QWidget *parent)
    : QMainWindow(parent), wrapper(w), joysticks(j)
    , ui(new Ui::ControllerWindow)
{
    currentPath = QCoreApplication::applicationDirPath();
    ui->setupUi(this);
    speedTimer = new QTimer(this);
    speedTimer->setTimerType(Qt::CoarseTimer);
    connect(speedTimer, &QTimer::timeout, this, &ControllerWindow::updateControlCoord);
    connect(QJoysticks::getInstance(), &QJoysticks::POVEvent, this, &ControllerWindow::onPOVEvent);
    connect(QJoysticks::getInstance(), &QJoysticks::axisEvent, this, &ControllerWindow::onAxisEvent);
    connect(QJoysticks::getInstance(), &QJoysticks::buttonEvent, this, &ControllerWindow::onButtonEvent);
    connect(ui->menuDebugger, &QMenu::aboutToShow, this, &ControllerWindow::onEnableMotorDebugger);
    connect(ui->menuLoadDescFiles, &QMenu::aboutToShow, this, &ControllerWindow::onSelectDescJSONPath);
    connect(ui->deviceSelectComboBox, &QComboBox::currentIndexChanged, this, &ControllerWindow::onSelectDevice);
    connect(ui->equipmentSelectComboBox, &QComboBox::currentIndexChanged, this, &ControllerWindow::onSelectEquipment);
    connect(ui->actuatorSelectComboBox, &QComboBox::currentIndexChanged, this, &ControllerWindow::onSelectActuator);
    connect(ui->panelRotationSpinBox, &QDoubleSpinBox::valueChanged, this, [this](){
        if(panelActuator) panelActuator->setRotationDegAbs(ui->panelRotationSpinBox->value());
    });
    connect(ui->panelPushPullSpinBox, &QDoubleSpinBox::valueChanged, this, [this](){
        if(panelActuator) panelActuator->setPushPullDegAbs(ui->panelPushPullSpinBox->value());
    });
    connect(ui->panelLinearSpinBox, &QDoubleSpinBox::valueChanged, this, [this](){
        if(panelActuator) panelActuator->setLinearDegAbs(ui->panelLinearSpinBox->value());
    });
    connect(ui->panelLinearAlignButton, &QPushButton::clicked, this, [this](){
        if(panelActuator) panelActuator->beginLinearHoming();
    });
    connect(ui->panelRotationAlignButton, &QPushButton::clicked, this, [this](){
        if(panelActuator) panelActuator->beginRotationHoming();
    });
    connect(ui->panelPushPullAlignButton, &QPushButton::clicked, this, [this](){
        if(panelActuator) panelActuator->beginPushPullHoming();
    });
    connect(ui->panelPreHomingButton, &QPushButton::clicked, this, [this](){
        if(panelActuator) panelActuator->beginPreInstallHoming();
    });
    connect(ui->panelPostHomingButton, &QPushButton::clicked, this, [this](){
        if(panelActuator) panelActuator->beginPostInstallHoming();
    });
    speedTimer->start(100);
    leftKinematics = new KinematicsAtan2();
    rightKinematics = new KinematicsAtan2();
}

void ControllerWindow::updateControlCoord()
{
    static QElapsedTimer speedTimer;
    float timeElapsed = 0.0f;
    if(!speedTimer.isValid()) speedTimer.start();
    else
    {
        timeElapsed = ((float)speedTimer.elapsed()) / 1000.0f;
        speedTimer.restart();
    }
    float multiplier_left = (float)ui->leftSpeedSlider->value() * 0.25f;
    leftProxCoord.x += ui->leftJoyPad->x() * timeElapsed * multiplier_left;
    leftProxCoord.y += ui->leftJoyPad->y() * timeElapsed * multiplier_left;
    leftDistCoord.x += leftPOVSpeed.x * timeElapsed * multiplier_left;
    leftDistCoord.y += leftPOVSpeed.y * timeElapsed * multiplier_left;
    if(leftEquipment)
    {
        leftKinematics->proximal_params = &leftEquipment->getProximal().lock()->kineParams;
        leftKinematics->distal_params = &leftEquipment->getDistal().lock()->kineParams;
    }
    leftKinematics->calculate(leftProxCoord, leftDistCoord);
    if(leftKinematics->proximal_params)
    {
        ui->leftProxCoordJoyPad->setX(leftProxCoord.x / leftKinematics->proximal_params->max_abs_pushpull);
        ui->leftProxCoordJoyPad->setY(leftProxCoord.y / leftKinematics->proximal_params->max_abs_pushpull);
    }
    if(leftKinematics->distal_params)
    {
        ui->leftDistCoordJoyPad->setX(leftDistCoord.x / leftKinematics->distal_params->max_abs_pushpull);
        ui->leftDistCoordJoyPad->setY(leftDistCoord.y / leftKinematics->distal_params->max_abs_pushpull);
    }
    ui->leftProxXLineEdit->setText(QString::asprintf("%.3f", leftProxCoord.x));
    ui->leftProxYLineEdit->setText(QString::asprintf("%.3f", leftProxCoord.y));
    ui->leftProxLinearLineEdit->setText(QString::asprintf("%.3f", leftProxCoord.z));
    ui->leftDistXLineEdit->setText(QString::asprintf("%.3f", leftDistCoord.x));
    ui->leftDistYLineEdit->setText(QString::asprintf("%.3f", leftDistCoord.y));
    ui->leftDistLinearLineEdit->setText(QString::asprintf("%.3f", leftDistCoord.z));
    // qDebugMessage(QString::asprintf("Left: angle: %.2f, pull: %.2f", RAD2DEG(leftKinematics->proximal_act.rotation_angle), leftKinematics->proximal_act.pull));
    if(leftEquipment && leftEquipment->isAllReady())
    {
        leftEquipment->setProximalAct(leftKinematics->proximal_act);
        leftEquipment->setDistalAct(leftKinematics->distal_act);
    }
    // qDebugMessage(QString::asprintf("Prox: p: %.2f, a: %.2f", leftKinematics->proximal_act.pull, RAD2DEG(leftKinematics->proximal_act.rotation_angle)));
    float multiplier_right = (float)ui->rightSpeedSlider->value() * 0.25f;
    rightProxCoord.x += ui->rightJoyPad->x() * timeElapsed * multiplier_right;
    rightProxCoord.y += ui->rightJoyPad->y() * timeElapsed * multiplier_right;
    rightDistCoord.x += rightPOVSpeed.x * timeElapsed * multiplier_right;
    rightDistCoord.y += rightPOVSpeed.y * timeElapsed * multiplier_right;
    if(rightEquipment)
    {
        rightKinematics->proximal_params = &rightEquipment->getProximal().lock()->kineParams;
        rightKinematics->distal_params = &rightEquipment->getDistal().lock()->kineParams;
    }
    rightKinematics->calculate(rightProxCoord, rightDistCoord);
    if(rightKinematics->proximal_params)
    {
        ui->rightProxCoordJoyPad->setX(rightProxCoord.x / rightKinematics->proximal_params->max_abs_pushpull);
        ui->rightProxCoordJoyPad->setY(rightProxCoord.y / rightKinematics->proximal_params->max_abs_pushpull);
    }
    if(rightKinematics->distal_params)
    {
        ui->rightDistCoordJoyPad->setX(rightDistCoord.x / rightKinematics->distal_params->max_abs_pushpull);
        ui->rightDistCoordJoyPad->setY(rightDistCoord.y / rightKinematics->distal_params->max_abs_pushpull);
    }
    ui->rightProxXLineEdit->setText(QString::asprintf("%.3f", rightProxCoord.x));
    ui->rightProxYLineEdit->setText(QString::asprintf("%.3f", rightProxCoord.y));
    ui->rightProxLinearLineEdit->setText(QString::asprintf("%.3f", rightProxCoord.z));
    ui->rightDistXLineEdit->setText(QString::asprintf("%.3f", rightDistCoord.x));
    ui->rightDistYLineEdit->setText(QString::asprintf("%.3f", rightDistCoord.y));
    ui->rightDistLinearLineEdit->setText(QString::asprintf("%.3f", rightDistCoord.z));
    if(rightEquipment && rightEquipment->isAllReady())
    {
        rightEquipment->setProximalAct(rightKinematics->proximal_act);
        rightEquipment->setDistalAct(rightKinematics->distal_act);
    }
}

void ControllerWindow::showWindow()
{
    ui->leftJoyComboBox->addItems(joysticks.keys());
    ui->rightJoyComboBox->addItems(joysticks.keys());
    if(joysticks.size() > 0)
    {
        ui->leftJoyComboBox->setCurrentIndex(0);
        if(joysticks.size() > 1) ui->rightJoyComboBox->setCurrentIndex(1);
    }
    else
    {
        ui->leftJoyPad->isVirtualJoystick = true;
        ui->rightJoyPad->isVirtualJoystick = true;
        ui->leftAuxJoyPad->isVirtualJoystick = true;
        ui->rightAuxJoyPad->isVirtualJoystick = true;
        ui->leftJoyComboBox->setEnabled(false);
        ui->rightJoyComboBox->setEnabled(false);
        emit infoMessage("No physics joysticks found, now the left/right joysticks behave virtually");
    }
    motorSNSet.clear();
    motorHashMap.clear();
    motorSNSet = getMotorSN(wrapper.input_vector);
    emit infoMessage(QString::asprintf("Find %d motor(s) on EtherCAT Bus", motorSNSet.size()));
    for(const auto &i : std::as_const(motorSNSet))
    {
        // emit debugMessage("Find motor SN: " + i);
        uint8_t limiter_index = 0;
        QSharedPointer<Motor> motor = QSharedPointer<Motor>(new Motor(i.toUInt(), limiter_index));
        if(motor->findMotorInVector(wrapper.input_vector, wrapper.output_vector))
        {
            motor->resetState();
            motorHashMap.insert(i, motor);
            emit debugMessage("Successfully mapped motor SN: " + i);
        }
        else emit errorMessage("Error mapping motor SN:" + i);
    }
    this->show();
}

void ControllerWindow::onSelectDevice()
{
    ui->equipmentSelectComboBox->clear();
    QWeakPointer<Device> dev(deviceHashMap.value(ui->deviceSelectComboBox->currentText()));
    ui->equipmentSelectComboBox->addItems(dev.lock()->equipmentNames());
}

void ControllerWindow::onSelectEquipment()
{
    ui->actuatorSelectComboBox->clear();
    QWeakPointer<Device> dev(deviceHashMap.value(ui->deviceSelectComboBox->currentText()));
    QWeakPointer<Equipment6DoF> eq(dev.lock()->getEquipmentByName(ui->equipmentSelectComboBox->currentText()));
    if(!eq.lock()->getProximal().isNull()) ui->actuatorSelectComboBox->addItem("Proximal");
    if(!eq.lock()->getDistal().isNull()) ui->actuatorSelectComboBox->addItem("Distal");
}

void ControllerWindow::onSelectActuator()
{
    QWeakPointer<Device> dev(deviceHashMap.value(ui->deviceSelectComboBox->currentText()));
    QWeakPointer<Equipment6DoF> eq(dev.lock()->getEquipmentByName(ui->equipmentSelectComboBox->currentText()));
    if(ui->actuatorSelectComboBox->currentText() == "Proximal") panelActuator = eq.lock()->getProximal();
    else if(ui->actuatorSelectComboBox->currentText() == "Distal") panelActuator = eq.lock()->getDistal();
    if(panelActuator)
    {
        ui->deviceSelectComboBox->setEnabled(true);
        ui->equipmentSelectComboBox->setEnabled(true);
        ui->actuatorSelectComboBox->setEnabled(true);

        ui->panelRotationAlignButton->setEnabled(true);
        ui->panelRotationLimiterActivatedRadioButton->setEnabled(true);
        ui->panelRotationLimiterHasActivatedRadioButton->setEnabled(true);
        ui->panelRotationReadyRadioButton->setEnabled(true);
        ui->panelRotationIqEdit->setEnabled(true);
        ui->panelRotationLineEdit->setEnabled(true);
        ui->panelRotationSpinBox->setEnabled(true);
        if(panelActuator->rotation_ready) ui->panelRotationSpinBox->setValue(roundf(panelActuator->getRotationState()));

        ui->panelPushPullAlignButton->setEnabled(true);
        ui->panelPushPullLimiterActivatedRadioButton->setEnabled(true);
        ui->panelPushPullLimiterHasActivatedRadioButton->setEnabled(true);
        ui->panelPushPullReadyRadioButton->setEnabled(true);
        ui->panelPushPullIqEdit->setEnabled(true);
        ui->panelPushPullLineEdit->setEnabled(true);
        ui->panelPushPullSpinBox->setEnabled(true);
        if(panelActuator->pushpull_ready) ui->panelPushPullSpinBox->setValue(roundf(panelActuator->getPushPullState()));

        ui->panelLinearAlignButton->setEnabled(true);
        ui->panelLinearLimiterActivatedRadioButton->setEnabled(true);
        ui->panelLinearLimiterHasActivatedRadioButton->setEnabled(true);
        ui->panelLinearReadyRadioButton->setEnabled(true);
        ui->panelLinearIqEdit->setEnabled(true);
        ui->panelLinearLineEdit->setEnabled(true);
        ui->panelLinearSpinBox->setEnabled(true);
        if(panelActuator->linear_ready) ui->panelLinearSpinBox->setValue(roundf(panelActuator->getLinearState()));

        ui->panelPreHomingButton->setEnabled(true);
        ui->panelPostHomingButton->setEnabled(panelActuator->preInstall_ready);
    }
}

void ControllerWindow::updatePanelStatus()
{
    if(!panelActuator.isNull())
    {
        ui->panelRotationLineEdit->setText(QString::number(panelActuator->getRotationState()));
        ui->panelRotationIqEdit->setText(QString::number(panelActuator->motorRotation.first->getIq()));
        ui->panelRotationLimiterActivatedRadioButton->setChecked(panelActuator->motorRotation.first->isLimiterActivated());
        ui->panelRotationLimiterHasActivatedRadioButton->setChecked(panelActuator->motorRotation.first->hasLimiterActivated());
        ui->panelRotationReadyRadioButton->setChecked(panelActuator->rotation_ready);

        ui->panelPushPullLineEdit->setText(QString::number(panelActuator->getPushPullState()));
        ui->panelPushPullIqEdit->setText(QString::number(panelActuator->motorPushPull.first->getIq()));
        ui->panelPushPullLimiterActivatedRadioButton->setChecked(panelActuator->motorPushPull.first->isLimiterActivated());
        ui->panelPushPullLimiterHasActivatedRadioButton->setChecked(panelActuator->motorPushPull.first->hasLimiterActivated());
        ui->panelPushPullReadyRadioButton->setChecked(panelActuator->pushpull_ready);

        ui->panelLinearLineEdit->setText(QString::number(panelActuator->getLinearState()));
        ui->panelLinearIqEdit->setText(QString::number(panelActuator->motorLinear.first->getIq()));
        ui->panelLinearLimiterActivatedRadioButton->setChecked(panelActuator->motorLinear.first->isLimiterActivated());
        ui->panelLinearLimiterHasActivatedRadioButton->setChecked(panelActuator->motorLinear.first->hasLimiterActivated());
        ui->panelLinearReadyRadioButton->setChecked(panelActuator->linear_ready);

        ui->panelPostHomingButton->setEnabled(panelActuator->preInstall_ready);
    }
}

void ControllerWindow::onSelectDescJSONPath()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Device Descriptions (*.json)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setModal(false);
    QDir descDir(currentPath);
    for(uint8_t i = 0; i < 4; i++) // maximum recurse 4 layers of directory
    {
        QDir wantedDescDir(descDir.filePath("DeviceDescriptions"));
        if(wantedDescDir.exists())
        {
            descDir = wantedDescDir;
            break;
        }
        QStringList lst = descDir.entryList(QStringList() << "*.json", QDir::Files);
        if(lst.size() > 0) break;
        descDir.cdUp();
    }
    dialog.setDirectory(descDir);
    connect(&dialog, &QFileDialog::fileSelected, this, [this](const QString& file){
        QSharedPointer<Device> dev = QSharedPointer<Device>(new Device);
        ui->deviceSelectComboBox->clear();
        dev->parseJsonFromFile(file, motorHashMap);
        if(dev->availbleEquipmentCount() > 0)
        {
            deviceHashMap.insert(dev->deviceName(), dev);
            ui->deviceSelectComboBox->addItem(dev->deviceName());
            emit infoMessage("Parsed Device: " + dev->deviceName() + QString::asprintf(" with %d equipment(s)", dev->availbleEquipmentCount()));
        }
        else emit errorMessage("Parsed Device: " + dev->deviceName() + " with no available equipment, deleting");
    });
    dialog.open();
    while(!dialog.isHidden()) QApplication::processEvents();
}

void ControllerWindow::contextMenuEvent(QContextMenuEvent *event)
{
    auto lambda = [this, &event](bool isLeft = true)
    {
        QScopedPointer<QMenu> pMenu(new QMenu());
        QScopedPointer<QAction> pActionRst(new QAction("Reset", pMenu.get()));
        if((isLeft && leftEquipment) || (!isLeft && rightEquipment))
        {
            QAction *pAction = new QAction("Detach", pMenu.get());
            connect(pAction, &QAction::triggered, this, [this, &isLeft](){
                if(isLeft) leftEquipment.clear();
                else rightEquipment.clear();
            });
            pMenu->addAction(pAction);
        }
        connect(pActionRst.get(), &QAction::triggered, this, [this, &isLeft](){
            if(isLeft)
            {
                leftProxCoord.reset();
                leftKinematics->proximalReset();
                leftDistCoord.reset();
                leftKinematics->distalReset();
            }
            else
            {
                rightProxCoord.reset();
                rightKinematics->proximalReset();
                rightDistCoord.reset();
                rightKinematics->distalReset();
            }
        });
        pMenu->addAction(pActionRst.get());
        pMenu->addSeparator();
        QWeakPointer<Device> dev(deviceHashMap.value(ui->deviceSelectComboBox->currentText()));
        if(!dev.isNull())
        {
            for(auto &i : dev.lock()->equipmentNames())
            {
                QAction *pAction = new QAction(i, pMenu.get());
                pAction->setCheckable(true);
                if((isLeft && leftEquipment && leftEquipment->equipmentName() == i) ||
                    (!isLeft && rightEquipment && rightEquipment->equipmentName() == i)) pAction->setChecked(true);
                connect(pAction, &QAction::triggered, this, [this, dev, pAction, isLeft](){
                    if(isLeft) leftEquipment = dev.lock()->getEquipmentByName(pAction->text()).toStrongRef();
                    else rightEquipment = dev.lock()->getEquipmentByName(pAction->text()).toStrongRef();
                });
                pMenu->addAction(pAction);
            }
        }
        pMenu->exec(event->globalPos());
    };
    qDebugMessage(QString::asprintf("QContextMenuEvent fired at (%d, %d)", event->x(), event->y()));
    if(ui->rotationGroupBox->underMouse())
    {
        QScopedPointer<QMenu> pMenu(new QMenu());
        QScopedPointer<QAction> pAction(new QAction("Rotation Calibrate...", pMenu.get()));
        if(!this->panelActuator) pAction->setEnabled(false);
        connect(pAction.get(), &QAction::triggered, this, [this](){
            if(panelActuator) panelActuator->beginRotationCalibrate();
        });
        pMenu->addAction(pAction.get());
        pMenu->exec(event->globalPos());
    }
    else if(ui->leftJoyPad->underMouse()) lambda(true);
    else if(ui->rightJoyPad->underMouse()) lambda(false);
}

void mapPOVToSpeed(Vector2D& speed, int angle)
{
    speed.x = 0.0f;
    speed.y = 0.0f;
    switch(angle)
    {
    case 0:
        speed.y = 1.0f;
        break;
    case 45:
        speed.x = 0.707f;
        speed.y = 0.707f;
        break;
    case 90:
        speed.x = 1.0f;
        break;
    case 135:
        speed.x = 0.707f;
        speed.y = -0.707f;
        break;
    case 180:
        speed.y = -1.0f;
        break;
    case 225:
        speed.x = -0.707f;
        speed.y = -0.707f;
        break;
    case 270:
        speed.x = -1.0f;
        break;
    case 315:
        speed.x = -0.707f;
        speed.y = 0.707f;
        break;
    default: break;
    }
}

void ControllerWindow::onPOVEvent(const QJoystickPOVEvent &event)
{
    // emit infoMessage(event.joystick->name + " (POV): " + QString::asprintf("angle=%d, pov=%d", event.angle, event.pov));
    if(ui->leftJoyComboBox->currentText() == event.joystick->name)
    {
        mapPOVToSpeed(leftPOVSpeed, event.angle);
        ui->leftAuxJoyPad->setX(leftPOVSpeed.x);
        ui->leftAuxJoyPad->setY(leftPOVSpeed.y);
    }
    if(ui->rightJoyComboBox->currentText() == event.joystick->name)
    {
        mapPOVToSpeed(rightPOVSpeed, event.angle);
        ui->rightAuxJoyPad->setX(rightPOVSpeed.x);
        ui->rightAuxJoyPad->setY(rightPOVSpeed.y);
    }
}

void ControllerWindow::onAxisEvent(const QJoystickAxisEvent &event)
{
    // emit infoMessage(event.joystick->name + " (Axis): " + QString::asprintf("axis=%d, value=%.3f", event.axis, event.value));
    float value = event.value;
    if(std::abs(value) <= 0.05f) value = 0.0f; // deadzone
    if(ui->leftJoyComboBox->currentText() == event.joystick->name)
    {
        if(event.axis == 0)
        {
            ui->leftJoyPad->setX(value);
        }
        else if(event.axis == 1)
        {
            value *= -1.0f;
            ui->leftJoyPad->setY(value);
        }
        else if(event.axis == 2)
        {
            leftPOVSpeed.x = value;
            ui->leftAuxJoyPad->setX(leftPOVSpeed.x);
        }
        else if(event.axis == 3)
        {
            leftPOVSpeed.y = value;
            ui->leftAuxJoyPad->setY(leftPOVSpeed.y);
        }
    }
    if(ui->rightJoyComboBox->currentText() == event.joystick->name)
    {
        if(event.axis == 0)
        {
            ui->rightJoyPad->setX(value);
        }
        else if(event.axis == 1)
        {
            value *= -1.0f;
            ui->rightJoyPad->setY(value);
        }
        else if(event.axis == 2)
        {
            rightPOVSpeed.x = value;
            ui->rightAuxJoyPad->setX(rightPOVSpeed.x);
        }
        else if(event.axis == 3)
        {
            rightPOVSpeed.y = value;
            ui->rightAuxJoyPad->setY(rightPOVSpeed.y);
        }
    }
}

void ControllerWindow::onButtonEvent(const QJoystickButtonEvent &event)
{
    emit infoMessage(event.joystick->name + " (Button): " + QString::asprintf("button=%d, pressed=%d", event.button, event.pressed));
}

void ControllerWindow::controlLoop()
{
    for(auto i = motorHashMap.constKeyValueBegin(); i != motorHashMap.constKeyValueEnd(); ++i)
    {
        i->second.get()->applyMotorConfig();
    }
    static uint8_t timer_100ms = 0;
    if(timer_100ms++ >= 99)
    {
        if(debuggerWindow) debuggerWindow->updateState();
        if(panelActuator) updatePanelStatus();
        timer_100ms = 0;
    }
}

void ControllerWindow::onEnableMotorDebugger()
{
    debuggerWindow = new MotorDebugger(motorHashMap, this);
    connect(debuggerWindow, &MotorDebugger::debugMessage, this, &ControllerWindow::debugMessage);
    connect(debuggerWindow, &MotorDebugger::errorMessage, this, &ControllerWindow::errorMessage);
    connect(debuggerWindow, &MotorDebugger::infoMessage, this, &ControllerWindow::infoMessage);
    connect(debuggerWindow, &MotorDebugger::onCloseWindow, this, [this](){
        if(debuggerWindow) debuggerWindow = nullptr;
    });
    debuggerWindow->setAttribute(Qt::WA_DeleteOnClose);
    debuggerWindow->showWindow();
}

ControllerWindow::~ControllerWindow()
{
    emit debugMessage("ControllerWindow destroyed");
    delete ui;
    emit onCloseWindow();
}
