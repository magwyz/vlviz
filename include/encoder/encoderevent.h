#ifndef ENCODEREVENT_H
#define ENCODEREVENT_H

#include <opencv2/opencv.hpp>

#include "errors.h"
#include "encoder.h"
#include "log.h"


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
        if (frame.cols != encoder->width
            && frame.rows != encoder->height)
            resize(frame, encoder->curCapturedFrame,
                   cv::Size(encoder->width, encoder->height));
        else
            encoder->curCapturedFrame = frame;
        return VLVIZ_SUCCESS;
    }

private:
    cv::Mat frame;
};


class ClientMessageEvent : public EncoderEvent
{
public:
    ClientMessageEvent(std::string data)
        : data(data)
    { }

    virtual int updateEncoder(Encoder *encoder)
    {
        msg("Received: " + data);
        if (data == std::string("hello"))
        {
            encoder->reconstructedFrame = cv::Mat(
                    encoder->width, encoder->height, CV_8UC3);
            encoder->hasClient = true;
        }

        return VLVIZ_SUCCESS;
    }

private:
    std::string data;
};


#endif // ENCODEREVENT_H
