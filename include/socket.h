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
    int port;
    int sock;

    bool stopServer;
    std::mutex stopServerMutex;

    std::thread rt;
    std::thread st;

    MessageFIFO &senderFIFO;
};

void recvThread(bool &stopServer, std::mutex &stopServerMutex,
                int sock);
void sendThread(bool &stopServer, std::mutex &stopServerMutex,
                int sock, MessageFIFO &senderFIFO);


#endif // SOCKET_H
