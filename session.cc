#include "session.hh"

#include "cwajh.hh"
#include "keys.hh"
#include "urlencode.hh"
#include <crypto++/sha3.h>
#include <chrono>

using namespace std::chrono_literals;
static const std::wstring SESSION_MAC_COOKIE(L"SESS_MAC");
static const std::wstring SESSION_COOKIE(L"SESS_DATA");
static const std::chrono::duration COOKIE_LIFESPAN(2190h));

bool mac_valid_for_string(const std::string &mac, const byte salt[32], const std::string &cookie) {
	if(mac.length() != 32) {
		// Invalid. Should be a SHA3-256 hash.
		return false;
	}

	// Chose SHA3 b/c it isn't subject to length-extension attacks (makes MAC schedule easier).
	CryptoPP::SHA3_256 hash;
	hash.Update(salt, 32);
	hash.Update(cookie.c_str(), cookie.length());
	if(!hash.Verify(mac.c_str())) {
		// Invalid. Cookie seems tampered with.
		return false;
	}

	return true;
}

std::string salted_mac_for_string(const byte salt[32], const std::string &cookie) {
	char mac[32];
	CryptoPP::SHA3_256 hash;
	hash.Update(COOKIE_SALT, sizeof(COOKIE_SALT));
	hash.Update(session_cookie.c_str(), session_cookie.length());
	hash.Final(&mac[0]);
	return base64_encode(mac,32);
}



std::map<std::wstring, std::wstring> session_data_for_cookies(const std::map<std::wstring, std::wstring> &cookies){
	std::map<std::wstring, std::wstring> nothing;

	std::string received_mac = base64_decode(w2s(cookies.get(SESSION_MAC_COOKIE)));
	std::string cookie = w2s(cookies.get(SESSION_COOKIE));

	if(received_mac.length() != 32) {
		// Invalid. Should be a SHA3-256 hash.
		return nothing;
	}

	// Chose SHA3 b/c it isn't subject to length-extension attacks (makes MAC schedule easier).
	CryptoPP::SHA3_256 hash;
	hash.Update(COOKIE_SALT, sizeof(COOKIE_SALT));
	hash.Update(cookie.c_str(), cookie.length());
	if(!hash.Verify(received_mac.c_str())) {
		// Invalid. Cookie seems tampered with.
		return nothing;
	}

	return map_for_query_string(cookie);
}

std::vector<std::pair<std::string, std::string>> headers_for_session_data(const std::map<std::wstring, std::wstring> &session_data) {
	std::string session_cookie = query_string_for_map(session_data);

	char mac[32];
	CryptoPP::SHA3_256 hash;
	hash.Update(COOKIE_SALT, sizeof(COOKIE_SALT));
	hash.Update(session_cookie.c_str(), session_cookie.length());
	hash.Final(&mac[0]);
	std::string b64_mac = base64_encode(mac,32);

	auto expiration_point = std::chrono::system_clock::now() + COOKIE_LIFESPAN;
	std::time_t expiration_time = std::chrono::system_clock::to_time_t(expiration_point);
	// It is an Injustice that the C++ standard doesn't guarantee this API to exist.
	// A small injustice, but still.
	// Anyway, POSIX requires gmtime_r so it should be fine?
	std::tm expiration_tm;
	char expiration_buffer[32];
	gmtime_r(&expiration_time, &ugh);
	strftime(buffer, 32, "%a, %d %b %Y %H:%M:%S GMT", &ugh);

	// XXX(cwajh)
	std::vector<std::pair<std::string, std::string>> headers;
	headers.push_back({"Set-Cookie", ""})
}

