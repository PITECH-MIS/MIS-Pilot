#include "BridgeUtils.h"

using namespace cv;

namespace QtCVBridge
{
    cv::Mat rotateMat(const cv::Mat &src, float angle)
    {
        Point2f center(src.cols * 0.5f, src.rows * 0.5f);
        Mat rotationMatrix = getRotationMatrix2D(center, (double)angle, 1.0);
        Mat result;
        warpAffine(src, result, rotationMatrix, src.size());
        return result;
    }

    cv::Mat cutInCircleMat(const cv::Mat &src, int d)
    {
        if(d <= 0 || d > std::min(src.cols, src.rows)) d = std::min(src.cols, src.rows);
        int radius = d / 2;
        Point2f center(src.cols * 0.5f, src.rows * 0.5f);
        Mat mask(src.size(), CV_8UC1, Scalar::all(0));
        circle(mask, center, radius, Scalar::all(255), -1);
        Rect rRoi(center.x - radius, center.y - radius, d, d);
        Mat masked;
        if(src.channels() == 3) cvtColor(src, masked, cv::COLOR_BGR2BGRA);
        else masked = src.clone();
        // #1
        std::vector<cv::Mat> channels;
        split(masked, channels);
        channels[3].setTo(0, mask == 0);
        merge(channels, masked);
        // #2
        // for(auto y = 0; y < src.rows; ++y)
        // {
        //     for(auto x = 0; x < src.cols; ++x) if(mask.at<uchar>(y, x) == 0) masked.at<cv::Vec4b>(y, x)[3] = 0;
        // }
        return masked(rRoi);
    }

    QImage cvMatToQImage(const cv::Mat &src)
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
#ifdef ENV_WIN32
                memcpy_s(pDest, src.cols, pSrc, src.cols);
#else
                memccpy(pDest, pSrc, src.cols, src.cols);
#endif
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
}
