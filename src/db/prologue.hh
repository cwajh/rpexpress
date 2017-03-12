#ifndef INCLUDE_GUARD_FOR_DB_PROLOGUE_HH
#define INCLUDE_GUARD_FOR_DB_PROLOGUE_HH

#include <string>

#include <odb/pgsql/traits.hxx>

#include "../cwajh.hh"

namespace odb
{
	namespace pgsql
	{
		template <>
		class value_traits<std::wstring, id_string>
		{
		public:
			typedef std::wstring value_type;
			typedef value_type query_type;
			typedef details::buffer image_type;
			static void set_value(std::wstring &v, const details::buffer &b, std::size_t n, bool is_null) {
				std::string enc_v;
				value_traits<std::string, id_string>::set_value(enc_v, b, n, is_null);
				v = s2w(enc_v);
			}
			static void set_image(details::buffer &b, std::size_t &n, bool &is_null, const std::wstring &v) {
				std::string enc_v = w2s(v);
				value_traits<std::string, id_string>::set_image(b, n, is_null, enc_v);
			}
		};
	}
}

#endif /*INCLUDE_GUARD_FOR_DB_PROLOGUE_HH*/
