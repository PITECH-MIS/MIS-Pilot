#ifndef ENDOSCOPEVIEW_H
#define ENDOSCOPEVIEW_H

#include <QWidget>
#include <QTimer>
#include <QThread>
#include "OpenCV/CaptureWorker.h"

namespace Ui {
class EndoscopeView;
}

class EndoscopeView : public QWidget
{
    Q_OBJECT

public:
    explicit EndoscopeView(QWidget *parent = nullptr);
    ~EndoscopeView();
    bool isCrop = false;
    float rotationAngle = 0.0f;
private slots:
    void onConnectToCamera();
    void onCaptureTimeout();
    void onRefreshCameraList();
    void onSelectFPS();
    void refreshFrame(const QImage &image);
private:
    Ui::EndoscopeView *ui;
    QList<QCameraDevice> cameraList;
    QString recordPath;
    QThread *workerThread;
    QTimer *captureTimer;
    CaptureWorker *worker;
};

#endif // ENDOSCOPEVIEW_H
