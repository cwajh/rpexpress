#include "post.hh"

#include <chrono>
#include <sstream>

#include "paths/cplpaths.hh"
#include "cwajh.hh"
#include "keys.hh"
#include "crypto.hh"
#include "urlencode.hh"

#define TIMESTAMP_FIELD L"_form_timestamp"
#define TOKEN_FIELD L"_post_token"

#define CONTINUATIONS_KEY "_post_continuations"

using namespace std::chrono_literals;
static const std::chrono::hours FORM_LIFESPAN(1h);
static const std::chrono::hours OUTCOME_LIFESPAN(1h);

class bad_meta_field : public std::runtime_error
{
	public:
	bad_meta_field(const std::string &why) : std::runtime_error(std::string("Malformed form header (") + why + ")") {
	}
};

long l_for_post_field(const std::map<std::wstring, std::wstring> &fields, const std::wstring &field) {
	try {
		return std::stol(fields.at(field));
	} catch(const std::invalid_argument&) {
		throw bad_meta_field(w2s(field));
	} catch(const std::out_of_range&) {
		throw bad_meta_field(w2s(field));
	}
}

std::wstring ws_for_post_field(const std::map<std::wstring, std::wstring> &fields, const std::wstring &field) {
	try {
		return fields.at(field);
	} catch(const std::out_of_range&) {
		throw bad_meta_field(w2s(field));
	}
}

std::wstring xsrf_error_for_post(
	const std::map<std::wstring, std::wstring> &fields,
	const std::string &path,
	const std::string &session_identifier,
	std::map<std::string, std::string> &mac_fields
) {
	try {
		long form_timestamp = l_for_post_field(fields, TIMESTAMP_FIELD);
		std::wstring token = ws_for_post_field(fields, TOKEN_FIELD);

		// Check that the form was generated for the same session that submitted it.
		mac_fields = {
			{"Target", path},
			{"Timestamp", std::to_string(form_timestamp)},
			{"Session ID", session_identifier}
		};
		if(!mac_valid_for_string(w2s(token), FORM_SALT, query_string_for_map(mac_fields))) {
			return L"Invalid form ID. Did another site redirect you here by mistake?";
		}

		// Adversaries can do a replay XSRF if forms don't expire.
		auto expiration_point = std::chrono::system_clock::now() - FORM_LIFESPAN;
		std::time_t expiration_time = std::chrono::system_clock::to_time_t(expiration_point);
		if(form_timestamp < expiration_time) {
			return L"Form expired. Please try submitting again.";
		}
	} catch(const bad_meta_field &error) {
		return std::wstring(L"Mangled POST; ") + s2w(error.what());
	}
	return L"";
}

void store_post_outcome(
	const std::map<std::wstring, std::wstring> &post_outcome,
	const std::wstring &post_error,
	std::wstring &url,
	std::map<std::string, std::string> &session
) {
	std::string outcome_string = query_string_for_map(post_outcome);

	auto outcome_expiration_point = std::chrono::system_clock::now() + OUTCOME_LIFESPAN;
	std::time_t outcome_id = std::chrono::system_clock::to_time_t(outcome_expiration_point);

	std::map<std::string, std::string> continuations = map_for_query_string(session[CONTINUATIONS_KEY]);
	continuations[std::to_string(outcome_id)] = query_string_for_map({
		{"outcome",outcome_string},{"error",w2s(post_error)}
	});
	session[CONTINUATIONS_KEY] = query_string_for_map(continuations);

	std::wstring query_string = query_string_from_url(url);
	std::map<std::string, std::string> query_params = map_for_query_string(w2s(query_string));
	query_params["_po"] = std::to_string(outcome_id);

	url = (strip_query(url) + L"?" + s2w(query_string_for_map(query_params)) + fragment_from_url(url));
}


std::wstring post_validation_html(const std::wstring &action_url, const std::string session_id) {
	std::time_t minting_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::map<std::string, std::string> mac_fields = {
		{"Target", w2s(action_url)},
		{"Timestamp", std::to_string(minting_time)},
		{"Session ID", session_id}
	};
	std::string query_string = query_string_for_map(mac_fields);
	std::string form_token = salted_mac_for_string(FORM_SALT, query_string);

	std::basic_ostringstream<wchar_t> out;

	out << L"<!-- " << s2w(query_string) << L" -->";
	out << L"<input type='hidden' name='" TIMESTAMP_FIELD L"' value='" << minting_time << L"' />";
	out << L"<input type='hidden' name='" TOKEN_FIELD L"' value='" << s2w(form_token) << L"' />";
	return out.str();
}

/*

	try {
		long form_timestamp = l_for_post_field(fields, L"_form_timestamp");
		std::wstring token = ws_for_post_field(fields, L"_post_token");

		// Adversaries can do a replay XSRF if forms don't expire.
		auto expiration_point = std::chrono::system_clock::now() - FORM_LIFESPAN;
		std::time_t expiration_time = std::chrono::system_clock::to_time_t(expiration_point);
		if(form_timestamp < expiration_time) {
			ctx.post_error = L"Form expired. Please try submitting again.";
		}

		mac_fields = {
			{"Target", w2s(ctx.path)},
			{"Timestamp", std::to_string(form_timestamp)},
			{"Session ID", session_identifier(ctx.session)}
		};
		if(!mac_valid_for_string(w2s(token), FORM_SALT, query_string_for_map(mac_fields))) {
			ctx.post_error = L"Invalid form ID. Did another site redirect you here by mistake?";
		}
	} catch(const bad_meta_field &error) {
		ctx.post_error = std::wstring(L"Mangled POST; ") + s2w(error.what());
	}
*/

