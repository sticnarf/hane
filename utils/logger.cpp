#include "logger.h"

Logger::Logger()
{
    uv_loop_init(&logLoop);
}

Logger* Logger::getInstance()
{

    if (logger==nullptr)
    {
        logger = new Logger();
    }
    return logger;
}
