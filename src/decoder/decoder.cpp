
#include "decoder.h"
#include "log.h"
#include "errors.h"
#include "helpers.h"
#include "definitions.h"
#include "decoderevent.h"


Decoder::Decoder()
{
    curFrame = QuantizedFrame(640, 480);
}


int Decoder::postNewMessage(std::string data)
{
    DecoderEvent *ev = new SenderMessageEvent(data);
    return postEvent(ev);
}


int Decoder::processEvent(DecoderEvent *ev)
{
    int ret = ev->updateDecoder(this);
    delete ev;
    return ret;
}


int Decoder::processMessage(std::string msg)
{
    if (msg == std::string("hello"))
    {
        // Send a hello back
        senderFIFO.put(std::string("hello"));
    }
    else if (uint8Decode(msg, 0) == UNCOMPRESSED_TRANSMISSION_BLOCK)
    {
        decodeUncompressedBlock(msg.substr(1));
    }

    return VLVIZ_SUCCESS;
}


void Decoder::decodeUncompressedBlock(std::string blockData)
{
    ssize_t i = 0;
    unsigned bi = uint8Decode(blockData, i++);
    unsigned bj = uint8Decode(blockData, i++);

    msg("Block received: " + std::to_string(bi) + " " + std::to_string(bj));

    unsigned xStart = bi * TRANSMISSION_BLOCK_SIZE;
    unsigned xEnd = std::min((bi + 1) * TRANSMISSION_BLOCK_SIZE, curFrame.width);

    unsigned yStart = bj * TRANSMISSION_BLOCK_SIZE;
    unsigned yEnd = std::min((bj + 1) * TRANSMISSION_BLOCK_SIZE, curFrame.height);

    for (unsigned x = xStart; x < xEnd; ++x)
        for (unsigned y = yStart; y < yEnd; ++y)
            curFrame.at(x, y, 0) = uint8Decode(blockData, i++);
}
