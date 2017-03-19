#include "base64.hh"

#include <crypto++/pwdbased.h>
#include <crypto++/sha.h>
#include <crypto++/sha3.h>

#include "keys.hh"
#include "urlencode.hh"

std::string hash_for_password(const std::string &password, int64_t salt) {
	CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> password_hasher;
	std::string salt_string = std::to_string(salt) + std::string((char*)PASSWORD_PEPPER, 32);

	unsigned char hash[32];
	password_hasher.DeriveKey(
		(byte*)&hash[0], 32,
		0, //PBKDF2 doesn't use a "purpose" identifier
		(byte*)password.c_str(), password.size(),
		(byte*)salt_string.c_str(), salt_string.size(),
		// 4x recommended SHA2 iteration count. Too many ppl trying to break SHA2 these days.
		77777
	);
	return base64_encode(hash,32);
}

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
