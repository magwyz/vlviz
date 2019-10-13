
#include <string>

#include "encoder.h"
#include "errors.h"


int Encoder::addFrame(cv::Mat frame)
{
    /*std::string data(reinterpret_cast<char *>(frame.data),
                     frame.rows * frame.cols * frame.elemSize1());*/
    std::string data(reinterpret_cast<char *>(frame.data), 100);

    senderFIFO.put(data);

    return VLVIZ_SUCCESS;
}
