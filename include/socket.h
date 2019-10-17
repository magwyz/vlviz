#ifndef SOCKET_H
#define SOCKET_H

#include <thread>
#include <mutex>

#include "messagehandler.h"


class Socket
{
public:
    Socket(int dstPort, int localPort,
           MessageHandler &messageHandler);

    int bindSocket();
    void closeSocket();

private:
    void recvThread();
    void sendThread();

    int dstPort;
    int localPort;
    int sock;

    bool stop;
    std::mutex stopMutex;

    std::thread rt;
    std::thread st;

    MessageHandler &messageHandler;
};


#endif // SOCKET_H
