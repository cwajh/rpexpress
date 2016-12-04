#include <boost/date_time/posix_time/posix_time.hpp>
#include <fstream>
#include <fastcgi++/request.hpp>
#include <fastcgi++/manager.hpp>
#include <string>
#include <vector>
#include <pqxx/pqxx>
#include "request_context.hh"
#include "stuff.hh"
#include "cwajh.hh"

void error_log(const char* msg)
{
   using namespace std;
   using namespace boost;
   static ofstream error;
   if(!error.is_open())
   {
      error.open("/tmp/errlog", ios_base::out | ios_base::app);
      error.imbue(std::locale(error.getloc(), new posix_time::time_facet()));
   }
   error << '[' << posix_time::second_clock::local_time() << "] " << msg << endl;
   error.flush();
}
class TopServlet: public Fastcgipp::Request<wchar_t>
{
   bool response()
   {
      pqxx::connection db_conn("dbname=rpexpress");
      pqxx::work request_xact(db_conn, "request");
      struct request_context ctx = {
        &request_xact,
        environment().scriptName,
        environment().gets,
        session_data_for_cookies(environment().cookies)
      };
      //ctx.p_transaction = &request_xact;
      //ctx.path = environment().scriptName;
      //ctx.get = environment().gets;
      //ctx.post = environment().posts;

      std::map<std::string, std::string> headers;
      auto params = copal::stuff::gen_headers(ctx, headers);
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
      copal::stuff::gen_html<Fastcgipp::Fcgistream<wchar_t>,decltype(params)>(out,params);
      /*
      out << "<html><head><meta http-equiv='Content-Type' content='text/html; charset=utf-8' />";
      out << "<title>fastcgi++: Hello World in UTF-8</title></head><body>";
      out << "English: Hello World<br />";
      out << "Russian: " << russian << "<br />";
      out << "Greek: " << greek << "<br />";
      out << "Chinese: " << chinese << "<br />";
      out << "Japanese: " << japanese << "<br />";
      out << "Runic English?: " << runic << "<br />";
      out << "</body></html>";
      */

      err << "Hello apache error log";

	/// XXX(cwajh)
	out << L"<!--" << std::to_wstring((long long) &ctx.session);
      for(auto &session_var : ctx.session) {
	out << "X-CPL-";
	out << session_var.first;
         out << ": ";
         out << session_var.second;
         out << "\r\n";
      }
	out << L"-->" << endl;

      return true;
   }
};
int main()
{
   try
   {
      Fastcgipp::Manager<TopServlet> fcgi;
      fcgi.handler();
   }
   catch(std::exception& e)
   {
      error_log(e.what());
   }
}

