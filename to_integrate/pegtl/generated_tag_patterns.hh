#ifndef INCLUDE_GUARD_FOR_BBCODE_GENERATED_TAGS
#define INCLUDE_GUARD_FOR_BBCODE_GENERATED_TAGS

#include <tao/pegtl.hpp>

#include "tag_types.hh"

namespace bbcode {
	// These will be defined later.
	template<typename tag_name>	struct enclosing_tag;
	template<typename tag_name>	struct greedy_tag;
	template<typename tag_name>	struct attr_tag;
	
	namespace generated {
		using namespace tao::pegtl;
		using namespace tao::pegtl::ascii;

#define START_OF_OPEN_TAG(name) TAOCPP_PEGTL_ISTRING("[" #name "]"), TAOCPP_PEGTL_ISTRING("[" #name "="),
#define SIMPLE_TAG(name)                                START_OF_OPEN_TAG(name)
#define SIMPLE_TAG_MAP(name, out_tag)                   START_OF_OPEN_TAG(name)
#define SIMPLE_TAG_ATTR(name, out_tag, out_attributes)  START_OF_OPEN_TAG(name)
#define SELF_CLOSING_TAG(name, out_html)                START_OF_OPEN_TAG(name)
#define GREEDY_TAG_UNCHECKED(name, ...)                 START_OF_OPEN_TAG(name)
#define GREEDY_TAG(name, ...)                           START_OF_OPEN_TAG(name)
#define ATTR_TAG_UNCHECKED(name, ...)                   START_OF_OPEN_TAG(name)
#define ATTR_TAG(name, ...)                             START_OF_OPEN_TAG(name)	
		struct start_of_any_open_tag : sor<
#include "tag_definitions.hh"
		failure> {};
#undef SIMPLE_TAG
#undef SIMPLE_TAG_MAP
#undef SIMPLE_TAG_ATTR
#undef SELF_CLOSING_TAG
#undef GREEDY_TAG_UNCHECKED
#undef GREEDY_TAG
#undef ATTR_TAG_UNCHECKED
#undef ATTR_TAG
#undef START_OF_OPEN_TAG



#define CLOSE_TAG(name) TAOCPP_PEGTL_ISTRING("[/" #name "]"),
#define SIMPLE_TAG(name)                                CLOSE_TAG(name)
#define SIMPLE_TAG_MAP(name, out_tag)                   CLOSE_TAG(name)
#define SIMPLE_TAG_ATTR(name, out_tag, out_attributes)  CLOSE_TAG(name)
#define SELF_CLOSING_TAG(name, out_html)                CLOSE_TAG(name)
#define GREEDY_TAG_UNCHECKED(name, ...)                 CLOSE_TAG(name)
#define GREEDY_TAG(name, ...)                           CLOSE_TAG(name)
#define ATTR_TAG_UNCHECKED(name, ...)                   CLOSE_TAG(name)
#define ATTR_TAG(name, ...)                             CLOSE_TAG(name)
		struct any_close_tag : sor<
#include "tag_definitions.hh"
		failure> {};
#undef SIMPLE_TAG
#undef SIMPLE_TAG_MAP
#undef SIMPLE_TAG_ATTR
#undef SELF_CLOSING_TAG
#undef GREEDY_TAG_UNCHECKED
#undef GREEDY_TAG
#undef ATTR_TAG_UNCHECKED
#undef ATTR_TAG
#undef CLOSE_TAG



#define SIMPLE_TAG(name)                                struct t_ ## name : enclosing_tag<TAOCPP_PEGTL_ISTRING( #name )> {};
#define SIMPLE_TAG_MAP(name, out_tag)                   struct t_ ## name : enclosing_tag<TAOCPP_PEGTL_ISTRING( #name )> {};
#define SIMPLE_TAG_ATTR(name, out_tag, out_attributes)  struct t_ ## name : enclosing_tag<TAOCPP_PEGTL_ISTRING( #name )> {};
#define SELF_CLOSING_TAG(name, out_html)                struct tt_ ## name : TAOCPP_PEGTL_ISTRING("[" #name "]") {};
#define GREEDY_TAG_UNCHECKED(name, ...)                 struct gt_ ## name : greedy_tag<TAOCPP_PEGTL_ISTRING( #name )> {};
#define GREEDY_TAG(name, ...)                           struct gt_ ## name : greedy_tag<TAOCPP_PEGTL_ISTRING( #name )> {};
#define ATTR_TAG_UNCHECKED(name, ...)                   struct at_ ## name : attr_tag<TAOCPP_PEGTL_ISTRING( #name )> {};
#define ATTR_TAG(name, ...)                             struct at_ ## name : attr_tag<TAOCPP_PEGTL_ISTRING( #name )> {};
#include "tag_definitions.hh"
#undef SIMPLE_TAG
#undef SIMPLE_TAG_MAP
#undef SIMPLE_TAG_ATTR
#undef SELF_CLOSING_TAG
#undef GREEDY_TAG_UNCHECKED
#undef GREEDY_TAG
#undef ATTR_TAG_UNCHECKED
#undef ATTR_TAG


#define SIMPLE_TAG(name)                                t_ ## name ,
#define SIMPLE_TAG_MAP(name, out_tag)                   t_ ## name ,
#define SIMPLE_TAG_ATTR(name, out_tag, out_attributes)  t_ ## name ,
#define SELF_CLOSING_TAG(name, out_html)                tt_ ## name ,
#define GREEDY_TAG_UNCHECKED(name, ...)                 gt_ ## name ,
#define GREEDY_TAG(name, ...)                           gt_ ## name ,
#define ATTR_TAG_UNCHECKED(name, ...)                   at_ ## name ,
#define ATTR_TAG(name, ...)                             at_ ## name ,
		struct any_complete_tag : sor<
#include "tag_definitions.hh"
		failure> {};
#undef SIMPLE_TAG
#undef SIMPLE_TAG_MAP
#undef SIMPLE_TAG_ATTR
#undef SELF_CLOSING_TAG
#undef GREEDY_TAG_UNCHECKED
#undef GREEDY_TAG
#undef ATTR_TAG_UNCHECKED
#undef ATTR_TAG
	}
}

#endif /* INCLUDE_GUARD_FOR_BBCODE_GENERATED_TAGS */
