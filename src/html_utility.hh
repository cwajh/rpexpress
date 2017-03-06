#ifndef INCLUDE_GUARD_FOR_HTML_UTILITY_HH
#define INCLUDE_GUARD_FOR_HTML_UTILITY_HH

#include "request_context.hh"

class html_utility {
	struct request_context context;
public:
	html_utility(struct request_context context);
	std::wstring form_tokens(const std::wstring &action_url);
};

#endif /* INCLUDE_GUARD_FOR_HTML_UTILITY_HH */
