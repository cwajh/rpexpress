#ifndef INCLUDE_GUARD_FOR_CWAJH_HH
#define INCLUDE_GUARD_FOR_CWAJH_HH
#include <boost/locale/encoding_utf.hpp>

// thanks stackoverflow for these one-liners.
// gcc SHOULD support the codecvt way but. well.
static std::wstring s2w(const std::string& str)
{
	return boost::locale::conv::utf_to_utf<wchar_t>(str.c_str(), str.c_str() + str.size());
}
static std::string w2s(const std::wstring& str)
{
	return boost::locale::conv::utf_to_utf<char>(str.c_str(), str.c_str() + str.size());
}



static std::wstring strip_query(const std::wstring &str) {
	std::wstring::size_type qpos = str.find(L'?');
	if(qpos == std::wstring::npos){
		return str;
	}
	return std::wstring(str.c_str(), qpos);
}

#endif /*INCLUDE_GUARD_FOR_CWAJH_HH*/
