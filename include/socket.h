#ifndef SOCKET_H
#define SOCKET_H

#include <thread>
#include <mutex>

#include "messagefifo.h"


class Socket
{
public:
    Socket(int port, MessageFIFO &senderFIFO);

    int bindSocket();
    void closeSocket();

private:
    void recvThread();
    void sendThread();

    int port;
    int sock;

    bool stopServer;
    std::mutex stopServerMutex;

    std::thread rt;
    std::thread st;

    MessageFIFO &senderFIFO;
};


#endif // SOCKET_H
