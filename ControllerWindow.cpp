#include "ControllerWindow.h"
#include "ui_ControllerWindow.h"
#include <QFileDialog>
#include <QDir>

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
        emit infoMessage("No physics joysticks found, now the left/right joysticks behave virtually");
    }
    motorSNSet.clear();
    motorHashMap.clear();
    motorSNSet = getMotorSN(wrapper.input_vector);
    emit infoMessage(QString::asprintf("Find %d motor(s) on EtherCAT Bus", motorSNSet.size()));
    for(const auto &i : std::as_const(motorSNSet))
    {
        emit debugMessage("Find motor SN: " + i);
        uint8_t limiter_index = 0;
        Motor *motor = new Motor(i.toUInt(), limiter_index);
        if(motor->findMotorInVector(wrapper.input_vector, wrapper.output_vector))
        {
            motor->resetState();
            motor->setCurrentLimit(0.5f);
            motorHashMap.insert(i, motor);
            emit debugMessage("Successfully mapped motor SN: " + i);
        }
        else emit errorMessage("Error mapping motor SN:" + i);
    }
    this->show();
}

void ControllerWindow::onSelectDescJSONPath()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Device Descriptions (*.json)"));
    dialog.setViewMode(QFileDialog::Detail);
    QDir descDir(currentPath);
    for(uint8_t i = 0; i < 4; i++)
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
    if(dialog.exec())
    {
        // configXMLPath = dialog.selectedFiles().at(0);
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
        i->second->applyMotorConfig();
    }
    static uint16_t timer_100ms = 0;
    if(timer_100ms++ >= 99)
    {
        if(debuggerWindow) debuggerWindow->updateState();
        static uint8_t j = 0;
        j++;
        uint8_t r, g, b;
        GetColorWheel(j, &r, &g, &b);
        for(auto &i : wrapper.output_vector)
        {
            i->Interface_Set.LEDState = 1;
            i->Interface_Set.LEDR = r;
            i->Interface_Set.LEDG = g;
            i->Interface_Set.LEDB = b;
        }
        timer_100ms = 0;
    }
}

void ControllerWindow::onEnableMotorDebugger()
{
    debuggerWindow = new MotorDebugger(motorHashMap, this);
    connect(debuggerWindow, &MotorDebugger::debugMessage, this, &ControllerWindow::debugMessage);
    connect(debuggerWindow, &MotorDebugger::errorMessage, this, &ControllerWindow::errorMessage);
    connect(debuggerWindow, &MotorDebugger::infoMessage, this, &ControllerWindow::infoMessage);
    debuggerWindow->setAttribute(Qt::WA_DeleteOnClose);
    debuggerWindow->showWindow();
}

ControllerWindow::~ControllerWindow()
{
    delete ui;
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
