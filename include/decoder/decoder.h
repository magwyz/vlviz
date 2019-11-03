#ifndef DECODER_H
#define DECODER_H

#include <opencv2/opencv.hpp>

#include "messagehandler.h"
#include "quantizedframe.h"


class Decoder : public MessageHandler
{
public:
    Decoder();

    int postNewMessage(std::string data);
    void processData();

    friend class Receiver;
private:
    QuantizedFrame curFrame;

    MessageFIFO receiverFIFO;
};

#endif // DECODER_H
