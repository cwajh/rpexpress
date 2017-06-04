#ifndef INCLUDE_GUARD_FOR_DB_TRAIT_HH
#define INCLUDE_GUARD_FOR_DB_TRAIT_HH

#include <odb/core.hxx>

#include "dbtypes.hh"
#include "../bbcode/bbcode.hh"

namespace db {
	enum trait_type { major=1, minor };
	#pragma db object pointer(std::shared_ptr)
	class trait {
		friend class odb::access;
	public:
		trait(){}

		#pragma db id
		pkey_t trait_id;
		int position;

		#pragma db not_null
		LP<muse> character;

		trait_type category;

		#pragma db type("VARCHAR(140)")
		std::wstring name;
		bbcode::block value;
	};
}

#endif /*INCLUDE_GUARD_FOR_DB_TRAIT_HH*/
