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
        frameSize = cv::Size(frame.rows, frame.cols);
        if(m_isRecording)
        {
            if(videoWriter.isOpened()) videoWriter.write(frame);
        }
        emit onRefreshFrame(QtCVBridge::cvMatToQImage(frame));
    }
}

bool CaptureWorker::startRecording(QString filePath)
{
    if(m_isRecording) return false;
    if(!isOpened()) return false;
    if(videoWriter.isOpened()) stopRecording();
    if(!videoWriter.open(filePath.toStdString(), cv::VideoWriter::fourcc('H','2','6','4'), 30, frameSize, true)) // CAP_DSHOW, FPS = 0?
    // if(!videoWriter.open(filePath.toStdString(), cv::VideoWriter::fourcc('M','P','4','V'), cap.get(CAP_PROP_FPS), frameSize, true))
    // if(!videoWriter.open(filePath.toStdString(), cv::VideoWriter::fourcc('M','J','P','G'), 30, frameSize, true))
    {
        qDebugMessage("Failed to open VideoWriter: " + filePath);
        return false;
    }
    auto lambda = [this](){
        uint16_t timeout = 500;
        while(!videoWriter.isOpened() && timeout--) QThread::msleep(10);
        if(timeout) m_isRecording = true;
        else m_isRecording = false;
    };
    spawnTask(lambda);
    qDebugMessage("Recording started at " + filePath);
    return true;
}

void CaptureWorker::stopRecording()
{
    m_isRecording = false;
    videoWriter.release();
    qDebugMessage("Recording stopped");
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

bool CaptureWorker::isRecording()
{
    return m_isRecording;
}

void CaptureWorker::openCamera(int id)
{
    if(!cap.isOpened())
    {
        if(!cap.open(id, CAP_DSHOW)) qDebugMessage(QString::asprintf("Failed to open camera #%d", id));
    }
    else qDebugMessage("Camera already opened");
}

void CaptureWorker::closeCamera()
{
    if(cap.isOpened()) cap.release();
}
