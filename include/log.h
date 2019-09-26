#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>

static void msg(std::string s)
{
    std::cout << s << std::endl;
}


static void msg(const char *s)
{
    msg(std::string(s));
}


#endif // LOG_H
