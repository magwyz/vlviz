#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <sys/types.h>


static std::string uint8Encode(uint8_t d)
{
    return std::string(reinterpret_cast<char *>(&d), 1);
}

static uint8_t uint8Decode(std::string d, ssize_t pos)
{
    return *reinterpret_cast<const char *>(d.c_str() + pos);
}


#endif // HELPERS_H
