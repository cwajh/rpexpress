#ifndef INCLUDE_GUARD_FOR_BBCODE_HH
#define INCLUDE_GUARD_FOR_BBCODE_HH

#include <string>

#include "parse_trace.hh"

namespace bbcode {
	void check_validity(const std::string &document);
	void print_html_for(const std::string &document);
	struct block {
		std::string code;
		std::string html() const;
		block();
		block(const std::string &code);
	};
}

#endif /* INCLUDE_GUARD_FOR_BBCODE_HH */
