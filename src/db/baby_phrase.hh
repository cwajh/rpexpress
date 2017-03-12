#ifndef INCLUDE_GUARD_FOR_DB_BABY_PHRASE_HH
#define INCLUDE_GUARD_FOR_DB_BABY_PHRASE_HH
#include <cstdint>
#include <string>

#include <odb/core.hxx>

#include "dbtypes.hh"

namespace db {
	#pragma db object
	class baby_phrase {
		friend class odb::access;
	public:
		baby_phrase(){}
		#pragma db id
		long idx;
		std::wstring phrase;
	};
}

#endif /*INCLUDE_GUARD_FOR_DB_BABY_PHRASE_HH*/
