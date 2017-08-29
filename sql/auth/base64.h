#ifndef __base64_HHH
#define __base64_HHH
#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);
#endif
