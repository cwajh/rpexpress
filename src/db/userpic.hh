#ifndef INCLUDE_GUARD_FOR_DB_USERPIC_HH
#define INCLUDE_GUARD_FOR_DB_USERPIC_HH

#include <odb/core.hxx>

#include "dbtypes.hh"

namespace db {
	class upload;
	class mun;

	#pragma db object pointer(std::shared_ptr)
	class userpic {
		friend class odb::access;
	public:
		userpic(){}

		#pragma db id
		pkey_t userpic_id;

		#pragma db not_null
		P<mun> owner;

		#pragma db not_null
		P<upload> resource;

		int position;

		std::wstring option_text;
		std::wstring tooltip;
		std::wstring fallback_text;
		std::wstring credit;
	};
}

#endif /*INCLUDE_GUARD_FOR_DB_USERPIC_HH*/
