
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

    while (true)
    {
        decoder.processData();

        cv::imshow("Decoded frame", decoder.curFrame.getRawFrame().getRGBMat());

        if (cv::waitKey(10) == 27)
            break;
    }

    socket.closeSocket();
}
