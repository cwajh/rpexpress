#include "bbcode.hh"
#include <functional>
#include <iostream>
#include <fstream>
#include <tao/pegtl.hpp>

namespace bbcode {
	template<typename entity> bbcode::tag_identity identify(std::string &snippet) {
		bbcode::tag_identity identified_tag {"", k_unknown};
		try {
			tao::pegtl::string_input<> in(snippet);
			tao::pegtl::parse <tao::pegtl::must<entity>, bbcode::identifier_actions>(in, identified_tag);
			return identified_tag;
		} catch (const tao::pegtl::parse_error &error) {
			return bbcode::tag_identity {"", k_unknown};
		}
	}
}

static std::string entity_escape(const std::string &txt) {
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
	tao::pegtl::string_input<> in(document);
	std::vector<bbcode::trace::tag> parse_stack;
	try {
		tao::pegtl::parse <bbcode::document, bbcode::trace_gen_actions>(in, parse_stack);
	} catch (const bbcode::argument_error &error) {
		bbcode::trace::annotated_code diagnosis;
		diagnosis.code = document;
		diagnosis.annotations.insert(bbcode::trace::code_annotation(error.culprit, bbcode::trace::k_error));
		std::ostringstream description;
		description << "Tag argument <span style='bbcode_error_argument'>" << entity_escape(error.argument);
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
				description << "<li>" << entity_escape(tag.text) << "</li>" << std::endl;
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
				context_list << "<li>" << entity_escape(it->text) << "</li>" << std::endl;
			}
			std::ostringstream description;
			description << "Unexpected close tag [/" << entity_escape(identified_tag.name) << "]. ";
			if (found_matching_open_tag) {
				description << "If you meant for it to close " << entity_escape(matching_open_tag.text) << ", you need to close these first:";
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
				description << "[" << entity_escape(identified_tag.name) << "] tag ";
				description << "doesn't seem to have a matching [/" << entity_escape(identified_tag.name) << "].";
				break;
			case bbcode::k_wrong_simple:
				description << "[" << entity_escape(identified_tag.name) << "] tag requires an argument.";
				break;
			case bbcode::k_attr:
				description << "Couldn't find a valid end of your " << entity_escape(identified_tag.name) << " tag's argument. ";
				description << "Remember, there are special rules to using quotes/apostrophes/close-brackets inside a tag argument.";
				break;
			case bbcode::k_wrong_attr:
				description << "[" << entity_escape(identified_tag.name) << "] tag doesn't expect an argument.";
				break;
			default:
				description << "Invalid bbcode detected, but not sure exactly what's wrong. Contact technical support.<br/>";
				description << entity_escape(error.what());
				break;
		}
		throw bbcode::trace::parse_error(description.str(), diagnosis);
	}
}

int main(int argc, char* argv[]) {
	using namespace std;
	//cout << "Hi world." << endl;
	if( argc < 2 ) {
		cout << "(nothing to do)" << endl;
		return 1;
	}
	
	ifstream t(argv[1]);
	string bbcode_doc((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());	
	try {
		check_validity(bbcode_doc);
	} catch (const bbcode::trace::parse_error &error) {
		cerr << error.what() << endl;
		cerr << error.html_annotations << endl;
		return 1;
	}
	
	std::vector<bbcode::trace::tag> aaaaa;
	tao::pegtl::file_input<> in2(argv[1]);
	tao::pegtl::parse <bbcode::document, bbcode::http_gen_actions>(in2, aaaaa);
	cout << endl;
	return 0;
}

