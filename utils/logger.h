#ifndef RACKCPP_LOGGER_H
#define RACKCPP_LOGGER_H

#include <string>
#include <uv.h>

class Logger
{
private:
    std::string infoPath, errorPath;
    uv_loop_t* logLoop;
    uv_fs_t infoLogReq, errorLogReq;
    uv_buf_t infoLogBuf, errorLogBuf;
    uv_file infoLogFd = -1;
    uv_file errorLogFd = -1;

    Logger();
    void openLogFile(int& fd, const std::string& path);
    static void writeCallback(uv_fs_t* req);
public:
    static Logger& getInstance();
    void setInfoPath(const std::string& path);
    void setErrorPath(const std::string& path);
    void info(const std::string& msg);
    void error(const std::string& msg);
};

#endif
