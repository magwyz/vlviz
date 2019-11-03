#ifndef RGBFRAME_H
#define RGBFRAME_H

#include <opencv2/opencv.hpp>

#include "quantizedframe.h"


/**
 * @brief Raw YUV frame in packed format
 */
class RawFrame
{
public:
    RawFrame()
        : width(0), height(0)
    { }

    RawFrame(cv::Mat frame, bool yuv = false)
    {
        fromMat(frame, yuv);
    }

    RawFrame(const RawFrame &rf2)
    {
        f = rf2.f;
        width = rf2.width;
        height = rf2.height;
    }

    cv::Mat getRGBMat()
    {
        cv::Mat rgbFrame;
        cvtColor(f, rgbFrame, CV_YUV2BGR);
        return rgbFrame;
    }

    void resize(unsigned newWidth, unsigned newHeight)
    {
        cv::resize(f, f, cv::Size(newWidth, newHeight));
    }

    uint8_t at(unsigned x, unsigned y, unsigned plane)
    {
        return f.at<cv::Vec3b>(y, x)[plane];
    }

    unsigned width;
    unsigned height;

private:

    void fromMat(cv::Mat frame, bool yuv)
    {
        if (!yuv)
            cvtColor(frame, f, CV_BGR2YUV);
        else
            f = frame;
        width = frame.cols;
        height = frame.rows;
    }

    cv::Mat f;
};


#endif // RGBFRAME_H
