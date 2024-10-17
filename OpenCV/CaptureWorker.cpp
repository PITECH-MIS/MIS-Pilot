#include "CaptureWorker.h"
#include "../utils.h"

using namespace cv;

CaptureWorker::CaptureWorker() {}

CaptureWorker::~CaptureWorker()
{
    closeCamera();
    qDebugMessage("CaptureWorker destroyed");
}

void CaptureWorker::refreshFrame(bool isCrop, float rotationAngle)
{
    Mat frame;
    if(cap.read(frame))
    {
        if(isCrop) frame = QtCVBridge::cutInCircleMat(frame, d);
        frame = QtCVBridge::rotateMat(frame, rotationAngle);
        emit onRefreshFrame(QtCVBridge::cvMatToQImage(frame));
    }
}

void CaptureWorker::setResolution(int w, int h)
{
    if(!cap.set(CAP_PROP_FRAME_WIDTH, (double)w)) qDebugMessage("Failed to set CAP_PROP_FRAME_WIDTH");
    if(!cap.set(CAP_PROP_FRAME_HEIGHT, (double)h)) qDebugMessage("Failed to set CAP_PROP_FRAME_HEIGHT");
    d = std::min(w, h);
}

bool CaptureWorker::isOpened()
{
    return cap.isOpened();
}

void CaptureWorker::openCamera(int id)
{
    if(!cap.isOpened())
    {
        if(!cap.open(id)) qDebugMessage(QString::asprintf("Failed to open camera #%d", id));
    }
    else qDebugMessage("Camera already opened");
}

void CaptureWorker::closeCamera()
{
    if(cap.isOpened()) cap.release();
}
