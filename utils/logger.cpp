#include <iostream>
#include <cstring>
#include "logger.h"

Logger::Logger()
{
    logLoop = uv_default_loop();
}

Logger& Logger::getInstance()
{
    static Logger logger;
    return logger;
}

void Logger::setInfoPath(const std::string& path)
{
    infoPath = path;
    openLogFile(infoLogFd, path);
}

void Logger::setErrorPath(const std::string& path)
{
    errorPath = path;
    openLogFile(errorLogFd, path);
}

void Logger::openLogFile(int& fd, const std::string& path)
{
    uv_fs_t req;
    int res = uv_fs_open(logLoop, &req, path.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0744, NULL);

    if (res < 0)
    {
        std::cerr << "Error on opening " << path << std::endl;
        std::cerr << uv_strerror(res) << std::endl;
    }
    else
    {
        fd = res;
    }
}

void Logger::info(const std::string& msg)
{
    if (infoLogFd < 0)
        std::cerr << "Info logging not available" << std::endl;
    size_t len = msg.size();
    char* base = new char[len];
    memcpy(base, msg.c_str(), len);
    infoLogBuf = uv_buf_init(base, len);
    infoLogReq.data = base;
    uv_fs_write(logLoop, &infoLogReq, infoLogFd, &infoLogBuf, 1, 0, Logger::writeCallback);
}

void Logger::writeCallback(uv_fs_t* req)
{
    int res = req->result;
    if (res < 0)
    {
        std::cerr << uv_strerror(res) << std::endl;
    }
    delete[] (char*) req->data;
}

void Logger::error(const std::string& msg)
{
    if (infoLogFd < 0)
        std::cerr << "Error logging not available" << std::endl;
    size_t len = msg.size();
    char* base = new char[len];
    memcpy(base, msg.c_str(), len);
    errorLogBuf = uv_buf_init(base, len);
    errorLogReq.data = base;
    uv_fs_write(logLoop, &errorLogReq, errorLogFd, &errorLogBuf, 1, 0, Logger::writeCallback);
}
