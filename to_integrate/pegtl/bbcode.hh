#include <tao/pegtl.hpp>

// TODO(cwajh): switch for arbitrary ostream
#include <iostream>

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
#include "tag_types.hh"
#include "generated_tag_patterns.hh"
#include "action_types.hh"
#include "generated_actions.hh"
//#include "generated_actions.hh"

namespace bbcode {
	using namespace tao::pegtl;
	using namespace tao::pegtl::ascii;

	struct start_of_any_single_tag : sor<generated::start_of_any_open_tag, generated::any_close_tag> {};	

	struct untagged_content : star<not_at<start_of_any_single_tag>, any> {};

	///////////////////////
	// lex debug
	struct lat_size    : open_attr_tag<TAOCPP_PEGTL_ISTRING("size")> {};
	struct lat_color   : open_attr_tag<TAOCPP_PEGTL_ISTRING("color")> {};
	struct lat_font    : open_attr_tag<TAOCPP_PEGTL_ISTRING("font")> {};
	struct lat_style   : open_attr_tag<TAOCPP_PEGTL_ISTRING("style")> {};
	struct lat_link    : open_attr_tag<TAOCPP_PEGTL_ISTRING("url")> {};

	struct lt_bold         : open_tag<one<'b'>> {};
	struct lt_italic       : open_tag<one<'i'>> {};
	struct lt_underline    : open_tag<one<'u'>> {};
	struct lt_strike       : open_tag<one<'s'>> {};
	struct lt_sup          : open_tag<TAOCPP_PEGTL_ISTRING("sup")> {};
	struct lt_sub          : open_tag<TAOCPP_PEGTL_ISTRING("sub")> {};
	struct lt_items        : open_tag<TAOCPP_PEGTL_ISTRING("ul")> {};
	struct lt_list         : open_tag<TAOCPP_PEGTL_ISTRING("ol")> {};
	struct lt_item         : open_tag<TAOCPP_PEGTL_ISTRING("li")> {};
	struct lt_quote        : open_tag<TAOCPP_PEGTL_ISTRING("quote")> {};
	struct lt_left_align   : open_tag<TAOCPP_PEGTL_ISTRING("left")> {};
	struct lt_right_align  : open_tag<TAOCPP_PEGTL_ISTRING("right")> {};
	struct lt_center_align : open_tag<TAOCPP_PEGTL_ISTRING("center")> {};
	
	struct any_open_tag : sor< 
  lat_size    ,
  lat_color   ,
 lat_font    ,
 lat_style   ,
 lt_sup          ,
 lt_sub          ,
 lt_items        ,
 lt_list         ,
 lt_item         ,
 lt_quote        ,
 lt_left_align   ,
 lt_right_align  ,
 lt_center_align ,
 lat_link, generated::tt_hr,
 lt_bold         ,
 lt_italic       ,
 lt_underline    ,
 lt_strike       , generated::gt_code, generated::gt_img, generated::gt_youtube> {};
	
	struct lexed_document : seq<
		untagged_content,
		star<
			sor<any_open_tag,generated::any_close_tag>,
			untagged_content
		>, must<eof>
	> {} ;
	
	///////////////////////

	struct taggable_content : seq<untagged_content, star<generated::any_complete_tag, untagged_content>> {};
	
	struct document : seq<untagged_content, star<generated::any_complete_tag, untagged_content>, must<eof>> {};

	template<typename rule> struct annotation_actions : nothing<rule> {};
	
	template<> struct annotation_actions<untagged_content> {
		template< typename matched_a > static void apply( const matched_a& in ) {
			std::cout << "|" << in.string() << "|";
		}
	};
	template<typename tag_name> struct annotation_actions<open_tag<tag_name>> {
		template< typename matched_a > static void apply( const matched_a& in ) {
			std::cout << "~" << in.string() << "~";
		}
	};
	template<typename tag_name, char quote_char> struct annotation_actions<quoted_attr_inside<tag_name, quote_char>> {
		template< typename matched_a > static void apply( const matched_a& in ) {
			std::cout << "_" << quote_char << "_" << in.string() << "_";
		}
	};
	template<typename tag_name> struct annotation_actions<unquoted_attr<tag_name>> {
		template< typename matched_a > static void apply( const matched_a& in ) {
			std::cout << "__" << in.string() << "_";
		}
	};
	template<typename tag_name> struct annotation_actions<greedy_tag_contents<tag_name>> {
		template< typename matched_a > static void apply( const matched_a& in ) {
			std::cout << "<" << in.string() << ">";
		}
	};
	template<typename tag_name> struct annotation_actions<close_tag<tag_name>> {
		template< typename matched_a > static void apply( const matched_a& in ) {
			std::cout << "`" << in.string() << "`";
		}
	};
	
}
