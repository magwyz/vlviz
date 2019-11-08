
#include <opencv2/opencv.hpp>

#include "sender.h"
#include "cameracapturer.h"
#include "encoder.h"
#include "socket.h"
#include "log.h"
#include "encoderevent.h"


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

    Encoder encoder(640, 480);
    Socket socket(4213, 4212, encoder);

    if (socket.bindSocket() != VLVIZ_SUCCESS)
        return;

    while (true)
    {
        RawFrame rf;
        cap.getLastFrame(rf);

        NewFrameEvent *ev = new NewFrameEvent(rf);

        if (encoder.postEvent(ev))
        {
            msg("Error pushing the last frame.");
            break;
        }

        //imshow("this is you, smile! :)", rf.getRGBMat());

        if (cv::waitKey(10) == 27)
            break;
    }

    encoder.stop();
    socket.closeSocket();
}
