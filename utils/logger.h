#ifndef RACKCPP_LOGGER_H
#define RACKCPP_LOGGER_H

#include <string>
#include <uv.h>

class Logger
{
private:
    static Logger* logger = nullptr;
    uv_loop_t logLoop;

    Logger();

public:
    Logger* getInstance();
};

#endif
