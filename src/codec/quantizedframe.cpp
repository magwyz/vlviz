
#include "quantizedframe.h"
#include "rawframe.h"


QuantizedFrame::QuantizedFrame()
    : width(0), height(0)
{ }


QuantizedFrame::QuantizedFrame(unsigned width, unsigned height)
    : width(width), height(height)
{
    for (unsigned i = 0; i < 3; ++i)
        planes[i] = cv::Mat(i == 0 ? height : height / 2,
                            i == 0 ? width : width / 2,
                            CV_8UC1);
}


QuantizedFrame::QuantizedFrame(RawFrame rf)
    : width(rf.width), height(rf.height)
{
    for (unsigned i = 0; i < 3; ++i)
        planes[i] = cv::Mat(rf.height, rf.width, CV_8UC1);

    for (unsigned y = 0; y < rf.height; ++y)
        for (unsigned x = 0; x < rf.width; ++x)
            for (unsigned i = 0; i < 3; ++i)
                planes[i].at<uint8_t>(y, x) = rf.at(x, y, i);

    // Quantize the chroma plane
    for (unsigned i = 1; i < 3; ++i)
        resize(planes[i], planes[i], cv::Size(rf.width / 2, rf.height / 2));
}


RawFrame QuantizedFrame::getRawFrame()
{
    cv::Mat yuvMat(height, width, CV_8UC3);

    for (unsigned y = 0; y < height; ++y)
        for (unsigned x = 0; x < width; ++x)
            for (unsigned i = 0; i < 3; ++i)
                yuvMat.at<cv::Vec3b>(y, x)[i] =
                    planes[i].at<uint8_t>(i == 0 ? y : y / 2,
                                          i == 0 ? x : x / 2);

    return RawFrame(yuvMat, true);
}


uint8_t &QuantizedFrame::at(unsigned x, unsigned y, unsigned plane)
{
    return planes[plane].at<uint8_t>(y, x);
}
