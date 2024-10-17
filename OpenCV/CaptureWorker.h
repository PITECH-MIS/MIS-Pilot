#ifndef CAPTUREWORKER_H
#define CAPTUREWORKER_H

#include <QObject>
#include <QtMultimedia>
#include <QCamera>
#include "BridgeUtils.h"

class CaptureWorker : public QObject
{
    Q_OBJECT
public:
    CaptureWorker();
    ~CaptureWorker();
    void setResolution(int w = 400, int h = 400);
    void openCamera(int id);
    void closeCamera();
    bool isOpened();
signals:
    void onRefreshFrame(const QImage &image);
public slots:
    void refreshFrame(bool isCrop = false, float rotationAngle = 0.0f);
private:
    int camId;
    int d = 400;
    cv::VideoCapture cap;
};

#endif // CAPTUREWORKER_H
