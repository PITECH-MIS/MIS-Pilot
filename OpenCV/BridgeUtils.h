#ifndef BRIDGEUTILS_H
#define BRIDGEUTILS_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QImage>

namespace QtCVBridge
{
    QImage cvMatToQImage(const cv::Mat &src);
    cv::Mat cutInCircleMat(const cv::Mat &src, int d = -1);
    cv::Mat rotateMat(const cv::Mat &src, float angle);
};

#endif // BRIDGEUTILS_H
