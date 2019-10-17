#ifndef QUANTIZEDFRAME_H
#define QUANTIZEDFRAME_H

#include <opencv2/opencv.hpp>


class QuantizedFrame
{
public:
    QuantizedFrame();

private:
    cv::Mat planes[3];
};


#endif // QUANTIZEDFRAME_H
