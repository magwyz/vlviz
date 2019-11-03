#ifndef QUANTIZEDFRAME_H
#define QUANTIZEDFRAME_H

#include <opencv2/opencv.hpp>

class RawFrame;


class QuantizedFrame
{
public:
    QuantizedFrame();
    QuantizedFrame(unsigned width, unsigned height);
    QuantizedFrame(RawFrame rf);

    RawFrame getRawFrame();

    unsigned width;
    unsigned height;

private:
    cv::Mat planes[3];
};


#endif // QUANTIZEDFRAME_H
