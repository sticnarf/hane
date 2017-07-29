#ifndef RACKCPP_PROTOCOL_HELPER_H
#define RACKCPP_PROTOCOL_HELPER_H
#include <cstdlib>
#include "constants.hpp"

HttpMethod toHttpMethod(const char* str, size_t len);
const char* stringify(HttpMethod method);

HttpVersion toHttpVersion(const char* str, size_t len);
const char* stringify(HttpVersion version);

#endif
