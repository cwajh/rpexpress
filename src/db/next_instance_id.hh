#ifndef INCLUDE_GUARD_FOR_DB_NEXT_INSTANCE_ID_HH
#define INCLUDE_GUARD_FOR_DB_NEXT_INSTANCE_ID_HH

#include <odb/core.hxx>

#include "dbtypes.hh"

namespace db {
	#pragma db view table("next_instance_id")
	struct next_instance_id {
		#pragma db column("nextval") type("BIGINT")
		std::int64_t nextval;
	};
}

#endif /*INCLUDE_GUARD_FOR_DB_NEXT_INSTANCE_ID_HH*/
