#ifndef SENDER_H
#define SENDER_H

#include <thread>


class Sender
{
public:
    void start();
    void join();

private:
    void threadFunc();

    std::thread t;
};


#endif // SENDER_H
