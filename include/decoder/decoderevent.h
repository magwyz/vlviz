#ifndef DECODEREVENT_H
#define DECODEREVENT_H

#include "errors.h"
#include "decoder.h"


class DecoderEvent
{
public:
    /**
     * @brief Update the decoder.
     * Must be called from the decoder thread.
     * @return error
     */
    virtual int updateDecoder(Decoder *decoder) = 0;
    virtual ~DecoderEvent()
    { };
};


class SenderMessageEvent : public DecoderEvent
{
public:
    SenderMessageEvent(std::string data)
        : data(data)
    { }

    virtual int updateDecoder(Decoder *decoder)
    {
        msg("Data received: " + std::to_string(data.length()));
        return decoder->processMessage(data);
    }

private:
    std::string data;
};


#endif // DECODEREVENT_H
