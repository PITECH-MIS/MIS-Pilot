#include "MainWindow.h"
#include "opencv2/highgui.hpp"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QFontMetrics>

#define DEBUG_ENVIRONMENT

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebugMessage(QString::asprintf("[MainWindow] Thread ID: %x", this->thread()->currentThreadId()));
    currentPath = QCoreApplication::applicationDirPath();
    wrapper = ECATWrapper::getInstance();
    ui->setupUi(this);
    this->setWhatsThis("MIS-Pilot");
    QIcon icon = QIcon(":/image/logo.png");
    tray = new TrayIcon(icon, this);
    this->setWindowIcon(icon);
    statusBarStateLabel = new QLabel(this);
    statusBarWkcLabel = new QLabel(this);
    statusBarSlaveCountLabel = new QLabel(this);
    ui->statusbar->addPermanentWidget(statusBarSlaveCountLabel);
    ui->statusbar->addPermanentWidget(statusBarWkcLabel);
    ui->statusbar->addPermanentWidget(statusBarStateLabel);
    ui->menubar->raise();
    ui->menuTools->raise();
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
    QJoysticks* QJoy = QJoysticks::getInstance();
    QJoy->updateInterfaces();
    connect(ui->con_pushButton, &QPushButton::clicked, this, &MainWindow::onClickConnect);
    connect(wrapper, &ECATWrapper::errorMessage, this, &MainWindow::onErrorMsg, Qt::QueuedConnection);
    connect(wrapper, &ECATWrapper::infoMessage, this, &MainWindow::onInfoMsg, Qt::QueuedConnection);
    connect(wrapper, &ECATWrapper::debugMessage, this, &MainWindow::onDebugMsg, Qt::QueuedConnection);
    connect(wrapper, &ECATWrapper::onStateChanged, this, &MainWindow::onECATStateChanged, Qt::QueuedConnection);
    connect(ui->selectConfigPathButton, &QPushButton::clicked, this, &MainWindow::onSelectXMLPath);
    connect(ui->parseConfigXmlButton, &QPushButton::clicked, this, &MainWindow::onParseXML);
    connect(ui->enterControllerButton, &QPushButton::clicked, this, &MainWindow::onEnableController);
    ui->textBrowser->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->textBrowser, &QTextBrowser::customContextMenuRequested, this, &MainWindow::onTextBrowserCustomContextMenu);
    ui->actionEEPROM_Update->setEnabled(false);
    ui->actionPayload_Debugger->setEnabled(false);
    // connect(ui->actionEEPROM_Update, &QAction::triggered, )
    connect(ui->actionPayload_Debugger, &QAction::triggered, this, &MainWindow::onEnablePayloadDebugger);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onEnableController()
{
    if(!controllerWindow)
    {
        QJoysticks* QJoy = QJoysticks::getInstance();
        QJoy->updateInterfaces();
        for(int i = 0; i < QJoy->deviceNames().size(); i++) joysticks.insert(QJoy->deviceNames().at(i), QJoy->inputDevices().at(i));
        onDebugMsg(QString::asprintf("Detected %d joystick(s): ", joysticks.size()));
        for(auto i = joysticks.constKeyValueBegin(); i != joysticks.constKeyValueEnd(); ++i) onDebugMsg(i->first);
        controllerWindow = new ControllerWindow(wrapper, joysticks, this);
        connect(controllerWindow, &ControllerWindow::errorMessage, this, &MainWindow::onErrorMsg);
        connect(controllerWindow, &ControllerWindow::infoMessage, this, &MainWindow::onInfoMsg);
        connect(controllerWindow, &ControllerWindow::debugMessage, this, &MainWindow::onDebugMsg);
        connect(controllerWindow, &ControllerWindow::onCloseWindow, this, [this]() {
            if(controllerWindow) controllerWindow = nullptr;
        });
        controllerWindow->setAttribute(Qt::WA_DeleteOnClose);
        controllerWindow->showWindow();
    }
}

void MainWindow::onEnableEEPROMTool()
{

}

void MainWindow::onEnablePayloadDebugger()
{
    if(!payloadDebugger)
    {
        payloadDebugger = new PayloadDebugger(wrapper);
        connect(payloadDebugger, &PayloadDebugger::destroyed, this, [this](){
            if(payloadDebugger) payloadDebugger = nullptr;
        });
        payloadDebugger->setAttribute(Qt::WA_DeleteOnClose);
        payloadDebugger->show();
    }
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
        wrapper->parseSlaveXML(xmlFile);
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QMessageBox msgBox(this);
    msgBox.setText("Warning");
    msgBox.setText("State = " + wrapper->getExpectedStateName() + ", do you want to exit now?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    if(msgBox.exec() == QMessageBox::Ok)
    {
        if(wrapper->getExpectedState() > EC_STATE_INIT) wrapper->closeConnection();
        e->accept();
    }
    else e->ignore();
}

void MainWindow::updateEthList()
{
    ui->eth_comboBox->clear();
    if(wrapper->getEthInfo(ethInfo) > 0) ui->eth_comboBox->addItems(ethInfo.keys());
}

void MainWindow::onClickConnect(void)
{
    if(wrapper->getExpectedState() != EC_STATE_OPERATIONAL) wrapper->initEth(ethInfo[ui->eth_comboBox->currentText()]);
    else wrapper->closeConnection();
    // robotArm = new RobotArmWrapper;
    // robotArm->init();
}

void MainWindow::onECATStateChanged()
{
    // qDebugMessage(QString::asprintf("[MainWindow::onECATStateChanged] Thread ID: %x", this->thread()->currentThreadId())); // executed at Main Thread, with QueuedConnection
    uint16_t current_state = wrapper->getExpectedState();
    if(controllerWindow && current_state == EC_STATE_OPERATIONAL && (wrapper->getExpectedWKC() == wrapper->getRealWKC())) controllerWindow->controlLoop();
    static uint8_t timer_100ms = 0;
    if(timer_100ms++ >= 99 || current_state != EC_STATE_OPERATIONAL)
    {
        if(current_state == EC_STATE_OPERATIONAL)
        {
            ui->con_pushButton->setText(QString::fromUtf8("Disconnect"));
            ui->actionEEPROM_Update->setEnabled(true);
            ui->actionPayload_Debugger->setEnabled(true);
#ifndef DEBUG_ENVIRONMENT
            ui->enterControllerButton->setEnabled(true);
#endif
        }
        else
        {
            stateViewModel->clear();
            ui->con_pushButton->setText(QString::fromUtf8("Connect"));
            ui->actionEEPROM_Update->setEnabled(false);
            ui->actionPayload_Debugger->setEnabled(false);
#ifndef DEBUG_ENVIRONMENT
            ui->enterControllerButton->setEnabled(false);
#endif
        }
        statusBarStateLabel->setText("State: " + wrapper->getExpectedStateName() + " ");
        statusBarWkcLabel->setText(QString::asprintf("WKC: %d/%d ", wrapper->getRealWKC(), wrapper->getExpectedWKC()));
        statusBarSlaveCountLabel->setText(QString::asprintf("Slave(s): %d ", wrapper->getSlaveCount()));
        ParseStateViewModel();
        timer_100ms = 0;
    }
}

void MainWindow::ParseStateViewModel()
{
    bool newItem = wrapper->slaves.size() > stateViewModel->rowCount();
    for(int i = 0; i < wrapper->slaves.size(); i++)
    {
        QStandardItem *item;
        if(stateViewModel->rowCount() <= i) item = new QStandardItem;
        else item = stateViewModel->item(i);
        item->setColumnCount(2);
        const auto x = wrapper->slaves.values().at(i);
        if(x->input)
        {
            item->setText(QString::asprintf("Slave #%d (", x->slave_id) + x->name + ")");
            AppendDescToItem(QString::asprintf("MotorCount: %d", x->input->Interface_State.MotorCount), 0, item);
            AppendDescToItem(QString::asprintf("MCUTemp: %d", x->input->Interface_State.MCUTemp), 1, item);
            AppendDescToItem(QString::asprintf("VBus: %d", x->input->Interface_State.Vbus), 2, item);
            AppendDescToItem(QString::asprintf("FPS: %d", x->input->Interface_State.FramesPerSec), 3, item);
            AppendDescToItem("Uptime: " + secondsToHHmmss(x->input->Interface_State.Uptime), 4, item);
            AppendDescToItem(QString::asprintf("SN: %llu", x->serial_number), 5, item);
            stateViewModel->setItem(i, item);
        }
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

void MainWindow::onTextBrowserCustomContextMenu(const QPoint &pos)
{
    QScopedPointer<QMenu> pMenu(ui->textBrowser->createStandardContextMenu());
    QScopedPointer<QAction> pAction(new QAction("Clear", pMenu.get()));
    if(ui->textBrowser->toPlainText().size() == 0) pAction->setEnabled(false);
    connect(pAction.get(), &QAction::triggered, ui->textBrowser, &QTextBrowser::clear);
    pMenu->addAction(pAction.get());
    pMenu->exec(ui->textBrowser->mapToGlobal(pos));
}

void MainWindow::showEvent(QShowEvent *e)
{
    centerOnCursorScreen(this);
}

void MainWindow::onErrorMsg(QString s)
{
    // tray->errorMessage(s);
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
    qDebugMessage(s);
}
