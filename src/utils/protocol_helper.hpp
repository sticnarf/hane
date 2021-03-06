#ifndef HANE_PROTOCOL_HELPER_HPP
#define HANE_PROTOCOL_HELPER_HPP

#include <cstdlib>
#include "../constants.hpp"

HttpMethod toHttpMethod(const char *str, size_t len);

const char *stringify(HttpMethod method);

HttpVersion toHttpVersion(const char *str, size_t len);

const char *stringify(HttpVersion version);

const char *toReasonPhrase(StatusCode code);

#endif
