#ifndef DECODER_H
#define DECODER_H

#include <thread>

#include <opencv2/opencv.hpp>

#include "messagehandler.h"
#include "quantizedframe.h"
#include "eventhandler.h"

class DecoderEvent;


class Decoder : public MessageHandler, public EventHandler<DecoderEvent>
{
public:
    Decoder();

    int postNewMessage(std::string data);

    friend class Receiver;
    friend class SenderMessageEvent;
private:
    int processEvent(DecoderEvent *ev);
    int processMessage(std::string msg);
    void decodeUncompressedBlock(std::string blockData);

    QuantizedFrame curFrame;
};

#endif // DECODER_H
