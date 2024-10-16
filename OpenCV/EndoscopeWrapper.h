#ifndef ENDOSCOPEWRAPPER_H
#define ENDOSCOPEWRAPPER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QImage>
#include <QLabel>

// https://blog.csdn.net/weixin_43763292/article/details/112975207

class EndoscopeWrapper
{
public:
    EndoscopeWrapper(QLabel *label);
    void init();
private:
    QImage cvMatToQImage(const cv::Mat &src);
    QLabel *view;
};

#endif // ENDOSCOPEWRAPPER_H
