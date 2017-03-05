#ifndef INCLUDE_GUARD_FOR_CWAJH_HH
#define INCLUDE_GUARD_FOR_CWAJH_HH
#include <boost/locale/encoding_utf.hpp>
#include <sstream>
#include <vector>

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

// thanks stackoverflow for these one-liners.
// gcc SHOULD support the codecvt way but. well.
inline std::wstring s2w(const std::string& str)
{
	return boost::locale::conv::utf_to_utf<wchar_t>(str.c_str(), str.c_str() + str.size());
}
inline std::string w2s(const std::wstring& str)
{
	return boost::locale::conv::utf_to_utf<char>(str.c_str(), str.c_str() + str.size());
}



inline std::wstring strip_query(const std::wstring &str) {
	std::wstring::size_type qpos = str.find(L'?');
	std::wstring::size_type apos = str.find(L'#');
	std::wstring::size_type first_pos = MIN(qpos,apos);
	if(first_pos == std::wstring::npos){
		return str;
	}
	return std::wstring(str.c_str(), first_pos);
}

inline std::wstring query_string_from_url(const std::wstring &url) {
	std::wstring url_without_fragment = url;
	std::wstring::size_type apos = url.find(L'#');
	if(apos != std::wstring::npos) {
		url_without_fragment = std::wstring(url.c_str(),apos);
	}
	std::wstring::size_type qpos = url_without_fragment.find(L'?');
	if(qpos == std::wstring::npos){
		return L"";
	}
	return std::wstring(url_without_fragment.c_str()+qpos+1);
}

inline std::wstring fragment_from_url(const std::wstring &url) {
	std::wstring::size_type apos = url.find(L'#');
	if(apos == std::wstring::npos) {
		return L"";
	}
	return std::wstring(url.c_str()+apos);
}



// Split a string<charT>.
// Credit: http://stackoverflow.com/a/236803
template<class charT>
static void split(const std::basic_string<charT> &s, charT delim, std::vector< std::basic_string<charT> > &elems) {
    std::basic_stringstream<charT> ss;
    ss.str(s);
    std::basic_string<charT> item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}
template<class charT>
static std::vector< std::basic_string<charT> > split(const std::basic_string<charT> &s, charT delim) {
    std::vector< std::basic_string<charT> > elems;
    split(s, delim, elems);
    return elems;
}


#endif /*INCLUDE_GUARD_FOR_CWAJH_HH*/
