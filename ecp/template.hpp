#ifndef HANE_TEMPLATE_HPP
#define HANE_TEMPLATE_HPP
#include <string>

class Template {
protected:
    virtual std::string render() = 0;
};

#endif
