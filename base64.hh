#ifndef INCLUDE_GUARD_FOR_BASE64_HH
#define INCLUDE_GUARD_FOR_BASE64_HH

#include <string>

// Websafe base 64 (uses -_. instead of +/=)
std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif /* INCLUDE_GUARD_FOR_BASE64_HH */
