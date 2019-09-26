
#include "log.h"
#include "cameracapturer.h"


int CameraCapturer::open()
{
    if (!cap.open(0))
        return VLVIZ_EGENERIC;

    return VLVIZ_SUCCESS;
}


int CameraCapturer::getLastFrame(cv::Mat &frame)
{
    bool hasReadFrame = cap.read(frame);

    return hasReadFrame ? VLVIZ_SUCCESS : VLVIZ_NO_FRAME;
}
