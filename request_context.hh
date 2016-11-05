#pragma once

#include <string>
#include <map>
#include <pqxx/pqxx>

struct request_context {
	pqxx::transaction<> *p_transaction;
	std::wstring path;
	std::map<std::wstring, std::wstring> get;
};
