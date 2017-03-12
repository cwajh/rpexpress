#ifndef INCLUDE_GUARD_FOR_DB_MUSE_HH
#define INCLUDE_GUARD_FOR_DB_MUSE_HH

#include <string>

#include <odb/core.hxx>

#include "dbtypes.hh"

namespace db {
	class mun;

	#pragma db object pointer(std::shared_ptr)
	class muse {
		friend class odb::access;
	public:
		muse(){}

		#pragma db id
		pkey_t muse_id;

		#pragma db type("VARCHAR(20)")
		std::wstring username;

		#pragma db type("VARCHAR(100)")
		std::wstring name;

		std::wstring description;
		pkey_t default_avatar;

		#pragma db not_null
		P<mun> owner;
	};
}

#endif /*INCLUDE_GUARD_FOR_DB_MUSE_HH*/
