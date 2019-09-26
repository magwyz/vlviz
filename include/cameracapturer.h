#ifndef CAMERACAPTURER_H
#define CAMERACAPTURER_H

#include <opencv2/opencv.hpp>

#include "errors.h"

class CameraCapturer
{
public:
    int open();

    int getLastFrame(cv::Mat &frame);

private:
    cv::VideoCapture cap;
};


#endif // CAMERACAPTURER_H
