#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>

#include "errors.h"


template <class Event> class EventHandler
{
public:
    EventHandler()
        : mustStop(false)
    {
        t = std::thread(&EventHandler::processThread, this);
    }

    void stop()
    {
        {
            std::lock_guard<std::mutex> lock(stopMutex);
            mustStop = true;
            eventsCondVar.notify_one();
        }
        t.join();
    }

    int postEvent(Event *ev)
    {
        {
            std::lock_guard<std::mutex> lock(stopMutex);
            events.push_back(ev);
            eventsCondVar.notify_one();
        }

        return VLVIZ_SUCCESS;
    }

private:

    void processThread()
    {
        while (true)
        {
            std::unique_lock<std::mutex> lk(stopMutex);
            eventsCondVar.wait(lk, [&]{return !events.empty() || mustStop;});

            if (mustStop)
                break;

            Event *ev = events.front();
            events.pop_front();
            lk.unlock();
            eventsCondVar.notify_one();

            processEvent(ev);
        }
    }

    virtual int processEvent(Event *ev) = 0;

    std::thread t;

    bool mustStop;
    std::mutex stopMutex;

    std::deque<Event *> events;
    std::condition_variable eventsCondVar;
};


#endif // EVENTHANDLER_H
