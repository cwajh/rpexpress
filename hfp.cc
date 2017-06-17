#include "src/crypto.hh"
#include <iostream>
#include <string>
#include <cstdint>

int main(int argc, char* argv[]) {
	using namespace std;
	if(argc < 3){
		cerr << "usage: hfp password salt"<<endl;
		return 1;
	}
	string pw(argv[1]);
	string s_salt(argv[2]);
	int64_t salt = stoll(s_salt);
	cout << hash_for_password(pw,salt) << endl;
	return 0;
}
