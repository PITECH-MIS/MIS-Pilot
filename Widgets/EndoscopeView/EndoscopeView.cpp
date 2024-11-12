#include "EndoscopeView.h"
#include "ui_EndoscopeView.h"
#include "utils.h"
#include <QFileDialog>
#include <QStandardPaths>

EndoscopeView::EndoscopeView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EndoscopeView)
{
    ui->setupUi(this);
    onRefreshCameraList();
    workerThread = new QThread(this);
    worker = new CaptureWorker;
    captureTimer = new QTimer(this);
    // captureTimer->setTimerType(Qt::PreciseTimer);
    worker->moveToThread(workerThread);
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);
    connect(workerThread, &QThread::finished, worker, &CaptureWorker::deleteLater);
    connect(captureTimer, &QTimer::timeout, this, &EndoscopeView::onCaptureTimeout);
    connect(worker, &CaptureWorker::onRefreshFrame, this, &EndoscopeView::refreshFrame);
    onSelectFPS();
    connect(ui->cameraRefreshButton, &QToolButton::triggered, this, &EndoscopeView::onRefreshCameraList);
    connect(ui->fpsComboBox, &QComboBox::currentIndexChanged, this, &EndoscopeView::onSelectFPS);
    connect(ui->connectButton, &QPushButton::clicked, this, &EndoscopeView::onConnectToCamera);
    connect(ui->recordPathSetButton, &QPushButton::clicked, this, &EndoscopeView::onSelectRecordPath);
    ui->recordButton->setEnabled(false);
    connect(ui->recordButton, &QPushButton::clicked, this, &EndoscopeView::onTriggerRecording);
}

EndoscopeView::~EndoscopeView()
{
    captureTimer->stop();
    workerThread->quit();
    workerThread->wait();
    delete ui;
}

void EndoscopeView::addEquipList(QStringList equips)
{
    ui->rotCorrectEquipComboBox->addItems(equips);
}

void EndoscopeView::addActuatorList(QStringList actuators)
{
    ui->rotCorrectActuatorComboBox->addItems(actuators);
}

void EndoscopeView::onConnectToCamera()
{
    if(ui->connectButton->text() == "Connect")
    {
        if(cameraList.size() > 0)
        {
            workerThread->start();
            if(!captureTimer->isActive()) captureTimer->start();
            worker->openCamera(ui->cameraComboBox->currentIndex());
            // worker->setResolution();
            auto lambda = [this]()
            {
                int timeout = 1000;
                while(!worker->isOpened())
                {
                    timeout--;
                    QThread::msleep(10);
                }
                if(timeout > 0)
                {
                    ui->connectButton->setText("Disconnect");
                }
            };
            spawnTask(lambda);
            if(!recordPath.isEmpty()) ui->recordButton->setEnabled(true);
            ui->rotCorrectCheckBox->setEnabled(true);
        }
    }
    else
    {
        ui->cameraView->clear();
        captureTimer->stop();
        auto lambda = [this]()
        {
            if(worker->isRecording()) worker->stopRecording();
            worker->closeCamera();
            ui->connectButton->setText("Connect");
            ui->groupBox->setTitle("Camera View");
            while(captureTimer->isActive());
            QThread::msleep(10);
            ui->cameraView->clear();
        };
        spawnTask(lambda);
        ui->recordButton->setEnabled(false);
        ui->rotCorrectCheckBox->setEnabled(false);
    }
}

void EndoscopeView::refreshFrame(const QImage &image)
{
    ui->cameraView->setPixmap(QPixmap::fromImage(image));
}

void EndoscopeView::onTriggerRecording()
{
    if(worker->isRecording())
    {
        worker->stopRecording();
        auto lambda = [this]()
        {
            while(worker->isRecording());
            ui->recordButton->setText("Start Rec.");
        };
        spawnTask(lambda);
        ui->recordButton->setEnabled(true);
    }
    else
    {
        if(!recordPath.isEmpty())
        {
            QString videoPath = recordPath + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".mp4";
            ui->recordButton->setEnabled(false);
            if(worker->startRecording(videoPath)) ui->recordButton->setText("Stop Rec.");
            ui->recordButton->setEnabled(true);
        }
    }
}

void EndoscopeView::onSelectRecordPath()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setModal(false);
    auto videoDir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    QDir descDir(videoDir + "/MIS-Pilot-Record");
    if(!descDir.exists()) descDir.mkpath(descDir.path());
    dialog.setDirectory(descDir);
    connect(&dialog, &QFileDialog::fileSelected, this, [this](const QString& file){
        // qDebugMessage("Selected path: " + file);
        recordPath = file;
        ui->recordPathLineEdit->setText(getElidedString(recordPath, ui->recordPathLineEdit->font(), ui->recordPathLineEdit->width() - 5));
        if(worker->isOpened()) ui->recordButton->setEnabled(true);
    });
    dialog.open();
    while(!dialog.isHidden()) QApplication::processEvents();
}

void EndoscopeView::onCaptureTimeout()
{
    isCrop = true;
    if(!ui->rotCorrectCheckBox->isChecked())
    {
        ui->groupBox->setTitle("Camera View - Fixed");
    }
    auto lambda = [this]()
    {
        worker->refreshFrame(isCrop, rotationAngle);
    };
    spawnTask(lambda);
}

void EndoscopeView::onRefreshCameraList()
{
    ui->cameraComboBox->clear();
    cameraList.clear();
    cameraList = QMediaDevices::videoInputs();
    for(const auto &camera : cameraList) ui->cameraComboBox->addItem(camera.description());
    if(cameraList.size() > 0) ui->connectButton->setEnabled(true);
    else ui->connectButton->setEnabled(false);
}

void EndoscopeView::onSelectFPS()
{
    auto fps = ui->fpsComboBox->currentText().toInt();
    if(fps > 0 && fps < 240) captureTimer->setInterval(1000 / fps);
}
