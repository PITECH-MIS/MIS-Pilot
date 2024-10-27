#include "ControllerWindow.h"
#include "ui_ControllerWindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QElapsedTimer>
#include "../../Kinematics/KinematicsAtan2.h"

ControllerWindow::ControllerWindow(ECATWrapper* w, QMap<QString, QJoystickDevice*>& j, QWidget *parent)
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
    QWeakPointer<Device> dev(deviceHashMap.value(ui->deviceSelectComboBox->currentText()));
    if(dev)
    {
        if(!leftEquipmentName.isEmpty())
        {
            QSharedPointer<Equipment6DoF> leftEquipment = dev.lock()->getEquipmentByName(leftEquipmentName).lock();
            if(leftEquipment)
            {
                float multiplier_left = (float)ui->leftSpeedSlider->value() * 0.2f;
                if(!leftEquipment->kinematics) leftEquipment->kinematics = new KinematicsAtan2();
                if(leftEquipment->getProximalTarget())
                {
                    leftEquipment->kinematics->proximal_params = &leftEquipment->getProximal().lock()->kineParams;
                    leftEquipment->getProximalTarget()->x += ui->leftJoyPad->x() * timeElapsed * multiplier_left;
                    leftEquipment->getProximalTarget()->y += ui->leftJoyPad->y() * timeElapsed * multiplier_left;
                    leftEquipment->getProximalTarget()->z += leftProxLinSpeed * timeElapsed * multiplier_left;
                    if(leftEquipment->getDistalTarget())
                    {
                        leftEquipment->kinematics->distal_params = &leftEquipment->getDistal().lock()->kineParams;
                        leftEquipment->getDistalTarget()->x += leftPOVSpeed.x * timeElapsed * multiplier_left;
                        leftEquipment->getDistalTarget()->y += leftPOVSpeed.y * timeElapsed * multiplier_left;
                        leftEquipment->getDistalTarget()->z += leftDistLinSpeed * timeElapsed * multiplier_left;
                        leftEquipment->kinematics->calculate(*leftEquipment->getProximalTarget(), *leftEquipment->getDistalTarget());
                        leftEquipment->getProximalTarget()->z = leftEquipment->kinematics->proximal_act.translation;
                        leftEquipment->getDistalTarget()->z = leftEquipment->kinematics->distal_act.translation;
                        ui->leftProxCoordJoyPad->setX(leftEquipment->getProximalTarget()->x / leftEquipment->kinematics->proximal_params->max_abs_pushpull);
                        ui->leftProxCoordJoyPad->setY(leftEquipment->getProximalTarget()->y / leftEquipment->kinematics->proximal_params->max_abs_pushpull);
                        if(std::abs(leftEquipment->kinematics->proximal_act.pull / leftEquipment->kinematics->proximal_params->max_abs_pushpull) >= 0.9f) ui->leftProxCoordJoyPad->isWarning = true;
                        else ui->leftProxCoordJoyPad->isWarning = false;
                        ui->leftProxLabel->setText(leftEquipment->equipmentName() + " " + leftEquipment->getProximal().lock()->actuatorName());
                        ui->leftProxPushLineEdit->setText(QString::asprintf("%.3f", leftEquipment->kinematics->proximal_act.pull));
                        ui->leftProxRotLineEdit->setText(QString::asprintf("%.3f", RAD2DEG(leftEquipment->kinematics->proximal_act.rotation_angle)));

                        ui->leftDistCoordJoyPad->setX(leftEquipment->getDistalTarget()->x / leftEquipment->kinematics->distal_params->max_abs_pushpull);
                        ui->leftDistCoordJoyPad->setY(leftEquipment->getDistalTarget()->y / leftEquipment->kinematics->distal_params->max_abs_pushpull);
                        if(std::abs(leftEquipment->kinematics->distal_act.pull / leftEquipment->kinematics->distal_params->max_abs_pushpull) >= 0.9f) ui->leftDistCoordJoyPad->isWarning = true;
                        else ui->leftDistCoordJoyPad->isWarning = false;
                        ui->leftDistLabel->setText(leftEquipment->equipmentName() + " " + leftEquipment->getDistal().lock()->actuatorName());
                        ui->leftDistPushLineEdit->setText(QString::asprintf("%.3f", leftEquipment->kinematics->distal_act.pull));
                        ui->leftDistRotLineEdit->setText(QString::asprintf("%.3f", RAD2DEG(leftEquipment->kinematics->distal_act.rotation_angle)));

                        ui->leftProxXLineEdit->setText(QString::asprintf("%.3f", leftEquipment->getProximalTarget()->x));
                        ui->leftProxYLineEdit->setText(QString::asprintf("%.3f", leftEquipment->getProximalTarget()->y));
                        ui->leftProxLinearLineEdit->setText(QString::asprintf("%.3f", leftEquipment->getProximalTarget()->z));
                        ui->leftDistXLineEdit->setText(QString::asprintf("%.3f", leftEquipment->getDistalTarget()->x));
                        ui->leftDistYLineEdit->setText(QString::asprintf("%.3f", leftEquipment->getDistalTarget()->y));
                        ui->leftDistLinearLineEdit->setText(QString::asprintf("%.3f", leftEquipment->getDistalTarget()->z));
                        if(leftEquipment->isAllReady())
                        {
                            ui->leftReadyLabel->setText("READY");
                            leftEquipment->setProximalAct(leftEquipment->kinematics->proximal_act);
                            leftEquipment->setDistalAct(leftEquipment->kinematics->distal_act);
                        }
                        else ui->leftReadyLabel->setText("Not Ready");
                    }
                }
            }
        }
        else
        {
            ui->leftProxCoordJoyPad->setX(0.0f);
            ui->leftProxCoordJoyPad->setY(0.0f);
            ui->leftProxLabel->clear();
            ui->leftProxPushLineEdit->clear();
            ui->leftProxRotLineEdit->clear();
            ui->leftDistCoordJoyPad->setX(0.0f);
            ui->leftDistCoordJoyPad->setY(0.0f);
            ui->leftDistLabel->clear();
            ui->leftDistPushLineEdit->clear();
            ui->leftDistRotLineEdit->clear();
            ui->leftProxXLineEdit->clear();
            ui->leftProxYLineEdit->clear();
            ui->leftProxLinearLineEdit->clear();
            ui->leftDistXLineEdit->clear();
            ui->leftDistYLineEdit->clear();
            ui->leftDistLinearLineEdit->clear();
            ui->leftReadyLabel->setText("Not Ready");
        }
        if(!rightEquipmentName.isEmpty())
        {
            QSharedPointer<Equipment6DoF> rightEquipment = dev.lock()->getEquipmentByName(rightEquipmentName).lock();
            if(rightEquipment)
            {
                float multiplier_right = (float)ui->rightSpeedSlider->value() * 0.2f;
                if(!rightEquipment->kinematics) rightEquipment->kinematics = new KinematicsAtan2();
                if(rightEquipment->getProximalTarget())
                {
                    rightEquipment->kinematics->proximal_params = &rightEquipment->getProximal().lock()->kineParams;
                    rightEquipment->getProximalTarget()->x += ui->rightJoyPad->x() * timeElapsed * multiplier_right;
                    rightEquipment->getProximalTarget()->y += ui->rightJoyPad->y() * timeElapsed * multiplier_right;
                    rightEquipment->getProximalTarget()->z += rightProxLinSpeed * timeElapsed * multiplier_right;
                    if(rightEquipment->getDistalTarget())
                    {
                        rightEquipment->kinematics->distal_params = &rightEquipment->getDistal().lock()->kineParams;
                        rightEquipment->getDistalTarget()->x += rightPOVSpeed.x * timeElapsed * multiplier_right;
                        rightEquipment->getDistalTarget()->y += rightPOVSpeed.y * timeElapsed * multiplier_right;
                        rightEquipment->getDistalTarget()->z += rightDistLinSpeed * timeElapsed * multiplier_right;
                        rightEquipment->kinematics->calculate(*rightEquipment->getProximalTarget(), *rightEquipment->getDistalTarget());
                        rightEquipment->getProximalTarget()->z = rightEquipment->kinematics->proximal_act.translation;
                        rightEquipment->getDistalTarget()->z = rightEquipment->kinematics->distal_act.translation;
                        ui->rightProxCoordJoyPad->setX(rightEquipment->getProximalTarget()->x / rightEquipment->kinematics->proximal_params->max_abs_pushpull);
                        ui->rightProxCoordJoyPad->setY(rightEquipment->getProximalTarget()->y / rightEquipment->kinematics->proximal_params->max_abs_pushpull);
                        if(std::abs(rightEquipment->kinematics->proximal_act.pull / rightEquipment->kinematics->proximal_params->max_abs_pushpull) >= 0.9f) ui->rightProxCoordJoyPad->isWarning = true;
                        else ui->rightProxCoordJoyPad->isWarning = false;
                        ui->rightProxLabel->setText(rightEquipment->equipmentName() + " " + rightEquipment->getProximal().lock()->actuatorName());
                        ui->rightProxPushLineEdit->setText(QString::asprintf("%.3f", rightEquipment->kinematics->proximal_act.pull));
                        ui->rightProxRotLineEdit->setText(QString::asprintf("%.3f", RAD2DEG(rightEquipment->kinematics->proximal_act.rotation_angle)));

                        ui->rightDistCoordJoyPad->setX(rightEquipment->getDistalTarget()->x / rightEquipment->kinematics->distal_params->max_abs_pushpull);
                        ui->rightDistCoordJoyPad->setY(rightEquipment->getDistalTarget()->y / rightEquipment->kinematics->distal_params->max_abs_pushpull);
                        if(std::abs(rightEquipment->kinematics->distal_act.pull / rightEquipment->kinematics->distal_params->max_abs_pushpull) >= 0.9f) ui->rightDistCoordJoyPad->isWarning = true;
                        else ui->rightDistCoordJoyPad->isWarning = false;
                        ui->rightDistLabel->setText(rightEquipment->equipmentName() + " " + rightEquipment->getDistal().lock()->actuatorName());
                        ui->rightDistPushLineEdit->setText(QString::asprintf("%.3f", rightEquipment->kinematics->distal_act.pull));
                        ui->rightDistRotLineEdit->setText(QString::asprintf("%.3f", RAD2DEG(rightEquipment->kinematics->distal_act.rotation_angle)));

                        ui->rightProxXLineEdit->setText(QString::asprintf("%.3f", rightEquipment->getProximalTarget()->x));
                        ui->rightProxYLineEdit->setText(QString::asprintf("%.3f", rightEquipment->getProximalTarget()->y));
                        ui->rightProxLinearLineEdit->setText(QString::asprintf("%.3f", rightEquipment->getProximalTarget()->z));
                        ui->rightDistXLineEdit->setText(QString::asprintf("%.3f", rightEquipment->getDistalTarget()->x));
                        ui->rightDistYLineEdit->setText(QString::asprintf("%.3f", rightEquipment->getDistalTarget()->y));
                        ui->rightDistLinearLineEdit->setText(QString::asprintf("%.3f", rightEquipment->getDistalTarget()->z));
                        if(rightEquipment->isAllReady())
                        {
                            ui->rightReadyLabel->setText("READY");
                            rightEquipment->setProximalAct(rightEquipment->kinematics->proximal_act);
                            rightEquipment->setDistalAct(rightEquipment->kinematics->distal_act);
                        }
                        else ui->rightReadyLabel->setText("Not Ready");
                    }
                }
            }
        }
        else
        {
            ui->rightProxCoordJoyPad->setX(0.0f);
            ui->rightProxCoordJoyPad->setY(0.0f);
            ui->rightProxLabel->clear();
            ui->rightProxPushLineEdit->clear();
            ui->rightProxRotLineEdit->clear();
            ui->rightDistCoordJoyPad->setX(0.0f);
            ui->rightDistCoordJoyPad->setY(0.0f);
            ui->rightDistLabel->clear();
            ui->rightDistPushLineEdit->clear();
            ui->rightDistRotLineEdit->clear();
            ui->rightProxXLineEdit->clear();
            ui->rightProxYLineEdit->clear();
            ui->rightProxLinearLineEdit->clear();
            ui->rightDistXLineEdit->clear();
            ui->rightDistYLineEdit->clear();
            ui->rightDistLinearLineEdit->clear();
            ui->rightReadyLabel->setText("Not Ready");
        }
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
    motorSNSet = getMotorSN(wrapper->input_vector);
    emit infoMessage(QString::asprintf("Find %d motor(s) on EtherCAT Bus", motorSNSet.size()));
    for(const auto &i : std::as_const(motorSNSet))
    {
        // emit debugMessage("Find motor SN: " + i);
        uint8_t limiter_index = 0;
        QSharedPointer<Motor> motor = QSharedPointer<Motor>(new Motor(i.toUInt(), limiter_index));
        if(motor->findMotorInVector(wrapper->input_vector, wrapper->output_vector))
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
    if(eq.lock()->getProximal()) ui->actuatorSelectComboBox->addItem("Proximal");
    if(eq.lock()->getDistal()) ui->actuatorSelectComboBox->addItem("Distal");
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

void ControllerWindow::readRecordJson()
{
    QFile record(currentPath + "/record.json");
    if(!record.exists() || !record.open(QIODevice::ReadOnly))
    {
        emit infoMessage("No alignment record found");
        return;
    }
    emit infoMessage("Found alignment record: " + record.fileName());
    QByteArray data = record.readAll();
    QJsonParseError result;
    QJsonDocument doc = QJsonDocument::fromJson(data, &result);
    if(doc.isNull() || !doc.isObject() || result.error != QJsonParseError::NoError)
    {
        emit errorMessage("Error while parsing record JSON: " + result.errorString());
        return;
    }
    recordJsonObject = doc.object();
    for(const auto &i : recordJsonObject.keys())
    {
        if(recordJsonObject.value(i).isObject() && deviceHashMap.contains(i))
        {
            emit debugMessage("Found matched device: " + i);
            QWeakPointer<Device> dev(deviceHashMap.value(i));
            QJsonObject devObj = recordJsonObject.value(i).toObject();
            for(const auto &j: devObj.keys())
            {
                if(devObj.value(j).isObject() && dev.lock()->equipmentNames().contains(j))
                {
                    emit debugMessage("Found matched equipment: " + j);
                    auto equipment = dev.lock()->getEquipmentByName(j).lock();
                    QJsonObject equipObj = devObj.value(j).toObject();
                    for(const auto &k : equipObj.keys())
                    {
                        if(k.toLower() == "proximal")
                        {
                            auto prox = equipment->getProximal().lock();
                            if(prox)
                            {
                                emit debugMessage("Found matched proximal actuator");
                                QJsonObject actuaObj = equipObj.value(k).toObject();

                            }
                        }
                        else if(k.toLower() == "distal")
                        {
                            emit debugMessage("Found matched distal actuator");
                        }
                        else emit debugMessage("Found unrecognized actuator in equipment record: " + k);
                    }
                }
                else emit debugMessage("Found unrecognized equipment in device record: " + j);
            }
        }
        else emit debugMessage("Found unrecognized device in root record: " + i);
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
        if(deviceHashMap.size() > 0) readRecordJson();
    });
    dialog.open();
    while(!dialog.isHidden()) QApplication::processEvents();
}

void ControllerWindow::contextMenuEvent(QContextMenuEvent *event)
{
    auto lambda = [this, &event](bool isLeft = true)
    {
        QScopedPointer<QMenu> pMenu(new QMenu(this));
        QScopedPointer<QAction> pActionRst(new QAction("Reset", pMenu.get()));
        if((isLeft && !leftEquipmentName.isEmpty()) || (!isLeft && !rightEquipmentName.isEmpty()))
        {
            QAction *pAction = new QAction("Detach", pMenu.get());
            connect(pAction, &QAction::triggered, this, [this, &isLeft](){
                if(isLeft) leftEquipmentName.clear();
                else rightEquipmentName.clear();
            });
            pMenu->addAction(pAction);
        }
        connect(pActionRst.get(), &QAction::triggered, this, [this, &isLeft](){
            QWeakPointer<Device> dev(deviceHashMap.value(ui->deviceSelectComboBox->currentText()));
            if(dev)
            {
                if(isLeft)
                {
                    auto leftEquipment = dev.lock()->getEquipmentByName(leftEquipmentName).lock();
                    if(leftEquipment)
                    {
                        if(leftEquipment->kinematics)
                        {
                            leftEquipment->kinematics->proximalReset();
                            leftEquipment->kinematics->distalReset();
                        }
                        if(leftEquipment->getProximalTarget()) leftEquipment->getProximalTarget()->reset();
                        if(leftEquipment->getDistalTarget()) leftEquipment->getDistalTarget()->reset();
                    }
                }
                else
                {
                    auto rightEquipment = dev.lock()->getEquipmentByName(rightEquipmentName).lock();
                    if(rightEquipment)
                    {
                        if(rightEquipment->kinematics)
                        {
                            rightEquipment->kinematics->proximalReset();
                            rightEquipment->kinematics->distalReset();
                        }
                        if(rightEquipment->getProximalTarget()) rightEquipment->getProximalTarget()->reset();
                        if(rightEquipment->getDistalTarget()) rightEquipment->getDistalTarget()->reset();
                    }
                }
            }
        });
        pMenu->addAction(pActionRst.get());
        pMenu->addSeparator();
        QWeakPointer<Device> dev(deviceHashMap.value(ui->deviceSelectComboBox->currentText()));
        if(dev)
        {
            for(const auto &i : dev.lock()->equipmentNames())
            {
                QAction *pAction = new QAction(i, pMenu.get());
                pAction->setCheckable(true);
                if((isLeft && leftEquipmentName == i) || (!isLeft && rightEquipmentName == i)) pAction->setChecked(true);
                connect(pAction, &QAction::triggered, this, [this, pAction, isLeft](){
                    if(isLeft)
                    {
                        if(pAction->text() != rightEquipmentName) leftEquipmentName = pAction->text();
                    }
                    else
                    {
                        if(pAction->text() != leftEquipmentName) rightEquipmentName = pAction->text();
                    }
                });
                pMenu->addAction(pAction);
            }
        }
        pMenu->exec(event->globalPos());
    };
    qDebugMessage(QString::asprintf("QContextMenuEvent fired at (%d, %d)", event->x(), event->y()));
    if(ui->rotationGroupBox->underMouse())
    {
        QScopedPointer<QMenu> pMenu(new QMenu(this));
        QScopedPointer<QAction> pAction(new QAction("Rotation Calibrate...", pMenu.get()));
        if(!this->panelActuator) pAction->setEnabled(false);
        connect(pAction.get(), &QAction::triggered, this, [this](){
            if(panelActuator) panelActuator->beginRotationCalibrate();
        });
        pMenu->addAction(pAction.get());
        pMenu->exec(event->globalPos());
    }
    else if(ui->leftJoyPad->underMouse() || ui->leftAuxJoyPad->underMouse()) lambda(true);
    else if(ui->rightJoyPad->underMouse() || ui->rightAuxJoyPad->underMouse()) lambda(false);
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
    auto lambdaSwitchEquipment = [this, &event](bool isLeft)
    {
        if(event.pressed == 0)
        {
            QWeakPointer<Device> dev(deviceHashMap.value(ui->deviceSelectComboBox->currentText()));
            if(dev)
            {
                QString selectEquipName = "";
                switch(event.button)
                {
                case 27: // left
                    for(const auto &i : dev.lock()->equipmentNames())
                    {
                        if(i.contains("Left"))
                        {
                            selectEquipName = i;
                            break;
                        }
                    }
                    break;
                case 26: // mid
                    for(const auto &i : dev.lock()->equipmentNames())
                    {
                        if(i.contains("Upper"))
                        {
                            selectEquipName = i;
                            break;
                        }
                    }
                    break;
                case 28: // right
                    for(const auto &i : dev.lock()->equipmentNames())
                    {
                        if(i.contains("Right"))
                        {
                            selectEquipName = i;
                            break;
                        }
                    }
                    break;
                default: break;
                }
                if(dev.lock()->equipmentNames().contains(selectEquipName))
                {
                    if(isLeft)
                    {
                        if(selectEquipName != rightEquipmentName)
                        {
                            if(selectEquipName == leftEquipmentName) leftEquipmentName.clear();
                            else leftEquipmentName = selectEquipName;
                        }
                    }
                    else
                    {
                        if(selectEquipName != leftEquipmentName)
                        {
                            if(selectEquipName == rightEquipmentName) rightEquipmentName.clear();
                            else rightEquipmentName = selectEquipName;
                        }
                    }
                }
            }
        }
    };
    auto lambdaLinearMotion = [this, &event](bool isLeft)
    {
        // button = 20, proximal forward
        // button = 21, proximal backward 100%
        // button = 0, proximal backward 100%
        // button = 1, proximal backward 125%
        // button = 10, distal forward
        // button = 12, distal backward
        if(event.button == 20 || event.button == 21 || event.button == 0 || event.button == 1 || event.button == 10 || event.button == 12)
        {
            if(isLeft)
            {
                if(event.button == 20 || event.button == 21 || event.button == 0 || event.button == 1)
                {
                    if(event.pressed == 0) leftProxLinSpeed = 0.0f;
                    else
                    {
                        if(event.button == 20) leftProxLinSpeed = desiredLinSpeed;
                        else if(event.button == 0 || event.button == 21) leftProxLinSpeed = -desiredLinSpeed;
                        else leftProxLinSpeed = -desiredLinSpeed * 1.25f;
                    }
                }
                else
                {
                    if(event.pressed == 0) leftDistLinSpeed = 0.0f;
                    else
                    {
                        if(event.button == 10) leftDistLinSpeed = desiredLinSpeed;
                        else leftDistLinSpeed = -desiredLinSpeed;
                    }
                }
            }
            else
            {
                if(event.button == 20 || event.button == 0 || event.button == 1)
                {
                    if(event.pressed == 0) rightProxLinSpeed = 0.0f;
                    else
                    {
                        if(event.button == 20) rightProxLinSpeed = desiredLinSpeed;
                        else if(event.button == 0 || event.button == 21) rightProxLinSpeed = -desiredLinSpeed;
                        else rightProxLinSpeed = -desiredLinSpeed * 1.25f;
                    }
                }
                else
                {
                    if(event.pressed == 0) rightDistLinSpeed = 0.0f;
                    else
                    {
                        if(event.button == 10) rightDistLinSpeed = desiredLinSpeed;
                        else rightDistLinSpeed = -desiredLinSpeed;
                    }
                }
            }
        }
    };
    if(ui->leftJoyComboBox->currentText() == event.joystick->name)
    {
        lambdaSwitchEquipment(true);
        lambdaLinearMotion(true);
    }
    else if(ui->rightJoyComboBox->currentText() == event.joystick->name)
    {
        lambdaSwitchEquipment(false);
        lambdaLinearMotion(false);
    }
    emit infoMessage(event.joystick->name + " (Button): " + QString::asprintf("button=%d, pressed=%d", event.button, event.pressed));
}

void ControllerWindow::controlLoop()
{
    qDebugMessage(QString::asprintf("[controlLoop] Thread ID: %x", this->thread()->currentThreadId()));
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

void ControllerWindow::showEvent(QShowEvent *event)
{
    centerOnCursorScreen(this);
}

ControllerWindow::~ControllerWindow()
{
    emit debugMessage("ControllerWindow destroyed");
    delete ui;
    emit onCloseWindow();
}
