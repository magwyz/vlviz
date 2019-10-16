
#include "decoder.h"
#include "log.h"


Decoder::Decoder()
{
    curFrame = cv::Mat(480, 640, CV_8UC3);
}


void Decoder::processData()
{
    std::string data = receiverFIFO.get();
    msg("Data received: " + std::to_string(data.length()));
}
