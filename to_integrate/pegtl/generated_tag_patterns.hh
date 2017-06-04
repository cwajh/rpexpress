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

// TODO(cwajh): These bracket structs could be generalized into a per-tag "identity" struct/class with the various strings and patterns. This would probably simplify action generation.

// Diagnostic info for static patterns.
//  Why "constexpr const"?
//  https://stackoverflow.com/questions/8016780/undefined-reference-to-static-constexpr-char
//  https://stackoverflow.com/questions/5287566/constexpr-and-deprecated-conversion-warning
#define SIMPLE_BRACKET(tag_name) struct bracket_tag_ ##tag_name : TAOCPP_PEGTL_ISTRING("[" #tag_name "]") { \
	static constexpr const char *name = #tag_name; \
	static constexpr preview_type tag_type = k_simple; \
};
#define ATTR_BRACKET(tag_name) struct bracket_tag_ ##tag_name : TAOCPP_PEGTL_ISTRING("[" #tag_name "=") { \
	static constexpr const char *name = #tag_name; \
	static constexpr preview_type tag_type = k_attr; \
};
#define W_SIMPLE_BRACKET(tag_name) struct bracket_wtag_ ##tag_name : TAOCPP_PEGTL_ISTRING("[" #tag_name "]") { \
	static constexpr const char *name = #tag_name; \
	static constexpr preview_type tag_type = k_wrong_simple; \
};
#define W_ATTR_BRACKET(tag_name) struct bracket_wtag_ ##tag_name : TAOCPP_PEGTL_ISTRING("[" #tag_name "=") { \
	static constexpr const char *name = #tag_name; \
	static constexpr preview_type tag_type = k_wrong_attr; \
};
#define CLOSE_BRACKET(tag_name) struct bracket_xtag_ ##tag_name : TAOCPP_PEGTL_ISTRING("[/" #tag_name "]") { \
	static constexpr const char *name = #tag_name; \
	static constexpr preview_type tag_type = k_close; \
};
#define W_CLOSE_BRACKET(tag_name) struct bracket_xtag_ ##tag_name : TAOCPP_PEGTL_ISTRING("[/" #tag_name "]") { \
	static constexpr const char *name = #tag_name; \
	static constexpr preview_type tag_type = k_wrong_close; \
};

#define SIMPLE_TAG(name)                                SIMPLE_BRACKET(name) W_ATTR_BRACKET(name) CLOSE_BRACKET(name)
#define SIMPLE_TAG_MAP(name, out_tag)                   SIMPLE_BRACKET(name) W_ATTR_BRACKET(name) CLOSE_BRACKET(name)
#define SIMPLE_TAG_ATTR(name, out_tag, out_attributes)  SIMPLE_BRACKET(name) W_ATTR_BRACKET(name) CLOSE_BRACKET(name)
#define SELF_CLOSING_TAG(name, out_html)                SIMPLE_BRACKET(name) W_ATTR_BRACKET(name) W_CLOSE_BRACKET(name)
#define GREEDY_TAG_UNCHECKED(name, ...)                 SIMPLE_BRACKET(name) W_ATTR_BRACKET(name) CLOSE_BRACKET(name)
#define GREEDY_TAG(name, ...)                           SIMPLE_BRACKET(name) W_ATTR_BRACKET(name) CLOSE_BRACKET(name)
#define ATTR_TAG_UNCHECKED(name, ...)                   W_SIMPLE_BRACKET(name) ATTR_BRACKET(name) CLOSE_BRACKET(name)
#define ATTR_TAG(name, ...)                             W_SIMPLE_BRACKET(name) ATTR_BRACKET(name) CLOSE_BRACKET(name)
#include "tag_definitions.hh"
#undef SIMPLE_TAG
#undef SIMPLE_TAG_MAP
#undef SIMPLE_TAG_ATTR
#undef SELF_CLOSING_TAG
#undef GREEDY_TAG_UNCHECKED
#undef GREEDY_TAG
#undef ATTR_TAG_UNCHECKED
#undef ATTR_TAG
#undef SIMPLE_BRACKET
#undef ATTR_BRACKET
#undef W_SIMPLE_BRACKET
#undef W_ATTR_BRACKET
#undef CLOSE_BRACKET
#undef W_CLOSE_BRACKET


#define START_OF_OPEN_TAG(name) struct bracket_tag_ ##name , struct bracket_wtag_ ##name ,
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
#define SIMPLE_TAG(name)                                struct bracket_xtag_ ##name ,
#define SIMPLE_TAG_MAP(name, out_tag)                   struct bracket_xtag_ ##name ,
#define SIMPLE_TAG_ATTR(name, out_tag, out_attributes)  struct bracket_xtag_ ##name ,
#define SELF_CLOSING_TAG(name, out_html)                /* No such thing as a close tag for a self closing tag. */
#define GREEDY_TAG_UNCHECKED(name, ...)                 struct bracket_xtag_ ##name ,
#define GREEDY_TAG(name, ...)                           struct bracket_xtag_ ##name ,
#define ATTR_TAG_UNCHECKED(name, ...)                   struct bracket_xtag_ ##name ,
#define ATTR_TAG(name, ...)                             struct bracket_xtag_ ##name ,
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
