#include <boost/locale/encoding_utf.hpp>

static std::wstring s2w(const std::string& str)
{
	// thanks stackoverflow for this one liner.
	return boost::locale::conv::utf_to_utf<wchar_t>(str.c_str(), str.c_str() + str.size());
}

