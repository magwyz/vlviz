
#include <string>

#include "encoder.h"
#include "errors.h"
#include "encoderevent.h"
#include "log.h"

#include "definitions.h"


Encoder::Encoder(unsigned width, unsigned height)
    : stop(false),
      width(width), height(height),
      hasClient(false)
{
    nbTransBlockX = width / TRANS_BLOCK_SIZE;
    nbTransBlockY = height / TRANS_BLOCK_SIZE;

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


int Encoder::postNewMessage(std::string data)
{
    EncoderEvent *ev = new ClientMessageEvent(data);
    return postEvent(ev);
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
        if (!hasClient)
        {
            std::string data("hello");
            senderFIFO.put(data);
        }
        else
        {
            std::string data("test");
            senderFIFO.put(data);
        }
    }
}


int Encoder::selectMostDiffTransBlock(unsigned &bx, unsigned &by)
{
    for (unsigned i = 0; i < nbTransBlockX; ++i)
        for (unsigned j = 0; j < nbTransBlockY; j++)
        {
            unsigned xStart = i * TRANS_BLOCK_SIZE;
            unsigned xEnd = std::min((i + 1) * TRANS_BLOCK_SIZE, width);
        }

    return VLVIZ_SUCCESS;
}
