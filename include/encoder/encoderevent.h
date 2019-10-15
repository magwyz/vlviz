#ifndef ENCODEREVENT_H
#define ENCODEREVENT_H

#include <opencv2/opencv.hpp>

#include "errors.h"
#include "encoder.h"


class EncoderEvent
{
public:
    /**
     * @brief Update the encoder.
     * Must be called from the encoder thread.
     * @return error
     */
    virtual int updateEncoder(Encoder *encoder) = 0;
};


class NewFrameEvent : public EncoderEvent
{
public:
    NewFrameEvent(cv::Mat frame)
        : frame(frame)
    { }

    virtual int updateEncoder(Encoder *encoder)
    {
        // Update the current frame of the encoder
        encoder->curCapturedFrame = frame;
        return VLVIZ_SUCCESS;
    }

private:
    cv::Mat frame;
};

#endif // ENCODEREVENT_H
