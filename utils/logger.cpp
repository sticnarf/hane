#include <iostream>
#include <cstring>
#include "logger.hpp"

Logger::Logger()
{
    enabled = false;
}

Logger& Logger::getInstance()
{
    static Logger logger;
    return logger;
}

void Logger::initLog()
{
    if (enabled)
    {
        spdlog::drop("logger");
        logger.reset();
        enabled = false;
    }

    try
    {
        logger = spdlog::basic_logger_mt("logger", logPath);
        logger->flush_on(spdlog::level::err);

        enabled = true;
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    }
}

void Logger::setLogPath(const std::string& path)
{
    logPath = path;
    initLog();
}

Logger::~Logger()
{
    spdlog::drop_all();
}