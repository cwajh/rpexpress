#include "bbcode.hh"
#include <iostream>
#include <tao/pegtl.hpp>
#include <tao/pegtl/argv_input.hpp>
#include <tao/pegtl/analyze.hpp>

std::string preplace(const std::string original, const std::string needle, const std::string replacement) {
	std::string output = original;
	auto needle_pos = output.find(needle);
	while(needle_pos != std::string::npos) {
		output.replace(needle_pos, needle.length(), replacement);
		needle_pos = output.find(needle, needle_pos + replacement.length());
	}
	return output;
}

template<typename rule> struct token_list_actions {
	template< typename matched_a > static void apply( const matched_a& in ) {
		std::string indented_str = preplace(in.string(), "\n", "\n\t");
		std::cout << "Found " << typeid(rule).name();
		std::cout << " @ " << in.line() << ":" << in.byte_in_line() << std::endl;
		std::cout << "\t" << indented_str << std::endl;
	}
};

int main(int argc, char* argv[]) {
	using namespace std;
	cout << "Hi world." << endl;
	if( argc < 2 ) {
		cout << "(nothing to do)" << endl;
		return 1;
	}
	tao::pegtl::analyze <bbcode::document>();
	tao::pegtl::argv_input<> in(argv, 1);
	tao::pegtl::parse <bbcode::lexed_document, token_list_actions>(in);
	cout << endl;
	return 0;
}

