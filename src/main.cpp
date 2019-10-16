#include "log.h"
#include "sender.h"
#include "receiver.h"


void usage()
{
    msg("Usage: vlviz -s/-r");
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        usage();
        return 1;
    }

    std::string modeStr(argv[1]);
    enum {
        SENDER,
        RECEIVER
    } mode;

    if (modeStr == std::string("-s"))
        mode = SENDER;
    else if (modeStr == std::string("-r"))
        mode = RECEIVER;
    else
    {
        usage();
        return 1;
    }

    switch (mode) {
    case SENDER:
    {
        Sender s;
        s.start();
        s.join();
        break;
    }
    case RECEIVER:
    {
        Receiver r;
        r.start();
        r.join();
        break;
    }
    default:
        break;
    }

    return 0;
}

