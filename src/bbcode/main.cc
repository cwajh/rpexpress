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
	bbcode::block block;
	try {
		block = bbcode::block(bbcode_doc);
	} catch (const bbcode::trace::parse_error &error) {
		cerr << error.what() << endl;
		cerr << error.html_annotations << endl;
		return 1;
	}
	cout << block.html() << endl;
	return 0;
}

