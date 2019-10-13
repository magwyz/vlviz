#ifndef MESSAGEFIFO_H
#define MESSAGEFIFO_H

#include <string>
#include <deque>
#include <mutex>
#include <condition_variable>


/**
 * @brief A message FIFO that can be accessed from multiple threads.
 */
class MessageFIFO
{
public:
    void put(std::string inStr)
    {
        std::lock_guard<std::mutex> lock(fifoMutex);
        fifo.push_back(inStr);
        hasMsgCondVar.notify_one();
    }

    /**
     * @brief Get a message from the FIFO.
     * If the FIFO is empty, block until a new message comes.
     * @return the message
     */
    std::string get()
    {
        std::unique_lock<std::mutex> lk(fifoMutex);
        hasMsgCondVar.wait(lk, [&]{return !fifo.empty();});

        std::string outStr = fifo.front();
        fifo.pop_front();

        lk.unlock();
        hasMsgCondVar.notify_one();

        return outStr;
    }

private:
    std::deque<std::string> fifo;
    std::mutex fifoMutex;
    std::condition_variable hasMsgCondVar;
};


#endif // MESSAGEFIFO_H
