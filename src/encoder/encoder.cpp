
#include <string>

#include "encoder.h"
#include "errors.h"
#include "encoderevent.h"
#include "log.h"


Encoder::Encoder()
    : stop(false)
{
    t = std::thread(&Encoder::processThread, this);
}


void Encoder::stopEncoder()
{
    {
        std::lock_guard<std::mutex> lock(stopMutex);
        stop = true;
        eventsCondVar.notify_one();
    }
    t.join();
}


int Encoder::postEvent(EncoderEvent *ev)
{
    {
        std::lock_guard<std::mutex> lock(stopMutex);
        events.push_back(ev);
        eventsCondVar.notify_one();
    }

    return VLVIZ_SUCCESS;
}


void Encoder::processThread()
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(stopMutex);
        eventsCondVar.wait(lk, [&]{return !events.empty() || stop;});

        if (stop)
            break;

        EncoderEvent *ev = events.front();
        events.pop_front();
        lk.unlock();
        eventsCondVar.notify_one();

        ev->updateEncoder(this);
        delete ev;

        // Encode a part of the frame.

        std::string data(reinterpret_cast<char *>(curCapturedFrame.data), 100);
        senderFIFO.put(data);

    }
}
