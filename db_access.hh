#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <pqxx/pqxx>

struct BabyPhrase {
	std::wstring phrase;
	int64_t index;
};

std::vector<BabyPhrase> ez_phrases();
std::vector<BabyPhrase> phrases(pqxx::work &transaction);


//template<typename RowType> std::vector<RowType> db_rows(std::string query);


