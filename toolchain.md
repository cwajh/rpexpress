Templated pages are written in Copal. This is a slight modification of the Amber
(https://github.com/burner/sweet.hpp.git) language, completely rewritten.

Use a single fastcgi++ to call all page headers for dynamic access.

	g++ --std=c++1y -I/home/rpexpress/local/include -L/home/rpexpress/local/lib \
		-lboost_thread-mt -lboost_system-mt -pthread -lboost_date_time-mt \
		fcgi_stuff.cc local/lib/libfastcgipp.a -o stuff.fcgi

Configure lighttpd mod_fastcgi to hit that.

Unsolved: hitting static file, then backfilling to dynamic file?
