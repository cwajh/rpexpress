#ifndef INCLUDE_GUARD_FOR_REQUEST_CONTEXT_HH
#define INCLUDE_GUARD_FOR_REQUEST_CONTEXT_HH

#include <string>
#include <map>
#include <odb/transaction.hxx>
#include <pqxx/pqxx>

struct request_context {
	pqxx::transaction<> *p_transaction;
	odb::core::database *p_odb;
	std::wstring path;
	std::map<std::wstring, std::wstring> get;
	std::map<std::string, std::string> session;
	std::map<std::wstring, std::wstring> post_outcome;
	std::wstring post_error;
};

#endif /* INCLUDE_GUARD_FOR_REQUEST_CONTEXT_HH */
