#ifndef INCLUDE_GUARD_FOR_CRYPTO_HH
#define INCLUDE_GUARD_FOR_CRYPTO_HH

#include <cstdint>
#include <string>

bool mac_valid_for_string(const std::string &mac, const uint8_t salt[32], const std::string &cookie);

std::string salted_mac_for_string(const uint8_t salt[32], const std::string &cookie);

std::string hash_for_password(const std::string &password, int64_t salt);

#endif /* INCLUDE_GUARD_FOR_CRYPTO_HH */
