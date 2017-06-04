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

	struct taggable_content : seq<untagged_content, star<generated::any_complete_tag, untagged_content>> {};
	
	struct document : seq<untagged_content, star<generated::any_complete_tag, untagged_content>, must<eof>> {};

	template<typename entity> tag_identity identify(std::string &snippet) {
		tag_identity identified_tag {"", k_unknown};
		try {
			tao::pegtl::string_input<> in(snippet);
			tao::pegtl::parse <tao::pegtl::must<entity>, identifier_actions>(in, identified_tag);
			return identified_tag;
		} catch (const tao::pegtl::parse_error &error) {
			return tag_identity {"", k_unknown};
		}
	}
	
}


static std::string entity_quote_escape(const std::string &txt) {
	std::ostringstream escaped;
	for(const char &ch : txt) {
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

void check_validity(std::string document) {
	using namespace bbcode;
	tao::pegtl::string_input<> in(document);
	std::vector<bbcode::trace::tag> parse_stack;
	try {
		tao::pegtl::parse <bbcode::document, bbcode::trace_gen_actions>(in, parse_stack);
	} catch (const bbcode::argument_error &error) {
		bbcode::trace::annotated_code diagnosis;
		diagnosis.code = document;
		diagnosis.annotations.insert(bbcode::trace::code_annotation(error.culprit, bbcode::trace::k_error));
		std::ostringstream description;
		description << "Tag argument <span style='bbcode_error_argument'>" << entity_quote_escape(error.argument);
		description << "</span> didn't fit the expected format. " << error.message;
		throw bbcode::trace::parse_error(description.str(), diagnosis);
	} catch (const tao::pegtl::parse_error &error) {
		std::cout << "Aborted with error `" << error.what() << "` and parse stack:" << std::endl;
		for(const bbcode::trace::tag &tag : parse_stack) {
			std::cout << tag << std::endl;
		}


		bbcode::trace::annotated_code diagnosis;
		diagnosis.code = document;

		std::string failure_point = document.substr(error.positions[0].byte);		
		if (failure_point.empty()) {
			std::ostringstream description;
			description << "Looks like the post ended too soon. These tags still need to be closed:<ul>" << std::endl;
			for(const bbcode::trace::tag &tag : parse_stack) {
				diagnosis.annotations.insert(bbcode::trace::code_annotation(tag.pos, bbcode::trace::k_context));
				description << "<li>" << entity_quote_escape(tag.text) << "</li>" << std::endl;
			}
			description << "</ul>" << std::endl;
			throw bbcode::trace::parse_error(description.str(), diagnosis);
		}
		
		bbcode::tag_identity identified_tag = bbcode::identify<bbcode::generated::any_close_tag>(failure_point);
		if (identified_tag.type) {
			diagnosis.annotations.insert(bbcode::trace::code_annotation(
				error.positions[0].line,
				error.positions[0].byte_in_line,
				identified_tag.length,
				bbcode::trace::k_error
			));
			
			std::ostringstream context_list;
			bool found_matching_open_tag = false;
			bbcode::trace::tag matching_open_tag("invalid","invalid",-1,-1,-1);
			for(auto it = parse_stack.rbegin(); it != parse_stack.rend(); it++) {
				if (it->name == identified_tag.name) {
					diagnosis.annotations.insert(bbcode::trace::code_annotation(it->pos, bbcode::trace::k_counterpart));
					matching_open_tag = *it;
					found_matching_open_tag = true;
					break;
					
				}
				diagnosis.annotations.insert(bbcode::trace::code_annotation(it->pos, bbcode::trace::k_context));
				context_list << "<li>" << entity_quote_escape(it->text) << "</li>" << std::endl;
			}
			std::ostringstream description;
			description << "Unexpected close tag [/" << entity_quote_escape(identified_tag.name) << "]. ";
			if (found_matching_open_tag) {
				description << "If you meant for it to close " << entity_quote_escape(matching_open_tag.text) << ", you need to close these first:";
			} else {
				description << "None of the currently open tags seem to match it:";
			}
			description << "<ul>" << std::endl << context_list.str() << "</ul>" << std::endl;
			
			throw bbcode::trace::parse_error(description.str(), diagnosis);
		}
		
		identified_tag = bbcode::identify<bbcode::generated::start_of_any_open_tag>(failure_point);
		if (!identified_tag.type) {
			identified_tag.length = failure_point.substr(0,16).length();
		}
		diagnosis.annotations.insert(bbcode::trace::code_annotation(
			error.positions[0].line,
			error.positions[0].byte_in_line,
			identified_tag.length,
			bbcode::trace::k_error
		));
		std::ostringstream description;
		switch(identified_tag.type) {
			case bbcode::k_simple:
				// I think the only situation where this can happen is with greedy tags.
				description << "[" << entity_quote_escape(identified_tag.name) << "] tag ";
				description << "doesn't seem to have a matching [/" << entity_quote_escape(identified_tag.name) << "].";
				break;
			case bbcode::k_wrong_simple:
				description << "[" << entity_quote_escape(identified_tag.name) << "] tag requires an argument.";
				break;
			case bbcode::k_attr:
				description << "Couldn't find a valid end of your " << entity_quote_escape(identified_tag.name) << " tag's argument. ";
				description << "Remember, there are special rules to using quotes/apostrophes/close-brackets inside a tag argument.";
				break;
			case bbcode::k_wrong_attr:
				description << "[" << entity_quote_escape(identified_tag.name) << "] tag doesn't expect an argument.";
				break;
			default:
				description << "Invalid bbcode detected, but not sure exactly what's wrong. Contact technical support.<br/>";
				description << entity_quote_escape(error.what());
				break;
		}
		throw bbcode::trace::parse_error(description.str(), diagnosis);
	}
}
