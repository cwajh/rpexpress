#include "session.hh"

#include "cwajh.hh"
#include "base64.hh"
#include "keys.hh"
#include "urlencode.hh"
#include <crypto++/sha3.h>
#include <chrono>

using namespace std::chrono_literals;
static const std::wstring SESSION_MAC_COOKIE(L"SESS_MAC");
static const std::wstring SESSION_COOKIE(L"SESS_DATA");
static const std::chrono::hours COOKIE_LIFESPAN(2190h);
static const std::string SESSION_ID("__SESS_ID");

bool mac_valid_for_string(const std::string &mac, const byte salt[32], const std::string &cookie) {
	const std::string bin_mac = base64_decode(mac);
	if(bin_mac.length() != 32) {
		// Invalid. Should be a SHA3-256 hash.
		return false;
	}

	// Chose SHA3 b/c it isn't subject to length-extension attacks (makes MAC schedule easier).
	CryptoPP::SHA3_256 hash;
	hash.Update(salt, 32);
	hash.Update((const unsigned char*)cookie.c_str(), cookie.length());
	if(!hash.Verify((const unsigned char*)bin_mac.c_str())) {
		// Invalid. Cookie seems tampered with.
		return false;
	}

	return true;
}

std::string salted_mac_for_string(const byte salt[32], const std::string &cookie) {
	unsigned char mac[32];
	CryptoPP::SHA3_256 hash;
	hash.Update(salt, 32);
	hash.Update((const unsigned char*)cookie.c_str(), cookie.length());
	hash.Final(&mac[0]);
	return base64_encode(mac,32);
}



std::map<std::string, std::string> session_data_for_cookies(const std::map<std::wstring, std::wstring> &cookies){
	std::string received_mac;
	auto cookie_field = cookies.find(SESSION_MAC_COOKIE);
	if (cookie_field != cookies.end()) {
		received_mac = w2s(cookie_field->second);
	}

	std::string cookie;
	cookie_field = cookies.find(SESSION_COOKIE);
	if (cookie_field != cookies.end()) {
		cookie = w2s(cookie_field->second);
	}

	if(!mac_valid_for_string(received_mac, COOKIE_SALT, cookie)) {
		std::map<std::string, std::string> nothing;
		return nothing;
	}

	return map_for_query_string(cookie);
}

std::vector<std::pair<std::wstring, std::wstring>> headers_for_session_data(const std::map<std::string, std::string> &session_data) {
	const std::string session_cookie = query_string_for_map(session_data);

	const std::string b64_mac = salted_mac_for_string(COOKIE_SALT, session_cookie);

	auto expiration_point = std::chrono::system_clock::now() + COOKIE_LIFESPAN;
	std::time_t expiration_time = std::chrono::system_clock::to_time_t(expiration_point);
	// It is an Injustice that the C++ standard doesn't guarantee this API to exist.
	// A small injustice, but still.
	// Anyway, POSIX requires gmtime_r so it should be fine?
	std::tm expiration_tm;
	char expiration_buffer[32] = {'\0'};
	gmtime_r(&expiration_time, &expiration_tm);
	strftime(expiration_buffer, 32, "%a, %d %b %Y %H:%M:%S GMT", &expiration_tm);
	std::string expiration_string(expiration_buffer);

	std::vector<std::pair<std::wstring, std::wstring>> headers;
	headers.push_back({L"Set-Cookie",
		SESSION_MAC_COOKIE + L"=" + s2w(b64_mac)
		+ L"; Expires=" + s2w(expiration_string)
		+ L"; SameSite=Lax; HttpOnly"});
	headers.push_back({L"Set-Cookie",
		SESSION_COOKIE + L"=" + s2w(session_cookie)
		+ L"; Expires=" + s2w(expiration_string)
		+ L"; SameSite=Lax; HttpOnly"});
	return headers;
}

void reset_session_identifier(std::map<std::string, std::string> &session_data) {
	// TODO(cwajh): use some random number thing instead of this.
	std::time_t sess_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	session_data[SESSION_ID] = std::to_string(sess_time);
}

std::string session_identifier(std::map<std::string, std::string> &session_data) {
	if(session_data.find(SESSION_ID) == session_data.end()) {
		reset_session_identifier(session_data);
	}
	return session_data[SESSION_ID];
}
