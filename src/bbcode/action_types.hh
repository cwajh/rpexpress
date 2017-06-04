#ifndef INCLUDE_GUARD_FOR_BBCODE_ACTION_TYPES_HH
#define INCLUDE_GUARD_FOR_BBCODE_ACTION_TYPES_HH

#include <tao/pegtl.hpp>

// TODO(cwajh): switch for arbitrary ostream
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <regex>

#include "parse_trace.hh"
#include "tag_types.hh"

namespace bbcode {
	
	template<typename rule> struct identifier_actions : tao::pegtl::nothing<rule> {};
	struct tag_identity { const char *name; preview_type type; int length; };

	template<typename rule> struct identifier_action {
		template< typename matched_a > static void apply( const matched_a& in, tag_identity &found_identity ) {
			found_identity.name = rule::name;
			found_identity.type = rule::tag_type;
			found_identity.length = in.string().length();
		}
	};
	
	template<typename rule> struct trace_gen_actions : tao::pegtl::nothing<rule> {};
	
	template<char const* name> struct trace_push_tag {
		template< typename matched_a > static void apply( const matched_a& in, std::vector<trace::tag> &tag_stack ) {
			trace::tag tag(name, in.string(), in.line(), in.byte_in_line(), in.string().length());
			tag_stack.push_back(tag);
		}
	};
	
	template<char const* name> struct trace_pop_tag {
		template< typename matched_a > static void apply( const matched_a& in, std::vector<trace::tag> &tag_stack ) {
			if (tag_stack.empty()) {
				throw std::logic_error("TODO(cwajh): Popping when there's no tags left.");
			}
			if (std::string(name) != tag_stack.back().name){
				throw std::logic_error("TODO(cwajh): Popping wrong tag.");
			}
			tag_stack.pop_back();
		}
	};
	
	
	template<typename rule> struct http_gen_actions : tao::pegtl::nothing<rule> {};

	static inline std::string entity_escape(const std::string &txt) {
		std::ostringstream escaped;
		for(const char ch : txt) {
			switch(ch) {
			case '<':
				escaped << "&lt;";
				break;
			case '>':
				escaped << "&gt;"; 
				break;
			case '&':
				escaped << "&amp;";
				break;
			case '"':
				escaped << "&quot;";
				break;
			case '\'':
				escaped << "&apos;";
				break;
			default:  
				escaped << ch;
			}
		} 
		return escaped.str();
	}

	template<> struct http_gen_actions<untagged_content> {
		template< typename matched_a > static void apply( const matched_a& in, std::ostream &out ) {
			out << entity_escape(in.string());
		}
	};

	template<char const* text> struct generate_text {
		template< typename matched_a > static void apply( const matched_a& in, std::ostream &out ) {
			out << text;
		}
	};
	
	static inline void replace_all(std::string &text, const char *needle, size_t needle_len, std::string replacement) {
		size_t current_position_in_string = text.find(needle);
		while (current_position_in_string != std::string::npos) {
			text.replace(
				current_position_in_string,
				needle_len,
				replacement
			);
			current_position_in_string = text.find(needle, current_position_in_string + replacement.length());
		}
	}
	
	
	class argument_error : public std::runtime_error {
	public:
		trace::codepos culprit;
		std::string argument;
		std::string message;
		argument_error(const trace::codepos &culprit, const std::string &argument, const std::string &message) :
			std::runtime_error((std::string("Tag argument didn't fit the expected format. ")) + message),
			message(message),
			argument(argument),
			culprit(culprit) {};
	};
	std::string replacement_text(int line, int column, const std::string &argument, char const* replacement_regex = nullptr, char const* regex_error = nullptr, char to_dedup = 0) {
		std::string replacement(argument);
		if (to_dedup) {
			char double_char[3] = {to_dedup, to_dedup, 0};
			replace_all(replacement, double_char, 2, &double_char[1]);
		}
		
		if (replacement_regex) {
			if (!std::regex_match(replacement, std::regex(replacement_regex))) {
				throw argument_error(trace::codepos(line, column, argument.length()), replacement, regex_error);
			}
		}
		return replacement;
	}

	template<char const* text, char const* needle, char const* replacement_regex = nullptr, char const* regex_error = nullptr, char to_dedup = 0> struct generate_text_with_replacement {
		
		template< typename matched_a > static void apply( const matched_a& in, std::ostream &out ) {
			std::string replacement = replacement_text(in.line(), in.byte_in_line(), in.string(), replacement_regex, regex_error, to_dedup);
			// Sub replacement into text.
			std::string out_text(text);
			constexpr size_t needle_length = strlen(needle);
			replace_all(out_text, needle, needle_length, entity_escape(replacement));
			out << out_text;
		}
	};
	
	
	template<char const* replacement_regex = nullptr, char const* regex_error = nullptr, char to_dedup = 0> struct trace_check_argument {
		template< typename matched_a > static void apply( const matched_a& in, std::vector<trace::tag> &tag_stack ) {
			// Can throw out the value; what's important is that it's valid.
			replacement_text(in.line(), in.byte_in_line(), in.string(), replacement_regex, regex_error, to_dedup);
		}
	};
}

#endif /* INCLUDE_GUARD_FOR_BBCODE_ACTION_TYPES_HH */
