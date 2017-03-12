#!/usr/bin/python
import sys

path = sys.argv[1]
obj_name = path.rsplit("/",1)[-1].split(".",1)[0]

with open(path,'w') as f:
	f.write( """#ifndef INCLUDE_GUARD_FOR_DB_%(u)s_HH
#define INCLUDE_GUARD_FOR_DB_%(u)s_HH

#include <odb/core.hxx>

#include "dbtypes.hh"

namespace db {
	#pragma db object pointer(std::shared_ptr)
	class %(l)s {
		friend class odb::access;
	public:
		%(l)s(){}

		#pragma db id
		pkey_t %(l)s_id;

		//#pragma db type("VARCHAR(20)")
		//std::string field;
	};
}

#endif /*INCLUDE_GUARD_FOR_DB_%(u)s_HH*/
"""%{'u':obj_name.upper(), 'l':obj_name.lower()})
