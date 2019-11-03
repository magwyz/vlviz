
#include "log.h"
#include "cameracapturer.h"


int CameraCapturer::open()
{
    if (!cap.open(0))
        return VLVIZ_EGENERIC;

    return VLVIZ_SUCCESS;
}


int CameraCapturer::getLastFrame(RawFrame &rf)
{
    cv::Mat frame;
    bool hasReadFrame = cap.read(frame);
    rf = RawFrame(frame);

    return hasReadFrame ? VLVIZ_SUCCESS : VLVIZ_NO_FRAME;
}
