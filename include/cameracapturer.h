#ifndef CAMERACAPTURER_H
#define CAMERACAPTURER_H

#include <opencv2/opencv.hpp>

#include "rawframe.h"
#include "errors.h"

class CameraCapturer
{
public:
    int open();

    int getLastFrame(RawFrame &rf);

private:
    cv::VideoCapture cap;
};


#endif // CAMERACAPTURER_H
