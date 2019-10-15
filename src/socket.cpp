
#include "socket.h"
#include "log.h"
#include "errors.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */


Socket::Socket(int port,
               MessageFIFO &senderFIFO, MessageFIFO &receiverFIFO)
    : port(port), stop(false),
      senderFIFO(senderFIFO), receiverFIFO(receiverFIFO)
{ }


int Socket::bindSocket()
{
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        msg("Cannot create socket");
        return VLVIZ_ENETWORK;
    }

    struct sockaddr_in sin = {0};

    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) == -1)
    {
        msg("Cannot bind socket");
        return VLVIZ_ENETWORK;
    }

    rt = std::thread(&Socket::recvThread, this);
    st = std::thread(&Socket::sendThread, this);


    return VLVIZ_SUCCESS;
}


void Socket::closeSocket()
{
    // Put an empty string to wake up the sending thread.
    senderFIFO.put(std::string());

    close(sock);
    {
        std::lock_guard<std::mutex> lock(stopMutex);
        stop = true;
    }
    rt.join();
    st.join();
}


void Socket::recvThread()
{
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(stopMutex);
            if (stop)
                break;
        }

        char buff[1500] = { 0 };
        sockaddr_in from;
        socklen_t fromlen = sizeof(from);
        int ret = recvfrom(sock, buff, sizeof(buff), 0,
                           reinterpret_cast<sockaddr*>(&from), &fromlen);
        if (ret <= 0)
        {
            msg("Error receiving data");
            break;
        }

        receiverFIFO.put(std::string(buff, ret));

    }
}


void Socket::sendThread()
{
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(stopMutex);
            if (stop)
                break;
        }

        std::string data = senderFIFO.get();
        // FIXME hack: if we get an empty string, it's time to quit...
        if (data.empty())
            break;

        unsigned portDst = port;

        sockaddr_in to = { 0 };
        inet_pton(AF_INET, "127.0.0.1", &to.sin_addr.s_addr);
        to.sin_family = AF_INET;
        to.sin_port = htons(portDst);

        int ret = sendto(sock, data.data(),
                         static_cast<int>(data.length()),
                         0, reinterpret_cast<const sockaddr*>(&to),
                         sizeof(to));
        if (ret <= 0)
        {
            msg("Error sending data");
            break;
        }

        msg("Data send: " + ts(data.length()));
    }
}
