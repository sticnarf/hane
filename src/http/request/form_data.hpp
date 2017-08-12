#ifndef HANE_FORM_DATA_HPP
#define HANE_FORM_DATA_HPP

#include <string>
#include "../header/header.hpp"

class FormData {
private:
    std::string data;
    FieldParameters parameters;
public:
    FormData() = default;

    FormData(const std::string &data);

    const std::string &getData() const;

    const FieldParameters &getParameters() const;

    friend class MultipartFormParser;
};

#endif
