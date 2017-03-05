#!/usr/bin/python2
from __future__ import print_function
import json
import sys
import os
import string

file = None
if len(sys.argv) > 1:
	file = sys.argv[1]
else:
	file = os.path.join(os.path.dirname(os.path.realpath(sys.argv[0])),"paths.lst")

def str2cconst(s):
	return ''.join((q if q in string.ascii_letters+string.digits+";<=>?[]^_{|}~!#%&'()*+,-./: " else "\\x%x"%ord(q)) for q in s)

imports = []
regex_declarations = []
path_declarations = []
post_declarations = []
with open(file) as paths:
	lineno = 0
	for line in paths:
		lineno += 1
		if not line:
			continue
		if line[0] == "#":
			continue
		if '\t' not in line:
			sys.stderr.write('%s: file %s line %d has no tab\n'%(sys.argv[0],file, lineno))
		module, regex = line.split('\t',1)
		regex = regex.rstrip('\n')
		imports.append('#include <copal/%s.hh>'%module)
		regex_declarations.append('const std::wregex path%d(L"%s");'%(lineno, str2cconst(regex)))
		path_declarations.append(
r"""		if(std::regex_match(ctx.path, path%(lineno)d)){
			std::map<std::string, std::string> headers;
			auto params = copal::%(module)s::gen_headers(ctx, headers);
			for(auto &header : headers) {
				out << s2w(header.first);
				out << ": ";
				out << s2w(header.second);
				out << "\r\n";
			}
			for(auto &header : headers_for_session_data(ctx.session)) {
				out << header.first;
				out << ": ";
				out << header.second;
				out << "\r\n";
			}
			out << "\r\n";
			html_utility util(ctx);
			copal::%(module)s::gen_html<Output,const decltype(params),html_utility>(out,params,util);
			return;
		}
"""%{'lineno':lineno,'module':module})
		post_declarations.append(
r"""		if(std::regex_match(ctx.path, path%(lineno)d)){
			if(ctx.post_error.empty()) {
				return copal::%(module)s::perform_post(ctx,fields,files);
			} else {
				return copal::%(module)s::handle_bad_post(ctx,fields,files);
			}
		}
"""%{'lineno':lineno,'module':module})

print ("""#ifndef CPLPATHS_DEFINED
#define CPLPATHS_DEFINED""")

print ('#include <regex>')
print ('#include <cwajh.hh>')
print ('#include <html_utility.hh>')
print ('#include <session.hh>')
for import_stmt in imports:
	print (import_stmt)

print ("namespace cplpaths {")

for declaration in regex_declarations:
	print ("\t"+declaration)

print ("\ttemplate<typename Output, typename Context> inline void gen_response(Output &out, Context &ctx) {")
for declaration in path_declarations:
	print(declaration)
print (
r"""		out << "Status: 404 Not Found\r\nContent-Type: text/html; charset=utf-8\r\n\r\n";
		out << "<h1>Template Not Found</h1>\r\n";
		out << "The server couldn't find a page matching \"" << ctx.path << "\".\r\n";
		out << "This isn't expected; normally you should at least be seeing a fancier 404 page.\r\n";
		out << "Please copy-paste this whole error message and send it to whoever runs the site.";
""")
print ("\t}\n")

print ("\ttemplate<typename Context, typename File> inline std::wstring perform_post(Context &ctx, const std::map<std::wstring, std::wstring> &fields, const std::map<std::wstring, File> &files) {")
for declaration in post_declarations:
	print(declaration)
print ('\t\tif(ctx.post_error.empty()) { ctx.post_error=std::wstring(L"Template not found: ") + ctx.path; }')
print ('\t\treturn L"";')
print ("\t}\n")

print ("}\n")
print ("#endif")
