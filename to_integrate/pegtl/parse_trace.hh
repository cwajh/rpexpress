#ifndef INCLUDE_GUARD_FOR_BBCODE_PARSE_TRACE_HH
#define INCLUDE_GUARD_FOR_BBCODE_PARSE_TRACE_HH

#include <functional>
#include <iostream>
#include <queue>
#include <set>
#include <stdexcept>

namespace bbcode {
	namespace trace {
		struct codepos {
			int line;
			int column;
			int len;
			codepos(int line, int column, int len) : line(line), column(column), len(len) {};
			bool operator<(const codepos &b) const {
				if (line < b.line) {
					return true;
				}
				if (line == b.line && column < b.column) {
					return true;
				}
				return false;
			}
			bool operator==(const codepos &b) const {
				return (line == b.line) && (column == b.column);
			}
			bool operator!=(const codepos &b) const {
				return !(*this == b);
			}
			bool operator>(const codepos &b) const {
				return !(*this == b) && !(*this < b);
			}
			bool operator<=(const codepos &b) const {
				return !(*this > b);
			}
			bool operator>=(const codepos &b) const {
				return !(*this < b);
			}
		};
		
		std::ostream& operator<<(std::ostream &out, const codepos &pos) {
			out << pos.line << ":" << pos.column;
			return out;
		}

		struct tag {
			std::string name;
			std::string text;
			codepos pos;
			tag(const std::string &name, const std::string &text, int line, int column, int len) : name(name), text(text), pos(line, column, len) {};
		};
		struct closed_tag : public tag {
			codepos close_pos;
			closed_tag(const std::string &name, const std::string &text, int line, int column, int len, int close_line, int close_column, int close_len) :
				tag(name, text, line, column, len), close_pos(close_line, close_column, close_len) {};
			closed_tag(const tag &base, int close_line, int close_column, int close_len) :
				tag(base), close_pos(close_line, close_column, close_len) {};
		};

		std::ostream& operator<<(std::ostream &out, const tag &t)
		{
			out << "A [" << t.name << "] tag, called `" << t.text << "`, at " << t.pos;
			return out;
		}
		
		enum annotation_type {
			k_invalid,
			k_error,
			k_counterpart,
			k_context
		};
		
		std::ostream& operator<<(std::ostream &out, const annotation_type &type)
		{
			switch(type) {
			case k_error:
				out << "error";
				break;
			case k_counterpart:
				out << "counterpart";
				break;
			case k_context:
				out << "context";
				break;
			default:
				out << "invalid_annotation_type";
				break;
			}
			return out;
		}
		
		struct code_annotation : public codepos {
			annotation_type type;
			code_annotation(int line, int column, int len, annotation_type type) : codepos(line,column,len), type(type) {};
			code_annotation(const codepos &pos, annotation_type type) : codepos(pos), type(type) {};
		};
		struct annotated_code {
			std::string code;
			std::set<code_annotation> annotations;
			std::string html() const 	{
				codepos current_pos(1,0,0);
				std::priority_queue<int, std::vector<int>, std::greater<int>> annotation_ends;
				std::ostringstream escaped;
				escaped << "<pre>" << std::endl;
				for(int i=0; i<code.length(); ++i) {
					// TODO(cwajh): support multiple annotations in the same place.
					while(annotation_ends.size() > 0 && annotation_ends.top() <= i) {
						annotation_ends.pop();
						escaped << "</span>";
					}
					auto it_annotation = annotations.find(code_annotation(current_pos, k_invalid));
					if(it_annotation != annotations.end() && (it_annotation->len > 0)) {
						escaped << "<span style='bbcode_annotation_" << (it_annotation->type) << "'>";
						annotation_ends.push(i + (it_annotation->len));
					}
					switch(code[i]) {
					case '<':
						escaped << "&lt;";
						break;
					case '>':
						escaped << "&gt;";
						break;
					case '&':
						escaped << "&amp;";
						break;
					case '\n':
						current_pos.line++;
						current_pos.column = -1;
						// fall through
					default:
						escaped << code[i];
					}
					current_pos.column++;
				}
				// close out
				while(annotation_ends.size() > 0) {
					annotation_ends.pop();
					escaped << "</span>";
				}
				escaped << "</pre>" << std::endl;
				return escaped.str();
			};
		};
		class parse_error : public std::runtime_error {
		public:
			std::string html_annotations;
			parse_error(const std::string &description, const annotated_code &annotations) : std::runtime_error(description), html_annotations(annotations.html()) {};
		};
		
	}
}

#endif /* INCLUDE_GUARD_FOR_BBCODE_PARSE_TRACE_HH */

/*

[a] one [b] two [c] three [/a]

Keep stack of open tags.

Does it look like a close tag?
	Unwind stack until you find a matching open tag.
	What did you unwind?
		Make it context.

	Found a matching open?
		Make it counterpart.
		"Unexpected close tag [/a]. If you meant for it to close [a], you need to close these first:
		* [c]
		* [b]"
	Didn't find one?
		"Unexpected close tag [/a]. None of the currently open tags seem to match it:
		* [c]
		* [b]"
Unexpected end?
	Get whole stack and mark them error.
	"Looks like the post ended too soon. These tags still need to be closed:
		* [c]
		* [b]
		* [a]"

=======
	"Seems like the greedy tag [youtube] was never closed."
=======
	"Wasn't able to find the end of the open tag that started at [style=. Double-check your syntax."
=======

*/

