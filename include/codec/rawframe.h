#ifndef RGBFRAME_H
#define RGBFRAME_H

#include <opencv2/opencv.hpp>

/**
 * @brief Raw YUV frame in packed format
 */
class RawFrame
{
public:
    RawFrame(cv::Mat rgbFrame)
    {
        cvtColor(rgbFrame, f, CV_BGR2YUV);
    }

    uint8_t getValue(x, y, plane)
    {
        return f.at<cv::Vec3b>(y, x)[plane];
    }

private:
    cv::Mat f;
};


#endif // RGBFRAME_H
