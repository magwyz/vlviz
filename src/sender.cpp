
#include <opencv2/opencv.hpp>

#include "sender.h"
#include "cameracapturer.h"
#include "encoder.h"
#include "socket.h"
#include "log.h"


void Sender::start()
{
    t = std::thread(&Sender::threadFunc, this);
}


void Sender::join()
{
    t.join();
}


void Sender::threadFunc()
{
    CameraCapturer cap;

    if (cap.open() != VLVIZ_SUCCESS)
    {
        msg("Could not open camera");
        return;
    }

    Encoder encoder;
    Socket socket(424242, encoder.senderFIFO);

    if (socket.bindSocket() != VLVIZ_SUCCESS)
        return;

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

    socket.closeSocket();
}
