#include "next_pkey.hh"

#include <cassert>
#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>

#include "db/next_instance_id.hh"
#include "db/rpexpress.odb.hh"

namespace db {
	namespace next_pkey {
		class pkey_generator {
			pkey_t current;
		public:
			pkey_generator() {
				odb::pgsql::database odb_conn("rpexpress","","rpexpress");
				odb::transaction txn(odb_conn.begin());
				odb::result<next_instance_id> seq_result(odb_conn.query<next_instance_id>());
				assert(seq_result.begin() != seq_result.end());
				std::int64_t instance_id = seq_result.begin()->nextval;
				txn.commit();
				//low tech but easy to check
				current = 0;
				for(int source=62;source>=0;source--){
					std::int64_t bitmask = (1<<source);
					if((instance_id&bitmask) != 0) {
						// bit 62 becomes bit 0, bit 61 becomes 1...
						int dest = (62-source);
						std::int64_t bit_to_set = 1<<dest;
						current |= bit_to_set;
					}
				}
				//current now has the first value for the given instance. :)
			}
			pkey_t next() {
				return current++;
			}
		};
		thread_local pkey_generator my_generator;
	}
}

db::pkey_t next_pkey() {
	return db::next_pkey::my_generator.next();
}
