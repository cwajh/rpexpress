#include "bbcode.hh"
#include <functional>
#include <iostream>
#include <fstream>
#include <tao/pegtl.hpp>



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
		bbcode::check_validity(bbcode_doc);
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

