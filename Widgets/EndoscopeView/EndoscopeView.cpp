#include "EndoscopeView.h"
#include "ui_EndoscopeView.h"
#include "utils.h"

EndoscopeView::EndoscopeView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EndoscopeView)
{
    ui->setupUi(this);
    onRefreshCameraList();
    workerThread = new QThread;
    worker = new CaptureWorker;
    captureTimer = new QTimer(this);
    captureTimer->setTimerType(Qt::PreciseTimer);
    worker->moveToThread(workerThread);
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);
    connect(workerThread, &QThread::finished, worker, &CaptureWorker::deleteLater);
    connect(captureTimer, &QTimer::timeout, this, &EndoscopeView::onCaptureTimeout);
    connect(worker, &CaptureWorker::onRefreshFrame, this, &EndoscopeView::refreshFrame);
    onSelectFPS();
    workerThread->start();
    connect(ui->cameraRefreshButton, &QToolButton::triggered, this, &EndoscopeView::onRefreshCameraList);
    connect(ui->fpsComboBox, &QComboBox::currentIndexChanged, this, &EndoscopeView::onSelectFPS);
    connect(ui->connectButton, &QPushButton::clicked, this, &EndoscopeView::onConnectToCamera);
}

EndoscopeView::~EndoscopeView()
{
    captureTimer->stop();
    workerThread->quit();
    workerThread->wait();
    delete ui;
}

void EndoscopeView::onConnectToCamera()
{
    if(ui->connectButton->text() == "Connect")
    {
        if(cameraList.size() > 0)
        {
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
        }
    }
    else
    {
        auto lambda = [this]()
        {
            ui->cameraView->clear();
            captureTimer->stop();
            worker->closeCamera();
            ui->connectButton->setText("Connect");
            ui->groupBox->setTitle("Camera View");
            while(captureTimer->isActive());
            QThread::msleep(10);
            ui->cameraView->clear();
        };
        spawnTask(lambda);
    }
}

void EndoscopeView::refreshFrame(const QImage &image)
{
    ui->cameraView->setPixmap(QPixmap::fromImage(image));
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
    auto lambda = [this]()
    {
        cameraList = QMediaDevices::videoInputs();
        for(const auto &camera : cameraList) ui->cameraComboBox->addItem(camera.description());
    };
    spawnTask(lambda);
    if(cameraList.size() > 0) ui->connectButton->setEnabled(true);
    else ui->connectButton->setEnabled(false);
}

void EndoscopeView::onSelectFPS()
{
    auto fps = ui->fpsComboBox->currentText().toInt();
    if(fps > 0 && fps < 240) captureTimer->setInterval(1000 / fps);
}
