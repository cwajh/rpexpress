#include "bbcode.hh"
#include <iostream>
#include <tao/pegtl.hpp>

#include "parse_tree.hh"

int main(int argc, char* argv[]) {
	using namespace std;
	cout << "Hi world." << endl;
	if( argc < 2 ) {
		cout << "(nothing to do)" << endl;
		return 1;
	}
	tao::pegtl::file_input<> in(argv[1]);
	tao::pegtl::parse <bbcode::document, bbcode::http_gen_actions>(in);
	cout << endl;
	return 0;
}

