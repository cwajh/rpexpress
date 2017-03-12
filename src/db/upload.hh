#ifndef INCLUDE_GUARD_FOR_DB_UPLOAD_HH
#define INCLUDE_GUARD_FOR_DB_UPLOAD_HH

#include <odb/core.hxx>

#include "dbtypes.hh"

namespace db {
	#pragma db object pointer(std::shared_ptr)
	class upload {
		friend class odb::access;
	public:
		upload(){}

		#pragma db id type("CHAR(43)")
		std::string hash; // sha256, base64 encoded

		#pragma db type("VARCHAR(10)")
		std::string extension;
	};
}

#endif /*INCLUDE_GUARD_FOR_DB_UPLOAD_HH*/
