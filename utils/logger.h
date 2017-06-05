#ifndef RACKCPP_LOGGER_H
#define RACKCPP_LOGGER_H

#include <string>
#include <uv.h>

class Logger
{
private:
    static Logger *logger = nullptr;
    uv_loop_t logLoop;

    Logger()
    {
        uv_loop_init(&logLoop);
    }

public:
    Logger *getInstance()
    {
        if (logger == nullptr) {
            logger = new Logger();
        }
        return logger;
    }
};

#endif
