#ifndef INCLUDE_GUARD_FOR_BBCODE_HH
#define INCLUDE_GUARD_FOR_BBCODE_HH

/*
A slightly stricter BBCode variant, in that it's actually parsed instead of regex'd.

[blah]what[/blah]
or
[bleh] (self-closing)

blah can be

[foo]
or
[bar=baz]
or
[bar="quux1"]
or
[bar='quux2']

baz can't have "'] in it
quux1 and quux2 use psql-style quote escaping ("a""b" = a"b)

*/

#include <string>

#include "parse_trace.hh"

namespace bbcode {
	struct block {
		std::string code;
		std::string html() const;
		std::wstring whtml() const;
		block();
		block(const std::string &code);
	};
}

#endif /* INCLUDE_GUARD_FOR_BBCODE_HH */
