
#include "receiver.h"
#include "decoder.h"
#include "socket.h"
#include "errors.h"
#include "rawframe.h"


void Receiver::start()
{
    t = std::thread(&Receiver::threadFunc, this);
}


void Receiver::join()
{
    t.join();
}


void Receiver::threadFunc()
{
    Decoder decoder;
    Socket socket(4212, 4213, decoder);

    if (socket.bindSocket() != VLVIZ_SUCCESS)
        return;

    auto lastTime = std::chrono::system_clock::now();

    while (true)
    {
        auto curTime = std::chrono::system_clock::now();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(curTime - lastTime)
            > std::chrono::milliseconds(16))
        {
            cv::imshow("Decoded frame", decoder.curFrame.getRawFrame().getRGBMat());
            if (cv::waitKey(10) == 27)
                break;
            lastTime = curTime;
        }
    }

    decoder.stop();
    socket.closeSocket();
}
