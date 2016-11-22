#include <string>
#include <map>

// For the moment, users can read their session data.
// Don't put anything super secret in there.

// Top level APIs.
std::map<std::wstring, std::wstring> session_data_for_cookies(const std::map<std::wstring, std::wstring> &cookies);
std::pair<std::string, std::string> header_for_session_data(const std::map<std::wstring, std::wstring> &session_data);

// These ones can be used if you want to reuse the MAC mechanism. Value returned is already base64 encoded.
bool mac_valid_for_string(const std::string &mac, const byte salt[32], const std::string &cookie);
std::string salted_mac_for_string(const byte salt[32], const std::string &cookie);
