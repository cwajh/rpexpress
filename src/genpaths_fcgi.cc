#include <boost/date_time/posix_time/posix_time.hpp>
#include <exception>
#include <fstream>
#include <fastcgi++/request.hpp>
#include <fastcgi++/manager.hpp>
#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>
#include <stdexcept>
#include <string>
#include <vector>


#include "request_context.hh"
#include "cwajh.hh"
#include "post.hh"
#include "paths/cplpaths.hh"
#include "session.hh"

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
}
class TopServlet: public Fastcgipp::Request<wchar_t>
{
   static odb::core::database *odb_conn() {
      // Thread-safe class!
      static odb::pgsql::database odb_conn("rpexpress","","rpexpress");
      return &odb_conn;
   }
   bool response()
   {
      error_log(w2s(L"Request received: " + environment().requestUri).c_str());
      struct request_context ctx = {
        odb_conn(),
        // simply using scriptName leaves out later bits of the path. no good!
        strip_query(environment().requestUri),
        environment().gets,
        session_data_for_cookies(environment().cookies)
      };
      error_log("Handing off to cplpaths");

try{

      if(environment().requestMethod == Fastcgipp::Http::HTTP_METHOD_POST){
        std::map<std::wstring, std::wstring> fields;
        std::map<std::wstring, post_file_t> files;
        for(auto &post : environment().posts) {
          if (post.second.type == Fastcgipp::Http::Post<wchar_t>::file) {
            files[post.first] = {post.second.filename, post.second.contentType};
          } else {
            fields[post.first] = post.second.value;
          }
        }
        perform_post<Fastcgipp::Fcgistream<wchar_t>,struct request_context>(out, fields, files, ctx);
      } else {
        populate_post_outcome<struct request_context>(ctx);
        cplpaths::gen_response<Fastcgipp::Fcgistream<wchar_t>, struct request_context>(out, ctx);
      }
      error_log("Complete");

} catch(const std::exception& e) {
	std::exception_ptr eptr = std::current_exception();
	if(eptr) {
		error_log(e.what());
		std::rethrow_exception(eptr);
	}
}
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

