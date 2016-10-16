#ifndef INCLUDE_GUARD_FOR_CWAJH_HH
#define INCLUDE_GUARD_FOR_CWAJH_HH
#include <boost/locale/encoding_utf.hpp>

static std::wstring s2w(const std::string& str)
{
	// thanks stackoverflow for this one liner.
	return boost::locale::conv::utf_to_utf<wchar_t>(str.c_str(), str.c_str() + str.size());
}

#endif /*INCLUDE_GUARD_FOR_CWAJH_HH*/
