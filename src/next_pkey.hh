#ifndef INCLUDE_GUARD_FOR_NEXT_PKEY_HH
#define INCLUDE_GUARD_FOR_NEXT_PKEY_HH

#include "db/dbtypes.hh"

/// Returns a unique identifier for use in creating DB objects.
db::pkey_t next_pkey();

#endif /*INCLUDE_GUARD_FOR_NEXT_PKEY_HH*/
