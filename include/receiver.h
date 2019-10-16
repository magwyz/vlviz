#ifndef RECEIVER_H
#define RECEIVER_H

#include <thread>

class Receiver
{
public:
    void start();
    void join();

private:
    void threadFunc();

    std::thread t;
};


#endif // RECEIVER_H
