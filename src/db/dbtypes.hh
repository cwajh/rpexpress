#ifndef INCLUDE_GUARD_FOR_DB_DBTYPES_HH
#define INCLUDE_GUARD_FOR_DB_DBTYPES_HH

#include <cstdint>
#include <memory>

#include <odb/lazy-ptr.hxx>

#include "../bbcode/bbcode.hh"

/*#define P std::shared_ptr
#define W std::weak_ptr
#define LS odb::lazy_shared_ptr
#define LW odb::lazy_weak_ptr
*/


template<typename T> using P = std::shared_ptr<T>;
template<typename T> using W = std::weak_ptr<T>;

namespace db {
	// pkey_t is a memory sandwich. normal indices will count up from bit 0.
	// counting down from bit 62 will be a server/thread identifier. exact details tbd.
	typedef std::int64_t pkey_t;

	template<typename T> using LP = odb::lazy_shared_ptr<T>;
	template<typename T> using LW = odb::lazy_weak_ptr<T>;
}


#pragma db value(std::wstring) type("TEXT")
#pragma db value(bbcode::block) type("TEXT")

#endif /*INCLUDE_GUARD_FOR_DB_DBTYPES_HH*/
