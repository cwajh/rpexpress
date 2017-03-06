#ifndef INCLUDE_GUARD_FOR_POST_HH
#define INCLUDE_GUARD_FOR_POST_HH

#include <map>
#include <string>

typedef struct {
	std::wstring path;
	std::wstring content_type;
} post_file_t;

template<typename Stream, typename Context>
static void perform_post(
	Stream &out,
	const std::map<std::wstring, std::wstring> &fields,
	const std::map<std::wstring, post_file_t> &files,
	Context &ctx
);

template<typename Context> static void populate_post_outcome(Context &ctx);

std::wstring post_validation_html(const std::wstring &action_url, const std::string session_id);

/* ==== IMPLEMENTATIONS BELOW - PLEASE IGNORE ==== */
#include "cwajh.hh"
#include "paths/cplpaths.hh"
#include "session.hh"
#include "urlencode.hh"

std::wstring xsrf_error_for_post(
        const std::map<std::wstring, std::wstring> &fields,
        const std::string &path,
        const std::string &session_identifier,
	std::map<std::string, std::string> &mac_fields
);

std::wstring url_with_post_outcome(
	const std::wstring &url,
	const std::map<std::wstring, std::wstring> &post_outcome,
	const std::wstring &post_error
);

template<typename Stream, typename Collection>
inline void output_headers(Stream &out, const Collection &headers) {
	for(auto &header : headers) {
		out << header.first;
		out << ": ";
		out << header.second;
		out << "\r\n";
	}
}

template<typename Stream, typename Context>
static void perform_post(
	Stream &out,
	const std::map<std::wstring, std::wstring> &fields,
	const std::map<std::wstring, post_file_t> &files,
	Context &ctx
) {
	std::map<std::string, std::string> mac_fields;
	ctx.post_error = xsrf_error_for_post(fields, w2s(ctx.path), session_identifier(ctx.session), mac_fields);

	std::wstring destination = cplpaths::perform_post(ctx, fields, files);

	if(destination.empty())	{
		if(ctx.post_error.empty()) {
			ctx.post_error = L"You tried to submit something to this website, but it doesn't know how to use that submission.";
		}
		out << L"400 Bad Request\r\n";
		output_headers(out, headers_for_session_data(ctx.session));
		out << L"Content-Type: text/html; charset=utf-8\r\n\r\n";
		out << L"<h1>Error 400 - Mangled POST</h1>\r\n";
		out << ctx.post_error;
		out << L"<p>Also, this was so unexpected that the site doesn't know how to handle the error.</p>";
		out << L"This is almost certainly a bug. Please let the site operators know, and include this info:<pre>";
		out << s2w(query_string_for_map(mac_fields));
		out << L"\r\n";
		output_headers(out, fields);
		out << L"</pre>Sorry for the trouble. Best wishes.";
		return;
	} else {
#ifdef POST303
		// 303-style continuations suffer a problem: where do you pass the outcome data?
		// Everything is very size-limited - cookies, GETs, all.
		// POST fields are unlimited but there's no way to redirect with new POST data.
		destination = url_with_post_outcome(destination, ctx.post_outcome, ctx.post_error);
		out << L"303 See Other\r\n";
		out << L"Location: ";
		out << destination << L"\r\n";
		output_headers(out, headers_for_session_data(ctx.session));
		out << L"\r\n";
#else
		// Server-side continuations don't change the URL bar, but should otherwise work.
		ctx.path = strip_query(destination);
		ctx.get = ws_map_for_query_string(w2s(query_string_from_url(destination)));
		// Need to refresh ctx.transaction? unclear
		cplpaths::gen_response<Stream,Context>(out,ctx);
#endif /* POST303 */
	}
}

template<typename Context> static void populate_post_outcome(Context &ctx){
#ifdef POST303
	// Implementation would go here, if it was the plan.
#endif /* POST303 */
}

#endif /* INCLUDE_GUARD_FOR_POST_HH */

