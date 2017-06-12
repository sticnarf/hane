#include "string.h"

std::string toLowercase(const std::string& str)
{
    std::string result;
    for (char c : str)
    {
        result.push_back((char) tolower(c));
    }
    return result;
}