#ifndef DECODER_H
#define DECODER_H

#include <opencv2/opencv.hpp>

#include "messagefifo.h"


class Decoder
{
public:
    Decoder();

    void processData();

    MessageFIFO senderFIFO;
    MessageFIFO receiverFIFO;

    friend class Receiver;
private:
    cv::Mat curFrame;

};

#endif // DECODER_H
