#ifndef ENCODER_H
#define ENCODER_H

#include <thread>
#include <mutex>
#include <deque>

#include <opencv2/opencv.hpp>

#include "messagefifo.h"

class EncoderEvent;


class Encoder
{
public:
    Encoder();

    int addFrame(cv::Mat frame);
    void stopEncoder();
    int postEvent(EncoderEvent *ev);

    MessageFIFO senderFIFO;
    MessageFIFO receiverFIFO;

    friend class NewFrameEvent;

private:
    void processThread();

    std::thread t;

    bool stop;
    std::mutex stopMutex;

    std::deque<EncoderEvent *> events;
    std::condition_variable eventsCondVar;

    cv::Mat curCapturedFrame;
};

#endif // ENCODER_H
