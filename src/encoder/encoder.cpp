
#include <string>
#include <future>
#include <chrono>

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
    nbTransBlockX = width / TRANSMISSION_BLOCK_SIZE;
    nbTransBlockY = height / TRANSMISSION_BLOCK_SIZE;

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
    auto lastTime = std::chrono::system_clock::now();

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
            // Send a new block
            if (rankedTransBlock.empty())
                continue;

            const RankedTransBlock &rtb = rankedTransBlock.top();
            unsigned bi = rtb.bi;
            unsigned bj = rtb.bj;
            rankedTransBlock.pop();

            msg("Most diff block: " + std::to_string(bi) + " " + std::to_string(bj));

            reconstructBlock(bi, bj);

            auto curTime = std::chrono::system_clock::now();

            if (std::chrono::duration_cast<std::chrono::milliseconds>(curTime - lastTime)
                > std::chrono::milliseconds(16))
            {
                msg("Display reconstructed frame");
                imshow("Reconstructed frame", reconstructedFrame.getRawFrame().getRGBMat());
                lastTime = curTime;
            }

            //std::string data("test");
            //senderFIFO.put(data);
            sendTickEvent(10);
        }
    }
}


void Encoder::sendTickEvent(int timeMs)
{
    std::async(std::launch::async, [&]() {
        std::this_thread::sleep_for(std::chrono::microseconds(timeMs));
        EncoderEvent *ev = new TickEvent();
        postEvent(ev);
    });
}


void Encoder::reconstructBlock(unsigned bi, unsigned bj)
{
    unsigned xStart = bi * TRANSMISSION_BLOCK_SIZE;
    unsigned xEnd = std::min((bi + 1) * TRANSMISSION_BLOCK_SIZE, width);

    unsigned yStart = bj * TRANSMISSION_BLOCK_SIZE;
    unsigned yEnd = std::min((bj + 1) * TRANSMISSION_BLOCK_SIZE, height);

    for (unsigned x = xStart; x < xEnd; ++x)
        for (unsigned y = yStart; y < yEnd; ++y)
        {
            reconstructedFrame.at(x, y, 0) = curCapturedFrame.at(x, y, 0);
            reconstructedFrame.at(x / 2, y / 2, 1) = curCapturedFrame.at(x / 2, y / 2, 1);
            reconstructedFrame.at(x / 2, y / 2, 2) = curCapturedFrame.at(x / 2, y / 2, 2);
        }
}


uint64_t Encoder::computeSAD(unsigned xStart, unsigned xEnd,
                             unsigned yStart, unsigned yEnd)
{
    uint64_t sum = 0;

    for (unsigned x = xStart; x < xEnd; ++x)
        for (unsigned y = yStart; y < yEnd; ++y)
            sum += std::abs((int)reconstructedFrame.at(x, y, 0)
                            - (int)curCapturedFrame.at(x, y, 0));

    return sum;
}


void Encoder::rankDiffTransBlock()
{
    rankedTransBlock = std::priority_queue<RankedTransBlock>();

    for (unsigned i = 0; i < nbTransBlockX; ++i)
        for (unsigned j = 0; j < nbTransBlockY; j++)
        {
            unsigned xStart = i * TRANSMISSION_BLOCK_SIZE;
            unsigned xEnd = std::min((i + 1) * TRANSMISSION_BLOCK_SIZE, width);

            unsigned yStart = j * TRANSMISSION_BLOCK_SIZE;
            unsigned yEnd = std::min((j + 1) * TRANSMISSION_BLOCK_SIZE, height);

            uint64_t sum = computeSAD(xStart, xEnd, yStart, yEnd);

            rankedTransBlock.emplace(RankedTransBlock(i, j, sum));
        }
}
