#ifndef INCLUDE_GUARD_FOR_DB_ARTWORK_HH
#define INCLUDE_GUARD_FOR_DB_ARTWORK_HH

#include <odb/core.hxx>

#include "dbtypes.hh"

namespace db {
	class upload;
	class muse;

	#pragma db object pointer(std::shared_ptr)
	class artwork {
		friend class odb::access;
	public:
		artwork(){}

		#pragma db id
		pkey_t artwork_id;

		#pragma db not_null
		P<muse> subject;

		#pragma db not_null
		P<upload> resource;

		int position;

		std::wstring tooltip;
		std::wstring fallback_text;
		std::wstring description;
	};
}

#endif /*INCLUDE_GUARD_FOR_DB_ARTWORK_HH*/
