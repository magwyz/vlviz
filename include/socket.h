#ifndef SOCKET_H
#define SOCKET_H

#include <thread>
#include <mutex>

#include "messagefifo.h"


class Socket
{
public:
    Socket(int port,
           MessageFIFO &senderFIFO, MessageFIFO &receiverFIFO);

    int bindSocket();
    void closeSocket();

private:
    void recvThread();
    void sendThread();

    int port;
    int sock;

    bool stop;
    std::mutex stopMutex;

    std::thread rt;
    std::thread st;

    MessageFIFO &senderFIFO;
    MessageFIFO &receiverFIFO;
};


#endif // SOCKET_H
