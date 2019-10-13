#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <thread>
#include <mutex>

#include "messagefifo.h"


class ServerSocket
{
public:
    ServerSocket(int port, MessageFIFO &senderFIFO);

    int bindServer();
    void closeServer();

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


#endif // SERVERSOCKET_H
