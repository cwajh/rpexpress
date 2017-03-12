#ifndef INCLUDE_GUARD_FOR_DB_MUN_HH
#define INCLUDE_GUARD_FOR_DB_MUN_HH
#include <array>
#include <cstdint>
#include <string>

#include <odb/core.hxx>

#include "dbtypes.hh"

namespace db {
	#pragma db object pointer(std::shared_ptr)
	class mun {
		friend class odb::access;
	public:
		mun(){}

		#pragma db id
		pkey_t mun_id;

		#pragma db type("VARCHAR(20)")
		std::string username;

		#pragma db type("VARCHAR(140)")
		std::string email;

		int64_t pw_salt;

		#pragma db type("CHAR(43)")
		std::string pw_hash; //base64 encoded sha256

		pkey_t default_userpic;

		//pragma db inverse(mun)
	};
}

#endif /*INCLUDE_GUARD_FOR_DB_MUN_HH*/
