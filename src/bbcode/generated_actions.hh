#ifndef INCLUDE_GUARD_FOR_BBCODE_GENERATED_ACTIONS
#define INCLUDE_GUARD_FOR_BBCODE_GENERATED_ACTIONS

#include <tao/pegtl.hpp>

#include "action_types.hh"
#include "generated_tag_patterns.hh"

namespace bbcode {

	namespace generated_cstr {
		// To instantiate the generator templates, having literal ""s isn't good enough. They need to have a specific NAME.
		// https://stackoverflow.com/questions/28809728/some-const-char-are-unavailable-at-compile-time
		extern char const attr[] = "&attr;";
		extern char const body[] = "&body;";

#define SIMPLE_TAG(name) \
	extern char const name__ ## name  [] =  #name ; \
	extern char const open__ ## name  [] = "<" #name ">"; \
	extern char const close__ ## name  [] = "</" #name ">";

#define SIMPLE_TAG_MAP(name, out_tag) \
	extern char const name__ ## name  [] =  #name ; \
	extern char const open__ ## name  [] = "<" out_tag ">"; \
	extern char const close__ ## name  [] = "</" out_tag ">";
             
#define SIMPLE_TAG_ATTR(name, out_tag, out_attributes) \
	extern char const name__ ## name  [] =  #name ; \
	extern char const open__ ## name  [] = "<" out_tag " " out_attributes ">" ; \
	extern char const close__ ## name  [] = "</" out_tag ">";

#define SELF_CLOSING_TAG(name, out_html) \
	extern char const tag__ ## name  [] = out_html;

#define GREEDY_TAG_UNCHECKED(name, out_html) \
	extern char const body__ ## name  [] = out_html ;

#define GREEDY_TAG(name, regex_match, error, out_html) \
	extern char const body__ ## name  [] = out_html ; \
	extern char const regex__ ## name  [] = regex_match ; \
	extern char const error__ ## name  [] = error ;

#define ATTR_TAG_UNCHECKED(name, out_tag, out_attributes) \
	extern char const name__ ## name  [] =  #name ; \
	extern char const open__ ## name  [] = "<" out_tag " " out_attributes ">" ; \
	extern char const close__ ## name  [] = "</" out_tag ">";

#define ATTR_TAG(name, regex_match, error, out_tag, out_attributes) \
	extern char const name__ ## name  [] =  #name ; \
	extern char const open__ ## name  [] = "<" out_tag " " out_attributes ">" ; \
	extern char const close__ ## name  [] = "</" out_tag ">"; \
	extern char const regex__ ## name  [] = regex_match ; \
	extern char const error__ ## name  [] = error ;
                    
#include "tag_definitions.hh"
#undef SIMPLE_TAG
#undef SIMPLE_TAG_MAP
#undef SIMPLE_TAG_ATTR
#undef SELF_CLOSING_TAG
#undef GREEDY_TAG_UNCHECKED
#undef GREEDY_TAG
#undef ATTR_TAG_UNCHECKED
#undef ATTR_TAG

	}
	
	
	// Have to be out of the generated namespace so they live where the actions live.
#define SIMPLE_TAG(name) \
	template<> struct http_gen_actions<open_tag<TAOCPP_PEGTL_ISTRING( #name )>> : generate_text< generated_cstr::open__ ## name > {}; \
	template<> struct http_gen_actions<close_tag<TAOCPP_PEGTL_ISTRING( #name )>> : generate_text< generated_cstr::close__ ## name > {};\
	template<> struct trace_gen_actions<open_tag<TAOCPP_PEGTL_ISTRING( #name )>> : trace_push_tag< generated_cstr::name__ ## name > {}; \
	template<> struct trace_gen_actions<close_tag<TAOCPP_PEGTL_ISTRING( #name )>> : trace_pop_tag< generated_cstr::name__ ## name > {};

#define SIMPLE_TAG_MAP(name, out_tag) \
	template<> struct http_gen_actions<open_tag<TAOCPP_PEGTL_ISTRING( #name )>> : generate_text< generated_cstr::open__ ## name > {}; \
	template<> struct http_gen_actions<close_tag<TAOCPP_PEGTL_ISTRING( #name )>> : generate_text< generated_cstr::close__ ## name > {};\
	template<> struct trace_gen_actions<open_tag<TAOCPP_PEGTL_ISTRING( #name )>> : trace_push_tag< generated_cstr::name__ ## name > {}; \
	template<> struct trace_gen_actions<close_tag<TAOCPP_PEGTL_ISTRING( #name )>> : trace_pop_tag< generated_cstr::name__ ## name > {};
             
#define SIMPLE_TAG_ATTR(name, out_tag, out_attributes) \
	template<> struct http_gen_actions<open_tag<TAOCPP_PEGTL_ISTRING( #name )>> : generate_text< generated_cstr::open__ ## name > {}; \
	template<> struct http_gen_actions<close_tag<TAOCPP_PEGTL_ISTRING( #name )>> : generate_text< generated_cstr::close__ ## name > {};\
	template<> struct trace_gen_actions<open_tag<TAOCPP_PEGTL_ISTRING( #name )>> : trace_push_tag< generated_cstr::name__ ## name > {}; \
	template<> struct trace_gen_actions<close_tag<TAOCPP_PEGTL_ISTRING( #name )>> : trace_pop_tag< generated_cstr::name__ ## name > {};

#define SELF_CLOSING_TAG(name, out_html) \
	template<> struct http_gen_actions<generated::tt_ ## name> : generate_text< generated_cstr::tag__ ## name > {};

#define GREEDY_TAG_UNCHECKED(name, out_html) \
	template<> struct http_gen_actions<greedy_tag_contents<TAOCPP_PEGTL_ISTRING( #name )>> : generate_text_with_replacement< generated_cstr::body__ ## name, generated_cstr::body> {};

#define GREEDY_TAG(name, regex_match, error, out_html) \
	template<> struct http_gen_actions<greedy_tag_contents<TAOCPP_PEGTL_ISTRING( #name )>> : generate_text_with_replacement< generated_cstr::body__ ## name, generated_cstr::body, generated_cstr::regex__ ## name, generated_cstr::error__ ## name> {}; \
	template<> struct trace_gen_actions<greedy_tag_contents<TAOCPP_PEGTL_ISTRING( #name )>> : trace_check_argument< generated_cstr::regex__ ## name, generated_cstr::error__ ## name> {};


#define ATTR_TAG_UNCHECKED(name, out_tag, out_attributes) \
	template<> struct http_gen_actions< quoted_attr_inside<TAOCPP_PEGTL_ISTRING( #name ), '"'> > : \
		generate_text_with_replacement< generated_cstr::open__ ## name, generated_cstr::attr, nullptr, nullptr, '"'> {}; \
	template<> struct http_gen_actions< quoted_attr_inside<TAOCPP_PEGTL_ISTRING( #name ), '\''> > : \
		generate_text_with_replacement< generated_cstr::open__ ## name, generated_cstr::attr, nullptr, nullptr, '\''> {}; \
	template<> struct http_gen_actions< unquoted_attr<TAOCPP_PEGTL_ISTRING( #name )> > : \
		generate_text_with_replacement< generated_cstr::open__ ## name, generated_cstr::attr> {}; \
	template<> struct http_gen_actions<close_tag<TAOCPP_PEGTL_ISTRING( #name )>> : generate_text< generated_cstr::close__ ## name > {};\
	template<> struct trace_gen_actions<open_attr_tag<TAOCPP_PEGTL_ISTRING( #name )>> : trace_push_tag< generated_cstr::name__ ## name > {}; \
	template<> struct trace_gen_actions<close_tag<TAOCPP_PEGTL_ISTRING( #name )>> : trace_pop_tag< generated_cstr::name__ ## name > {};

#define ATTR_TAG(name, regex_match, error, out_tag, out_attributes) \
	template<> struct http_gen_actions< quoted_attr_inside<TAOCPP_PEGTL_ISTRING( #name ), '"'> > : \
		generate_text_with_replacement< generated_cstr::open__ ## name, generated_cstr::attr, generated_cstr::regex__ ## name, generated_cstr::error__ ## name, '"'> {}; \
	template<> struct http_gen_actions< quoted_attr_inside<TAOCPP_PEGTL_ISTRING( #name ), '\''> > : \
		generate_text_with_replacement< generated_cstr::open__ ## name, generated_cstr::attr, generated_cstr::regex__ ## name, generated_cstr::error__ ## name, '\''> {}; \
	template<> struct http_gen_actions< unquoted_attr<TAOCPP_PEGTL_ISTRING( #name )> > : \
		generate_text_with_replacement< generated_cstr::open__ ## name, generated_cstr::attr, generated_cstr::regex__ ## name, generated_cstr::error__ ## name> {}; \
	template<> struct trace_gen_actions< quoted_attr_inside<TAOCPP_PEGTL_ISTRING( #name ), '"'> > : \
		trace_check_argument< generated_cstr::regex__ ## name, generated_cstr::error__ ## name, '"'> {}; \
	template<> struct trace_gen_actions< quoted_attr_inside<TAOCPP_PEGTL_ISTRING( #name ), '\''> > : \
		trace_check_argument< generated_cstr::regex__ ## name, generated_cstr::error__ ## name, '\''> {}; \
	template<> struct trace_gen_actions< unquoted_attr<TAOCPP_PEGTL_ISTRING( #name )> > : \
		trace_check_argument< generated_cstr::regex__ ## name, generated_cstr::error__ ## name> {}; \
	template<> struct http_gen_actions<close_tag<TAOCPP_PEGTL_ISTRING( #name )>> : generate_text< generated_cstr::close__ ## name > {};\
	template<> struct trace_gen_actions<open_attr_tag<TAOCPP_PEGTL_ISTRING( #name )>> : trace_push_tag< generated_cstr::name__ ## name > {}; \
	template<> struct trace_gen_actions<close_tag<TAOCPP_PEGTL_ISTRING( #name )>> : trace_pop_tag< generated_cstr::name__ ## name > {};

#include "tag_definitions.hh"
#undef SIMPLE_TAG
#undef SIMPLE_TAG_MAP
#undef SIMPLE_TAG_ATTR
#undef SELF_CLOSING_TAG
#undef GREEDY_TAG_UNCHECKED
#undef GREEDY_TAG
#undef ATTR_TAG_UNCHECKED
#undef ATTR_TAG


// This annoys the heck outta me, but it's not WRONG, just ugly
#define IDENTIFIERS(tag_name) \
	template<> struct identifier_actions<  generated::bracket_tag_ ##tag_name > : identifier_action<  generated::bracket_tag_ ##tag_name > {}; \
	template<> struct identifier_actions<  generated::bracket_wtag_ ##tag_name > : identifier_action<  generated::bracket_wtag_ ##tag_name > {}; \
	template<> struct identifier_actions<  generated::bracket_xtag_ ##tag_name > : identifier_action<  generated::bracket_xtag_ ##tag_name > {};
#define SIMPLE_TAG(name)                                IDENTIFIERS(name)
#define SIMPLE_TAG_MAP(name, out_tag)                   IDENTIFIERS(name)
#define SIMPLE_TAG_ATTR(name, out_tag, out_attributes)  IDENTIFIERS(name)
#define SELF_CLOSING_TAG(name, out_html)                IDENTIFIERS(name)
#define GREEDY_TAG_UNCHECKED(name, ...)                 IDENTIFIERS(name)
#define GREEDY_TAG(name, ...)                           IDENTIFIERS(name)
#define ATTR_TAG_UNCHECKED(name, ...)                   IDENTIFIERS(name)
#define ATTR_TAG(name, ...)                             IDENTIFIERS(name)
#include "tag_definitions.hh"
#undef SIMPLE_TAG
#undef SIMPLE_TAG_MAP
#undef SIMPLE_TAG_ATTR
#undef SELF_CLOSING_TAG
#undef GREEDY_TAG_UNCHECKED
#undef GREEDY_TAG
#undef ATTR_TAG_UNCHECKED
#undef ATTR_TAG
#undef IDENTIFIERS



}

#endif /* INCLUDE_GUARD_FOR_BBCODE_GENERATED_ACTIONS */
