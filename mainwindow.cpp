#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFontMetrics>

#define DEBUG_ENVIRONMENT

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    currentPath = QCoreApplication::applicationDirPath();
    ui->setupUi(this);
    statusBarStateLabel = new QLabel(this);
    statusBarWkcLabel = new QLabel(this);
    statusBarSlaveCountLabel = new QLabel(this);
    ui->statusbar->addPermanentWidget(statusBarSlaveCountLabel);
    ui->statusbar->addPermanentWidget(statusBarWkcLabel);
    ui->statusbar->addPermanentWidget(statusBarStateLabel);
    statusBarStateLabel->setText("State: NONE ");
    statusBarWkcLabel->setText("WKC: 0/0 ");
    statusBarSlaveCountLabel->setText("Slave(s): 0 ");
    ui->configXmlPathEdit->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    stateViewModel = new QStandardItemModel;
    ui->stateTreeView->setModel(stateViewModel);
#ifndef DEBUG_ENVIRONMENT
    ui->enterControllerButton->setEnabled(false);
#else
    ui->enterControllerButton->setEnabled(true);
#endif
    updateEthList();
    QJoysticks *QJoy = QJoysticks::getInstance();
    for(int i = 0; i < QJoy->deviceNames().size(); i++) joysticks.insert(QJoy->deviceNames().at(i), QJoy->inputDevices().at(i));
    onDebugMsg(QString::asprintf("Detected %d joystick(s): ", joysticks.size()));
    for(auto i = joysticks.constKeyValueBegin(); i != joysticks.constKeyValueEnd(); ++i) onDebugMsg(i->first);
    connect(ui->con_pushButton, &QPushButton::clicked, this, &MainWindow::onClickConnect);
    connect(&wrapper, &ECATWrapper::errorMessage, this, &MainWindow::onErrorMsg);
    connect(&wrapper, &ECATWrapper::infoMessage, this, &MainWindow::onInfoMsg);
    connect(&wrapper, &ECATWrapper::debugMessage, this, &MainWindow::onDebugMsg);
    connect(&wrapper, &ECATWrapper::onStateChanged, this, &MainWindow::onECATStateChanged);
    connect(ui->selectConfigPathButton, &QPushButton::clicked, this, &MainWindow::onSelectXMLPath);
    connect(ui->parseConfigXmlButton, &QPushButton::clicked, this, &MainWindow::onParseXML);
    connect(ui->enterControllerButton, &QPushButton::clicked, this, &MainWindow::onEnableController);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onEnableController()
{
    if(!controllerWindow)
    {
        controllerWindow = new ControllerWindow(wrapper, joysticks, this);
        connect(controllerWindow, &ControllerWindow::closed, this, &MainWindow::onDisableController);
        connect(controllerWindow, &ControllerWindow::errorMessage, this, &MainWindow::onErrorMsg);
        connect(controllerWindow, &ControllerWindow::infoMessage, this, &MainWindow::onInfoMsg);
        connect(controllerWindow, &ControllerWindow::debugMessage, this, &MainWindow::onDebugMsg);
        controllerWindow->showWindow();
    }
}

void MainWindow::onDisableController()
{
    controllerWindow = nullptr;
}

void MainWindow::onSelectXMLPath()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("EtherCAT Config (*.xml)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory(currentPath);
    if(dialog.exec())
    {
        configXMLPath = dialog.selectedFiles().at(0);
        QFontMetrics fontWidth(ui->configXmlPathEdit->font());
        QString pathElided = fontWidth.elidedText(configXMLPath, Qt::ElideMiddle, ui->configXmlPathEdit->width() - 5);
        ui->configXmlPathEdit->setText(pathElided);
        ui->configXmlPathEdit->setToolTip(configXMLPath);
    }
}

void MainWindow::onParseXML()
{
    QFile xmlFile(configXMLPath);
    if(xmlFile.exists())
    {
        wrapper.parseSlaveXML(xmlFile);
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QMessageBox msgBox;
    msgBox.setText("Warning");
    msgBox.setText("State = " + wrapper.getExpectedStateName() + ", do you want to exit now?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    if(msgBox.exec() == QMessageBox::Ok)
    {
        if(wrapper.getExpectedState() > EC_STATE_INIT) wrapper.closeConnection();
        e->accept();
    }
    else e->ignore();
}

void MainWindow::updateEthList()
{
    if(wrapper.getEthInfo(ethInfo) > 0) ui->eth_comboBox->addItems(ethInfo.keys());
}

void MainWindow::onClickConnect(void)
{
    if(wrapper.getExpectedState() != EC_STATE_OPERATIONAL) wrapper.initEth(ethInfo[ui->eth_comboBox->currentText()]);
    else wrapper.closeConnection();
}

void MainWindow::onECATStateChanged()
{
    uint16_t current_state = wrapper.getExpectedState();
    if(current_state == EC_STATE_OPERATIONAL)
    {
        if(controllerWindow && (wrapper.getExpectedWKC() == wrapper.getRealWKC())) controllerWindow->controlLoop();
        ui->con_pushButton->setText(QString::fromUtf8("Disconnect"));
#ifndef DEBUG_ENVIRONMENT
        ui->enterControllerButton->setEnabled(true);
#endif
    }
    else
    {
        stateViewModel->clear();
        ui->con_pushButton->setText(QString::fromUtf8("Connect"));
#ifndef DEBUG_ENVIRONMENT
        ui->enterControllerButton->setEnabled(false);
#endif
    }
    statusBarStateLabel->setText("State: " + wrapper.getExpectedStateName() + " ");
    statusBarWkcLabel->setText(QString::asprintf("WKC: %d/%d ", wrapper.getRealWKC(), wrapper.getExpectedWKC()));
    statusBarSlaveCountLabel->setText(QString::asprintf("Slave(s): %d ", wrapper.getSlaveCount()));
    ParseStateViewModel();
}

void MainWindow::ParseStateViewModel()
{
    bool newItem = wrapper.input_vector.size() > stateViewModel->rowCount();
    for(int i = 0; i < wrapper.input_vector.size(); i++)
    {
        QStandardItem *item;
        if(stateViewModel->rowCount() <= i) item = new QStandardItem;
        else item = stateViewModel->item(i);
        item->setColumnCount(2);
        item->setText(QString::asprintf("Slave #%d", i + 1));
        AppendDescToItem(QString::asprintf("MotorCount: %d", wrapper.input_vector.at(i)->Interface_State.MotorCount), 0, item);
        AppendDescToItem(QString::asprintf("MCUTemp: %d", wrapper.input_vector.at(i)->Interface_State.MCUTemp), 1, item);
        AppendDescToItem(QString::asprintf("VBus: %d", wrapper.input_vector.at(i)->Interface_State.Vbus), 2, item);
        AppendDescToItem(QString::asprintf("FPS: %d", wrapper.input_vector.at(i)->Interface_State.FramesPerSec), 3, item);
        stateViewModel->setItem(i, item);
    }
    if(newItem) ui->stateTreeView->expandAll();
}

void MainWindow::AppendDescToItem(QString desc, int row, QStandardItem *parent)
{
    QStandardItem *item;
    if(parent->rowCount() <= row) item = new QStandardItem;
    else item = parent->takeChild(row);
    item->setText(desc);
    parent->setChild(row, item);
}

void MainWindow::onErrorMsg(QString s)
{
    QDateTime currentDt = QDateTime::currentDateTime();
    QString result = "[" + currentDt.toString("yyyy-MM-dd hh:mm:ss.zzz") + "] [ERROR] " + s;
    qDebug() << result;
    ui->textBrowser->append(result);
}

void MainWindow::onInfoMsg(QString s)
{
    QDateTime currentDt = QDateTime::currentDateTime();
    QString result = "[" + currentDt.toString("yyyy-MM-dd hh:mm:ss.zzz") + "] [INFO] " + s;
    qDebug() << result;
    ui->textBrowser->append(result);
}

void MainWindow::onDebugMsg(QString s)
{
    QDateTime currentDt = QDateTime::currentDateTime();
    QString result = "[" + currentDt.toString("yyyy-MM-dd hh:mm:ss.zzz") + "] [DEBUG] " + s;
    qDebug() << result;
    // ui->textBrowser->append(result);
}
