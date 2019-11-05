#ifndef ENCODEREVENT_H
#define ENCODEREVENT_H

#include <opencv2/opencv.hpp>

#include "errors.h"
#include "encoder.h"
#include "log.h"
#include "rawframe.h"


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
    NewFrameEvent(RawFrame frame)
        : frame(frame)
    { }

    virtual int updateEncoder(Encoder *encoder)
    {
        // Update the current frame of the encoder
        if (frame.width != encoder->width
            && frame.height != encoder->height)
            frame.resize(encoder->width, encoder->height);
        else
            encoder->curCapturedFrame = frame;

        if (encoder->hasClient)
            encoder->rankDiffTransBlock();

        return VLVIZ_SUCCESS;
    }

private:
    RawFrame frame;
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
            encoder->reconstructedFrame = QuantizedFrame(
                    encoder->width, encoder->height);
            encoder->hasClient = true;
        }

        return VLVIZ_SUCCESS;
    }

private:
    std::string data;
};


class TickEvent : public EncoderEvent
{
public:
    TickEvent() { }

    virtual int updateEncoder(Encoder *encoder)
    {
        return VLVIZ_SUCCESS;
    }
};


#endif // ENCODEREVENT_H
