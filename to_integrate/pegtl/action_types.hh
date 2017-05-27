#ifndef INCLUDE_GUARD_FOR_BBCODE_ACTION_TYPES_HH
#define INCLUDE_GUARD_FOR_BBCODE_ACTION_TYPES_HH

#include <tao/pegtl.hpp>

// TODO(cwajh): switch for arbitrary ostream
#include <iostream>
#include <sstream>
#include <regex>

#include "tag_types.hh"

namespace bbcode {

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
		template< typename matched_a > static void apply( const matched_a& in ) {
			std::cout << entity_escape(in.string());
		}
	};

	template<char const* text> struct generate_text {
		template< typename matched_a > static void apply( const matched_a& in ) {
			std::cout << text;
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

	template<char const* text, char const* needle, char const* replacement_regex = nullptr, char const* regex_error = nullptr, char to_dedup = 0> struct generate_text_with_replacement {
		
		template< typename matched_a > static void apply( const matched_a& in ) {
			std::string replacement = in.string();
			
			if (to_dedup) {
				char double_char[3] = {to_dedup, to_dedup, 0};
				replace_all(replacement, double_char, 2, &double_char[1]);
			}
			
			if (replacement_regex) {
				if (!std::regex_match(replacement, std::regex(replacement_regex))) {
					std::ostringstream error_message;
					error_message << "Tag attribute `" << entity_escape(replacement);
					error_message << "` (on column " << in.byte_in_line() << " of row " << in.line() << ") ";
					error_message << "didn't fit the expected format.";
					if (regex_error) {
						error_message << " " << regex_error;
					}
					throw std::logic_error(error_message.str());
				}
			}
			
			// Sub replacement into text.
			std::string out_text(text);
			constexpr size_t needle_length = strlen(needle);
			replace_all(out_text, needle, needle_length, entity_escape(replacement));
			std::cout << out_text;
		}
	};
}

#endif /* INCLUDE_GUARD_FOR_BBCODE_ACTION_TYPES_HH */
