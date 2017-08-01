#ifndef HANE_HELLO_HTML_HPP
#define HANE_HELLO_HTML_HPP

#include <ecp/template.hpp>

class HelloHtml : public Template {
protected:
    std::string render() override;
};

#endif
