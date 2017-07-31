#ifndef RACKCPP_LOGGER_HPP
#define RACKCPP_LOGGER_HPP

#include <string>
#include <spdlog/spdlog.h>

class Logger {
private:
    std::string logPath;
    std::shared_ptr<spdlog::logger> logger;
    bool enabled;

    void initLog();

    Logger();

public:
    static Logger &getInstance();

    void setLogPath(const std::string &path);

    template<typename T>
    void info(const T &msg) {
        if (!enabled)
            return;

        logger->info(msg);
    }

    template<typename T>
    void error(const T &msg) {
        if (!enabled)
            return;

        logger->error(msg);
    }

    template<typename Arg1, typename... Args>
    void info(const char *fmt, const Arg1 &arg1, const Args &... args) {
        if (!enabled)
            return;

        logger->info(fmt, arg1, args...);
    }

    template<typename Arg1, typename... Args>
    void error(const char *fmt, const Arg1 &arg1, const Args &... args) {
        if (!enabled)
            return;

        logger->error(fmt, arg1, args...);
    }

    ~Logger();
};

#endif
