#ifndef INCLUDE_GUARD_FOR_SESSION_HH
#define INCLUDE_GUARD_FOR_SESSION_HH
#include <cstdint>
#include <string>
#include <map>
#include <utility>
#include <vector>

// For the moment, users can read their session data.
// Don't put anything super secret in there.

// Top level APIs.
std::map<std::string, std::string> session_data_for_cookies(const std::map<std::wstring, std::wstring> &cookies);
std::vector<std::pair<std::wstring, std::wstring> > headers_for_session_data(const std::map<std::string, std::string> &session_data);

void reset_session_identifier(std::map<std::string, std::string> &session_data);
std::string session_identifier(std::map<std::string, std::string> &session_data);

#endif /* INCLUDE_GUARD_FOR_SESSION_HH */
