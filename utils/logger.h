#ifndef RACKCPP_LOGGER_H
#define RACKCPP_LOGGER_H
#include <string>
#include <uv.h>

class Logger {
private:
    uv_loop_t logLoop;
};

#endif
