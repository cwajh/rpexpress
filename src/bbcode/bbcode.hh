#ifndef INCLUDE_GUARD_FOR_BBCODE_HH
#define INCLUDE_GUARD_FOR_BBCODE_HH

#include <string>

#include "parse_trace.hh"

namespace bbcode {
	struct block {
		std::string code;
		std::string html() const;
		std::string whtml() const;
		block();
		block(const std::string &code);
	};
}

#endif /* INCLUDE_GUARD_FOR_BBCODE_HH */
