#include "form_data.hpp"

const std::string& FormData::getData() const
{
    return data;
}

const FieldParameters& FormData::getParameters() const
{
    return parameters;
}

FormData::FormData(const std::string& data)
        :data(data) { }
