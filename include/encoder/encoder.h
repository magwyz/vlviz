#ifndef ENCODER_H
#define ENCODER_H

#include <thread>
#include <mutex>
#include <deque>

#include <opencv2/opencv.hpp>

#include "messagehandler.h"

class EncoderEvent;


class Encoder : public MessageHandler
{
public:
    Encoder(unsigned width, unsigned height);

    int addFrame(cv::Mat frame);
    void stopEncoder();
    int postNewMessage(std::string data);
    int postEvent(EncoderEvent *ev);

    friend class NewFrameEvent;
    friend class ClientMessageEvent;

private:
    void processThread();
    int selectMostDiffTransBlock(unsigned &bx, unsigned &by);

    std::thread t;

    bool stop;
    std::mutex stopMutex;

    std::deque<EncoderEvent *> events;
    std::condition_variable eventsCondVar;

    cv::Mat curCapturedFrame; // Current quantized captured frame
    cv::Mat reconstructedFrame; // Current quantized reconstructed frame

    unsigned width, height;
    unsigned nbTransBlockX, nbTransBlockY;

    bool hasClient; // A client is ready to receive the data we send.
};

#endif // ENCODER_H
