#ifndef INCLUDE_GUARD_FOR_PATH_INFO_HH
#define INCLUDE_GUARD_FOR_PATH_INFO_HH
#include <regex>
#include <string>

static std::wstring muse_for_path(const std::wstring &path) {
	thread_local const std::wregex re_musepath(L"/muse/([^/]+)/.*");
	std::wsmatch match;
	std::regex_match(path, match, re_musepath);
	return match[1];
}
#endif /* INCLUDE_GUARD_FOR_PATH_INFO_HH */
