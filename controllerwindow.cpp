#include "controllerwindow.h"
#include "ui_controllerwindow.h"

ControllerWindow::ControllerWindow(QWidget *parent, ECATWrapper *w)
    : QMainWindow(parent)
    , ui(new Ui::ControllerWindow)
{
    QJoy = QJoysticks::getInstance();
    wrapper = w;
    ui->setupUi(this);
    connect(QJoy, &QJoysticks::POVEvent, this, &ControllerWindow::onPOVEvent);
    connect(QJoy, &QJoysticks::axisEvent, this, &ControllerWindow::onAxisEvent);
    connect(QJoy, &QJoysticks::buttonEvent, this, &ControllerWindow::onButtonEvent);
    connect(ui->motorSNComboBox, &QComboBox::currentIndexChanged, this, &ControllerWindow::onSelectDebugMotor);
    connect(ui->motorEnableCheckBox, &QCheckBox::checkStateChanged, this, &ControllerWindow::onClickDebugEnable);
    connect(ui->motorModeSlider, &QSlider::valueChanged, this, &ControllerWindow::onSelectDebugMode);
    connect(ui->motorTrajAngleSlider, &QSlider::valueChanged, this, &ControllerWindow::onSendDebugAction);
}

void ControllerWindow::showWindow()
{
    if(joysticks.empty())
    {
        for(int i = 0; i < QJoy->deviceNames().size(); i++) joysticks.insert(QJoy->deviceNames().at(i), QJoy->inputDevices().at(i));
        emit debugMessage(QString::asprintf("Detected %d joystick(s): ", joysticks.size()));
        for(int i = 0; i < joysticks.size(); i++) emit debugMessage(joysticks.keys().at(i));
        ui->leftJoyComboBox->addItems(joysticks.keys());
        ui->rightJoyComboBox->addItems(joysticks.keys());
        ui->leftJoyComboBox->setCurrentIndex(0);
        if(joysticks.size() > 1) ui->rightJoyComboBox->setCurrentIndex(1);
    }
    ui->motorSNComboBox->clear();
    motorSNSet.clear();
    motorMap.clear();
    emit debugMessage("Motor infomation cleared");
    motorSNSet = getMotorSN(wrapper->input_vector);
    emit infoMessage(QString::asprintf("Find %d motor(s) in EtherCAT Bus", motorSNSet.size()));
    for(const auto &i : motorSNSet)
    {
        emit debugMessage("Find motor SN: " + i);
        uint8_t limiter_index = 0;
        Motor* motor = new Motor(wrapper->input_vector, wrapper->output_vector, i.toUInt(), limiter_index);
        if(motor->findMotorInVector())
        {
            motor->resetState();
            motor->setCurrentLimit(0.5f);
            motorMap.insert(i, motor);
            ui->motorSNComboBox->addItem(i);
            emit debugMessage("Successfully mapped motor SN: " + i);
        }
        else emit errorMessage("Error mapping motor SN:" + i);
    }
    onSelectDebugMotor();
    this->show();
}

void ControllerWindow::onSelectDebugMotor()
{
    currentDebuggingMotor = motorMap.value(ui->motorSNComboBox->currentText());
    ui->motorEnableCheckBox->setChecked(currentDebuggingMotor->getState());
    ui->motorModeSlider->setValue((int)currentDebuggingMotor->getMode());
    ui->motorTrajAngleSlider->setValue((int)(currentDebuggingMotor->getPosDeg() / 45.0f));
    emit debugMessage("Change debugging motor: " + ui->motorSNComboBox->currentText());
}

void ControllerWindow::onClickDebugEnable()
{
    if(!(currentDebuggingMotor && currentDebuggingMotor->setState((Motor::State)ui->motorEnableCheckBox->isChecked())))
    emit errorMessage("Error");
}

void ControllerWindow::onSelectDebugMode()
{
    if(!(currentDebuggingMotor && currentDebuggingMotor->setMode((Motor::Mode)ui->motorModeSlider->value())))
    emit errorMessage("Error");
}

void ControllerWindow::onSendDebugAction()
{
    if(!(currentDebuggingMotor && currentDebuggingMotor->setTrajAbsAngle((float)ui->motorTrajAngleSlider->value() * 45.0f)))
    emit errorMessage("Error");
}

void ControllerWindow::onPOVEvent(const QJoystickPOVEvent &event)
{
    emit infoMessage(event.joystick->name + " (POV): " + QString::asprintf("angle=%d, pov=%d", event.angle, event.pov));
}

void ControllerWindow::onAxisEvent(const QJoystickAxisEvent &event)
{
    // emit infoMessage(event.joystick->name + " (Axis): " + QString::asprintf("axis=%d, value=%.3f", event.axis, event.value));
    float value = event.value;
    if(event.axis == 0 && event.joystick->name.contains("VKBsim Gladiator EVO L"))
    {
        // value = (value + 0.5f) * 2.0f;
    }
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

void ControllerWindow::controlLoop()
{
    static uint8_t j = 0;
    j++;
    uint8_t r, g, b;
    GetColorWheel(j, &r, &g, &b);
    for(auto &i : wrapper->output_vector)
    {
        i->Interface_Set.LEDState = 1;
        i->Interface_Set.LEDR = r;
        i->Interface_Set.LEDG = g;
        i->Interface_Set.LEDB = b;
    }
    for(auto &i : motorMap.values())
    {
        i->applyMotorConfig();
    }
    if(currentDebuggingMotor)
    {
        ui->motorSpeedLcd->display(currentDebuggingMotor->getSpeed());
        ui->motorPositionLcd->display(currentDebuggingMotor->getPosDeg());
        ui->motorStatusLed1->display(currentDebuggingMotor->getIq());
        ui->motorStatusLed2->display(currentDebuggingMotor->getCurrentLimit());
        ui->motorStatusLed3->display(currentDebuggingMotor->hasLimiterActivated());
        ui->motorStatusLed4->display(currentDebuggingMotor->getErrorCode());
        ui->motorStatusLed5->display(currentDebuggingMotor->getState());
        ui->motorStatusLed6->display(currentDebuggingMotor->getMode());
    }
}

ControllerWindow::~ControllerWindow()
{
    delete ui;
}

void ControllerWindow::closeEvent(QCloseEvent *event)
{
    emit closed();
    QWidget::closeEvent(event);
}
