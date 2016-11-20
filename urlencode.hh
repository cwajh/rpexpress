#ifndef INCLUDE_GUARD_FOR_URLENCODE_HH
#define INCLUDE_GUARD_FOR_URLENCODE_HH
#include <string>
#include <map>

// These do not convert '+' to ' '
std::string urldecode(const std::string &src);
std::string urlencode(const std::string &src);

std::map<std::string, std::string> map_for_query_string(const std::string &query_string);
std::string query_string_for_map(std::map<std::string, std::string> key_values);

#endif /* INCLUDE_GUARD_FOR_URLENCODE_HH */
