#include <boost/date_time/posix_time/posix_time.hpp>
#include <fstream>
#include <fastcgi++/request.hpp>
#include <fastcgi++/manager.hpp>
#include <string>
#include <vector>
#include <pqxx/pqxx>
#include "request_context.hh"
#include "cwajh.hh"
#include "paths/cplpaths.hh"

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
   static pqxx::connection *db_conn(void) {
      static thread_local pqxx::connection db_conn("dbname=rpexpress");
      return &db_conn;
   }
   bool response()
   {
      struct request_context ctx;
      pqxx::work request_xact(*db_conn(), "request");
      ctx.p_transaction = &request_xact;
      // simply using scriptName leaves out later bits of the path. no good!
      ctx.path = strip_query(environment().requestUri);
      ctx.get = environment().gets;
      //error_log(environment().scriptName);
      //ctx.post = environment().posts;

      cplpaths::gen_response<Fastcgipp::Fcgistream<wchar_t>, struct request_context>(out, ctx);
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

