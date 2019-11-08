#ifndef ENCODER_H
#define ENCODER_H

#include <opencv2/opencv.hpp>

#include "messagehandler.h"
#include "quantizedframe.h"
#include "eventhandler.h"

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


class Encoder : public MessageHandler, public EventHandler<EncoderEvent>
{
public:
    Encoder(unsigned width, unsigned height);

    int addFrame(cv::Mat frame);
    int postNewMessage(std::string data);

    friend class NewFrameEvent;
    friend class ClientMessageEvent;

private:
    int processEvent(EncoderEvent *ev);
    std::string getUncompressedBlockData(unsigned bi, unsigned bj);
    void sendTickEvent(int timeMs);
    uint64_t computeSAD(unsigned xStart, unsigned xEnd,
                        unsigned yStart, unsigned yEnd);
    void reconstructBlock(unsigned bi, unsigned bj);
    void rankDiffTransBlock();

    QuantizedFrame curCapturedFrame; // Current quantized captured frame
    QuantizedFrame reconstructedFrame; // Current quantized reconstructed frame

    unsigned width, height;
    unsigned nbTransBlockX, nbTransBlockY;

    bool hasClient; // A client is ready to receive the data we send.

    std::priority_queue<RankedTransBlock> rankedTransBlock;

    std::chrono::time_point<std::chrono::system_clock> lastDisplayTime;
};

#endif // ENCODER_H
