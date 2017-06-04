#ifndef INCLUDE_GUARD_FOR_BBCODE_TAG_TYPES_HH
#define INCLUDE_GUARD_FOR_BBCODE_TAG_TYPES_HH

#include <stdexcept>
#include <tao/pegtl.hpp>

namespace bbcode {
	using namespace tao::pegtl;
	using namespace tao::pegtl::ascii;
	
	enum preview_type { k_unknown = 0, k_simple, k_attr, k_wrong_simple, k_wrong_attr, k_close, k_wrong_close };

	// These have self-referential definitions; can only declare them for now.
	struct untagged_content;
	struct taggable_content;
	
	// General pattern.
	template<typename tag_name>
	struct open_tag : seq<one<'['>, tag_name, one<']'>> {};

	template<typename tag_name>
	struct close_tag : seq<one<'['>, one<'/'>, tag_name, one<']'>> {};

	template<typename tag_name>
	struct enclosing_tag : seq<
		open_tag<tag_name>,
		taggable_content,
		must<close_tag<tag_name>>
	> {};

	// Attribute tags.
	template <typename tag_name>
	struct unquoted_attr : star< not_one<']','"','\''> > {};

	template <typename tag_name, char quote_char>
	struct quoted_attr_inside : star<
		not_at<
			one<quote_char>, one<']'>
		>, sor<
			not_one<quote_char>,
			two<quote_char>
		>
	> {};

	template <typename tag_name, char quote_char>
	struct quoted_attr : seq <
		one<quote_char>,
		quoted_attr_inside<tag_name, quote_char>,
		one<quote_char>
	> {};

	template<typename tag_name>
	struct open_attr_tag : seq<
		one<'['>, tag_name, one<'='>,
		//must<
			sor<unquoted_attr<tag_name>, quoted_attr<tag_name, '"'>, quoted_attr<tag_name, '\''>>,
			one<']'>
		//>
	> {};

	template<typename tag_name>
	struct attr_tag : seq<
		open_attr_tag<tag_name>,
		taggable_content,
		must<close_tag<tag_name>>
	> {};

	// Greedy tags care solely about their contents; inherit so that "open_tag" and "close_tag" action rules do not trigger.
	template<typename tag_name> struct greedy_open_tag : open_tag<tag_name> {};
	template<typename tag_name> struct greedy_close_tag : close_tag<tag_name> {};
	template<typename tag_name> struct greedy_tag_contents : star<not_at<close_tag<tag_name>>, any> {};

	template<typename tag_name>
	struct greedy_tag : seq<
		greedy_open_tag<tag_name>,
		greedy_tag_contents<tag_name>,
		greedy_close_tag<tag_name>
	> {};

}

#endif /* INCLUDE_GUARD_FOR_BBCODE_TAG_TYPES_HH */
