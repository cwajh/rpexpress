#ifndef INCLUDE_GUARD_FOR_DB_MUN_HH
#define INCLUDE_GUARD_FOR_DB_MUN_HH
#include <array>
#include <cstdint>
#include <string>

#include <odb/core.hxx>

namespace db {
	#pragma db object
	class mun {
		friend class odb::access;
	public:
		mun(){}
		#pragma db id
		int64_t mun_id;
		#pragma db type("VARCHAR(20)")
		std::string username;
		#pragma db type("VARCHAR(140)")
		std::string email;
		long pw_salt;
		#pragma db type("CHAR(43)")
		char pw_hash[43];
	};
}

#endif /*INCLUDE_GUARD_FOR_DB_MUN_HH*/
