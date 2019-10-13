#ifndef ENCODER_H
#define ENCODER_H

#include <opencv2/opencv.hpp>

#include "messagefifo.h"


class Encoder
{
public:
    int addFrame(cv::Mat frame);

    MessageFIFO senderFIFO;

};


#endif // ENCODER_H
