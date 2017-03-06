#include "html_utility.hh"

#include "post.hh"
#include "session.hh"

html_utility::html_utility(struct request_context context) : context(context) {}
std::wstring html_utility::form_tokens(const std::wstring &action_url) {
	std::string session_id = session_identifier(context.session);
	return post_validation_html(action_url, session_id);
}

