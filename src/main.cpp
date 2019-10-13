#include <opencv2/opencv.hpp>

#include "log.h"
#include "cameracapturer.h"
#include "encoder.h"
#include "serversocket.h"


int main(int argc, char** argv)
{
    CameraCapturer cap;

    if (cap.open() != VLVIZ_SUCCESS)
    {
        msg("Could not open camera");
        return 1;
    }

    Encoder encoder;
    ServerSocket serverSocket(424242, encoder.senderFIFO);

    if (serverSocket.bindServer() != VLVIZ_SUCCESS)
        return 1;

    while (true)
    {
          cv::Mat frame;
          cap.getLastFrame(frame);

          if (encoder.addFrame(frame) != VLVIZ_SUCCESS)
          {
              msg("Error encoding the last frame.");
              break;
          }

          imshow("this is you, smile! :)", frame);

          if (cv::waitKey(10) == 27)
              break;
    }

    serverSocket.closeServer();

    return 0;
}

