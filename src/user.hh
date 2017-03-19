#ifndef INCLUDE_GUARD_FOR_USER_HH
#define INCLUDE_GUARD_FOR_USER_HH
#include <map>
#include <string>
#include <odb/transaction.hxx>

#include "db/mun.hh"

P<db::mun> current_user(odb::core::database *p_db, const std::map<std::string, std::string> &session);
void set_current_user(const db::mun *p_mun, std::map<std::string, std::string> &session);

#endif /* INCLUDE_GUARD_FOR_USER_HH */
