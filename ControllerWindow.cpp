#include "ControllerWindow.h"
#include "ui_ControllerWindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

ControllerWindow::ControllerWindow(ECATWrapper& w, QMap<QString, QJoystickDevice*>& j, QWidget *parent)
    : QMainWindow(parent), wrapper(w), joysticks(j)
    , ui(new Ui::ControllerWindow)
{
    currentPath = QCoreApplication::applicationDirPath();
    ui->setupUi(this);
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
    if(ui->actuatorSelectComboBox->currentText() == "Proximal")
    {
        panelActuator = eq.lock()->getProximal();
    }
    else if(ui->actuatorSelectComboBox->currentText() == "Distal")
    {
        panelActuator = eq.lock()->getDistal();
    }
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

        ui->panelPushPullAlignButton->setEnabled(true);
        ui->panelPushPullLimiterActivatedRadioButton->setEnabled(true);
        ui->panelPushPullLimiterHasActivatedRadioButton->setEnabled(true);
        ui->panelPushPullReadyRadioButton->setEnabled(true);
        ui->panelPushPullIqEdit->setEnabled(true);
        ui->panelPushPullLineEdit->setEnabled(true);
        ui->panelPushPullSpinBox->setEnabled(true);

        ui->panelLinearAlignButton->setEnabled(true);
        ui->panelLinearLimiterActivatedRadioButton->setEnabled(true);
        ui->panelLinearLimiterHasActivatedRadioButton->setEnabled(true);
        ui->panelLinearReadyRadioButton->setEnabled(true);
        ui->panelLinearIqEdit->setEnabled(true);
        ui->panelLinearLineEdit->setEnabled(true);
        ui->panelLinearSpinBox->setEnabled(true);
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
    }
}

void ControllerWindow::onSelectDescJSONPath()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Device Descriptions (*.json)"));
    dialog.setViewMode(QFileDialog::Detail);
    QDir descDir(currentPath);
    for(uint8_t i = 0; i < 4; i++) // maximum recurse 4 layers of directory
    {
        QApplication::processEvents(QEventLoop::AllEvents, 100);
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
    if(dialog.exec())
    {
        QApplication::processEvents(QEventLoop::AllEvents, 100);
        QString path = dialog.selectedFiles().at(0);
        QSharedPointer<Device> dev = QSharedPointer<Device>(new Device);
        ui->deviceSelectComboBox->clear();
        dev->parseJsonFromFile(path, motorHashMap);
        if(dev->availbleEquipmentCount() > 0)
        {
            deviceHashMap.insert(dev->deviceName(), dev);
            ui->deviceSelectComboBox->addItem(dev->deviceName());
            emit infoMessage("Parsed Device: " + dev->deviceName() + QString::asprintf(" with %d equipment(s)", dev->availbleEquipmentCount()));
        }
        else emit errorMessage("Parsed Device: " + dev->deviceName() + " with no available equipment, deleting");
    }
}

void ControllerWindow::contextMenuEvent(QContextMenuEvent *event)
{
    qDebugMessage(QString::asprintf("QContextMenuEvent fired at (%d, %d)", event->x(), event->y()));
    if(ui->rotationGroupBox->underMouse())
    {
        QScopedPointer<QMenu> pMenu(new QMenu());
        QScopedPointer<QAction> pAction(new QAction("Rotation Calibrate...", this));
        if(!this->panelActuator) pAction->setEnabled(false);
        connect(pAction.get(), &QAction::triggered, this, [this](){
            if(panelActuator) panelActuator->beginRotationCalibrate();
        });
        pMenu->addAction(pAction.get());
        pMenu->exec(event->globalPos());
    }
}

void ControllerWindow::onPOVEvent(const QJoystickPOVEvent &event)
{
    emit infoMessage(event.joystick->name + " (POV): " + QString::asprintf("angle=%d, pov=%d", event.angle, event.pov));
}

void ControllerWindow::onAxisEvent(const QJoystickAxisEvent &event)
{
    // emit infoMessage(event.joystick->name + " (Axis): " + QString::asprintf("axis=%d, value=%.3f", event.axis, event.value));
    float value = event.value;
    // if(event.axis == 0 && event.joystick->name.contains("VKBsim Gladiator EVO L"))
    // {
    //     // value = (value + 0.5f) * 2.0f;
    // }
    if(ui->leftJoyComboBox->currentText() == event.joystick->name)
    {
        if(event.axis == 0)
        {
            ui->leftJoyPad->setX(value);
            ui->leftJoyHorizontalSlider->setValue(100 * value);
        }
        else if(event.axis == 1)
        {
            value *= -1.0f;
            ui->leftJoyPad->setY(value);
            ui->leftJoyVerticalSlider->setValue(100 * value);
        }
    }
    if(ui->rightJoyComboBox->currentText() == event.joystick->name)
    {
        if(event.axis == 0)
        {
            ui->rightJoyPad->setX(value);
            ui->rightJoyHorizontalSlider->setValue(100 * value);
        }
        else if(event.axis == 1)
        {
            value *= -1.0f;
            ui->rightJoyPad->setY(value);
            ui->rightJoyVerticalSlider->setValue(100 * value);
        }
    }
}

void ControllerWindow::onButtonEvent(const QJoystickButtonEvent &event)
{
    emit infoMessage(event.joystick->name + " (Button): " + QString::asprintf("button=%d, pressed=%d", event.button, event.pressed));
}

void GetColorWheel(uint8_t pos, uint8_t *r, uint8_t *g, uint8_t *b);

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
        // static uint8_t j = 0;
        // j++;
        // uint8_t r, g, b;
        // GetColorWheel(j, &r, &g, &b);
        // for(auto &i : wrapper.output_vector)
        // {
        //     i->Interface_Set.LEDState = 1;
        //     i->Interface_Set.LEDR = r;
        //     i->Interface_Set.LEDG = g;
        //     i->Interface_Set.LEDB = b;
        // }
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

void GetColorWheel(uint8_t pos, uint8_t *r, uint8_t *g, uint8_t *b)
{
    if(pos < 85)
    {
        *r = pos * 3;
        *g = 255 - *r;
        *b = 0;
        return;
    }
    if(pos < 170)
    {
        pos -= 85;
        *g = 0;
        *b = pos * 3;
        *r = 255 - *b;
        return;
    }
    pos -= 170;
    *r = 0;
    *g = pos * 3;
    *b = 255 - *g;
}
