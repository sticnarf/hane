#ifndef RACKCPP_FORM_DATA_H
#define RACKCPP_FORM_DATA_H
#include <string>
#include "header.h"

class FormData {
private:
    std::string data;
    FieldParameters parameters;
public:
    FormData() = default;
    FormData(const std::string& data);
    const std::string& getData() const;
    const FieldParameters& getParameters() const;

    friend class MultipartFormParser;
};

#endif
