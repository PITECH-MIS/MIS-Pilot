#include "EndoscopeWrapper.h"

using namespace cv;

EndoscopeWrapper::EndoscopeWrapper(QLabel *label) :
    view(label)
{

}

void EndoscopeWrapper::init()
{
    Mat image = imread("C:\\Users\\vip99\\Documents\\Coding\\segment-anything-2-main\\notebooks\\videos\\bedroom\\00000.jpg", 1);
    auto qImage = cvMatToQImage(image);
    view->setPixmap(QPixmap::fromImage(qImage));
}

QImage EndoscopeWrapper::cvMatToQImage(const cv::Mat &src)
{
    switch(src.type())
    {
    case CV_8UC1:
        {
            QImage result(src.cols, src.rows, QImage::Format_Indexed8);
            result.setColorCount(256);
            for(auto i = 0; i < 256; i++) result.setColor(i, qRgb(i, i, i));
            uchar *pSrc = src.data;
            for(auto row = 0; row < src.rows; row++)
            {
                uchar *pDest = result.scanLine(row);
                memcpy_s(pDest, src.cols, pSrc, src.cols);
                pSrc += src.step;
            }
            return result;
        }
    case CV_8UC3:
        {
            const uchar *pSrc = (const uchar*)src.data;
            QImage result(pSrc, src.cols, src.rows, src.step, QImage::Format_RGB888);
            return result.rgbSwapped();
        }
    case CV_8UC4:
        {
            const uchar *pSrc = (const uchar*)src.data;
            QImage result(pSrc, src.cols, src.rows, src.step, QImage::Format_ARGB32);
            return result.copy();
        }
    default:
        return QImage();
    }
}
