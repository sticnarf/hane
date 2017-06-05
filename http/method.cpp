#include <map>
#include "method.h"

Method parseMethod(const std::string &stringMethod) {
    static std::map<std::string, Method> mapping = {
            {"GET",     Method::HTTP_GET},
            {"HEAD",    Method::HTTP_HEAD},
            {"POST",    Method::HTTP_POST},
            {"PUT",     Method::HTTP_PUT},
            {"DELETE",  Method::HTTP_DELETE},
            {"CONNECT", Method::HTTP_CONNECT},
            {"OPTIONS", Method::HTTP_OPTIONS},
            {"TRACE",   Method::HTTP_TRACE}
    };
    return mapping[stringMethod];
}