#ifndef HANE_TEMPLATE_HPP
#define HANE_TEMPLATE_HPP

#include <string>
#include <spdlog/fmt/fmt.h>

class Template {
protected:
    virtual std::string render() = 0;
};

#endif
