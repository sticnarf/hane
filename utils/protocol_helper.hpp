#ifndef RACKCPP_PROTOCOL_HELPER_HPP
#define RACKCPP_PROTOCOL_HELPER_HPP

#include <cstdlib>
#include "constants.hpp"

HttpMethod toHttpMethod(const char *str, size_t len);

const char *stringify(HttpMethod method);

HttpVersion toHttpVersion(const char *str, size_t len);

const char *stringify(HttpVersion version);

#endif
