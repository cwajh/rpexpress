#include "urlencode.hh"

#include <cstdlib>
#include <cctype>
#include "cwajh.hh"
#include <iomanip>
#include <string>

// http://stackoverflow.com/a/4823686
std::string urldecode(const std::string &src) {
    std::string ret;
    char ch;
    int i, ii;
    for (i=0; i<src.length(); i++) {
        if (int(src[i])==37) {
            sscanf(src.substr(i+1,2).c_str(), "%x", &ii);
            ch=static_cast<char>(ii);
            ret+=ch;
            i=i+2;
        } else {
            ret+=src[i];
        }
    }
    return (ret);
}

// http://stackoverflow.com/a/17708801
std::string urlencode(const std::string &value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        std::string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char) c);
        escaped << std::nouppercase;
    }

    return escaped.str();
}

std::map<std::string, std::string> map_for_query_string(const std::string &query_string) {
	std::map<std::string, std::string> kv;
	for(auto&& kv_string : split(query_string, '&')) {
		size_t kv_boundary = kv_string.find_first_of('=');
		if(kv_boundary == std::string::npos) {
			// TODO(cwajh): log an error?
			continue;
		}
		kv[urldecode(kv_string.substr(0,kv_boundary))] =
			urldecode(kv_string.substr(kv_boundary+1));
	}
	return kv;
}

std::string query_string_for_map(const std::map<std::string, std::string> &key_values) {
	std::string query_string;
	for(auto&& kv_pair : key_values) {
		query_string.append(urlencode(kv_pair.first));
		query_string.append("=");
		query_string.append(urlencode(kv_pair.second));
		query_string.append("&");
	}
	if(query_string.length()) {
		//trailing &
		query_string.resize(query_string.length() - 1);
	}
	return query_string;
}

