
#include "decoder.h"
#include "log.h"
#include "errors.h"


Decoder::Decoder()
{
    curFrame = cv::Mat(480, 640, CV_8UC3);
}


int Decoder::postNewMessage(std::string data)
{
    receiverFIFO.put(data);
    return VLVIZ_SUCCESS;
}


void Decoder::processData()
{
    std::string data = receiverFIFO.get();
    msg("Data received: " + std::to_string(data.length()));

    if (data == std::string("hello"))
    {
        // Send a hello back
        senderFIFO.put(std::string("hello"));
    }
}
