#include "parse_trace.hh"
#include <queue>
#include <sstream>

namespace bbcode { namespace trace {
	codepos::codepos(int line, int column, int len) : line(line), column(column), len(len) {};
	bool codepos::operator<(const codepos &b) const {
		if (line < b.line) {
			return true;
		}
		if (line == b.line && column < b.column) {
			return true;
		}
		return false;
	}
	bool codepos::operator==(const codepos &b) const {
		return (line == b.line) && (column == b.column);
	}
	bool codepos::operator!=(const codepos &b) const {
		return !(*this == b);
	}
	bool codepos::operator>(const codepos &b) const {
		return !(*this == b) && !(*this < b);
	}
	bool codepos::operator<=(const codepos &b) const {
		return !(*this > b);
	}
	bool codepos::operator>=(const codepos &b) const {
		return !(*this < b);
	}
	
	std::ostream& operator<<(std::ostream &out, const codepos &pos) {
		out << pos.line << ":" << pos.column;
		return out;
	}

	std::ostream& operator<<(std::ostream &out, const tag &t)
	{
		out << "A [" << t.name << "] tag, called `" << t.text << "`, at " << t.pos;
		return out;
	}
	
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
	
	std::string annotated_code::html() const {
		codepos current_pos(1,0,0);
		std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> annotation_ends;
		std::ostringstream escaped;
		escaped << "<pre class='bbcode_annotated_code'>" << std::endl;
		for(unsigned int i=0; i < code.length(); ++i) {
			// TODO(cwajh): support multiple annotations in the same place.
			while(annotation_ends.size() > 0 && annotation_ends.top() <= i) {
				annotation_ends.pop();
				escaped << "</span>";
			}
			auto it_annotation = annotations.find(code_annotation(current_pos, k_invalid));
			if(it_annotation != annotations.end() && (it_annotation->len > 0)) {
				escaped << "<span class='bbcode_annotation_" << (it_annotation->type) << "'>";
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
	}
}}
