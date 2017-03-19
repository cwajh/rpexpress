#include "user.hh"

#include "db/rpexpress.odb.hh"

static const std::string USER_ID_KEY("USER_ID");

P<db::mun> current_user(odb::core::database *p_db, const std::map<std::string, std::string> &session) {
	auto it = session.find(USER_ID_KEY);
	if (it == session.end()){
		return nullptr;
	}
	uint64_t user_id = std::stoll(it->second);
	P<db::mun> user(p_db->template load<db::mun>(user_id));
	return user;
}
void set_current_user(const db::mun *p_mun, std::map<std::string, std::string> &session) {
	if(p_mun){
		session[USER_ID_KEY] = std::to_string(p_mun->mun_id);
	} else {
		session.erase(USER_ID_KEY);
	}
}

