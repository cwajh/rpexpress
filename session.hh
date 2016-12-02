#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <utility>

// For the moment, users can read their session data.
// Don't put anything super secret in there.

// Top level APIs.
std::map<std::string, std::string> session_data_for_cookies(const std::map<std::wstring, std::wstring> &cookies);
std::vector<std::pair<std::wstring, std::wstring> > headers_for_session_data(const std::map<std::string, std::string> &session_data);

// These ones can be used if you want to reuse the MAC mechanism. Value returned is already base64 encoded.
bool mac_valid_for_string(const std::string &mac, const uint8_t salt[32], const std::string &cookie);
std::string salted_mac_for_string(const uint8_t salt[32], const std::string &cookie);
