#include <opencv2/opencv.hpp>

#include "log.h"
#include "cameracapturer.h"


int main(int argc, char** argv)
{
    CameraCapturer cap;

    if (cap.open() != VLVIZ_SUCCESS)
    {
        msg("Could not open camera");
        return 1;
    }

    while (true)
    {
          cv::Mat frame;
          cap.getLastFrame(frame);
          imshow("this is you, smile! :)", frame);
          if (cv::waitKey(10) == 27)
              break;
    }

    return 0;
}

