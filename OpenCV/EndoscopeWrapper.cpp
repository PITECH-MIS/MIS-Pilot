#include "EndoscopeWrapper.h"

using namespace cv;

EndoscopeWrapper::EndoscopeWrapper() {}

void EndoscopeWrapper::init()
{
    Mat image = imread("C:\\Users\\vip99\\Documents\\Coding\\segment-anything-2-main\\notebooks\\videos\\bedroom\\00000.jpg", 1);
    namedWindow("DisplayWindow");
    imshow("DisplayWindow", image);
}
