#ifndef ENCODER_H
#define ENCODER_H

#include <thread>
#include <mutex>
#include <deque>

#include <opencv2/opencv.hpp>

#include "messagehandler.h"
#include "quantizedframe.h"

class EncoderEvent;


class RankedTransBlock
{
public:
    RankedTransBlock(unsigned bi, unsigned bj, uint64_t sad)
        : bi(bi), bj(bj), sad(sad)
    { }

    bool operator<(RankedTransBlock rtb2) const
    {
        return sad < rtb2.sad;
    }

    unsigned bi;
    unsigned bj;
    uint64_t sad;
};


class Encoder : public MessageHandler
{
public:
    Encoder(unsigned width, unsigned height);

    int addFrame(cv::Mat frame);
    void stopEncoder();
    int postNewMessage(std::string data);
    int postEvent(EncoderEvent *ev);

    friend class NewFrameEvent;
    friend class ClientMessageEvent;

private:
    void processThread();
    void sendTickEvent(int timeMs);
    uint64_t computeSAD(unsigned xStart, unsigned xEnd,
                        unsigned yStart, unsigned yEnd);
    void reconstructBlock(unsigned bi, unsigned bj);
    void rankDiffTransBlock();

    std::thread t;

    bool stop;
    std::mutex stopMutex;

    std::deque<EncoderEvent *> events;
    std::condition_variable eventsCondVar;

    QuantizedFrame curCapturedFrame; // Current quantized captured frame
    QuantizedFrame reconstructedFrame; // Current quantized reconstructed frame

    unsigned width, height;
    unsigned nbTransBlockX, nbTransBlockY;

    bool hasClient; // A client is ready to receive the data we send.

    std::priority_queue<RankedTransBlock> rankedTransBlock;
};

#endif // ENCODER_H
