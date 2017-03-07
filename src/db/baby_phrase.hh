#ifndef INCLUDE_GUARD_FOR_DB_BABY_PHRASE_HH
#define INCLUDE_GUARD_FOR_DB_BABY_PHRASE_HH
#include <cstdint>
#include <string>

#include <odb/core.hxx>

namespace db {
	#pragma db object
	class baby_phrase {
		friend class odb::access;
	public:
		baby_phrase(){}
		#pragma db id
		long idx;
		std::string phrase;
	};
}

#endif /*INCLUDE_GUARD_FOR_DB_BABY_PHRASE_HH*/
