#ifndef INCLUDE_GUARD_FOR_BBCODE_PARSE_TRACE_HH
#define INCLUDE_GUARD_FOR_BBCODE_PARSE_TRACE_HH

#include <functional>
#include <iostream>
#include <set>
#include <stdexcept>

namespace bbcode {
	namespace trace {
		struct codepos {
			int line;
			int column;
			int len;
			codepos(int line, int column, int len);
			bool operator<(const codepos &b) const;
			bool operator==(const codepos &b) const;
			bool operator!=(const codepos &b) const;
			bool operator>(const codepos &b) const;
			bool operator<=(const codepos &b) const;
			bool operator>=(const codepos &b) const;
		};
		
		std::ostream& operator<<(std::ostream &out, const codepos &pos);

		struct tag {
			std::string name;
			std::string text;
			codepos pos;
			tag(const std::string &name, const std::string &text, int line, int column, int len) : name(name), text(text), pos(line, column, len) {};
		};

		std::ostream& operator<<(std::ostream &out, const tag &t);
		
		enum annotation_type {
			k_invalid,
			k_error,
			k_counterpart,
			k_context
		};
		
		std::ostream& operator<<(std::ostream &out, const annotation_type &type);
		
		struct code_annotation : public codepos {
			annotation_type type;
			code_annotation(int line, int column, int len, annotation_type type) : codepos(line,column,len), type(type) {};
			code_annotation(const codepos &pos, annotation_type type) : codepos(pos), type(type) {};
		};
		struct annotated_code {
			std::string code;
			std::set<code_annotation> annotations;
			std::string html() const;
		};
		class parse_error : public std::runtime_error {
		public:
			std::string html_annotations;
			parse_error(const std::string &description, const annotated_code &annotations) : std::runtime_error(description), html_annotations(annotations.html()) {};
		};
		
	}
}

#endif /* INCLUDE_GUARD_FOR_BBCODE_PARSE_TRACE_HH */

