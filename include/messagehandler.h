#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "messagefifo.h"


class MessageHandler
{
public:
    virtual int postNewMessage(std::string data) = 0;

    MessageFIFO senderFIFO;
};

#endif // MESSAGEHANDLER_H
