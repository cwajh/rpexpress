#include "db_access.hh"

#include <pqxx/pqxx>

#include "cwajh.hh"



std::vector<BabyPhrase> ez_phrases() {
	pqxx::connection db_conn("dbname=rpexpress");
	pqxx::work baby_xact(db_conn, "baby");
	return phrases(&baby_xact);
}

std::vector<BabyPhrase> phrases(pqxx::transaction<> *transaction) {
	std::vector<BabyPhrase> out_phrases;
	pqxx::result phrase_result = transaction->exec("select * from test_phrases order by idx");
	for(pqxx::result::size_type i = 0; i < phrase_result.size(); i++) {
		std::string phrase_utf8;
		phrase_result[i][0].to(phrase_utf8);
		// std::wcout << "Encoded phrase #" << i << " is " << phrase_utf8.length() << " pseudochars." << std::endl;
		BabyPhrase phrase;
		phrase.phrase = s2w(phrase_utf8);
		phrase_result[i][1].to(phrase.index);
		out_phrases.push_back(phrase);
	}
	return out_phrases;
}
