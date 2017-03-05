LOCALD = /home/rpexpress/local

top.fcgi: genpaths_fcgi.cc copal_headers copal_paths cwajh.hh request_context.hh
	g++ --std=c++1y -g -I$(PWD) -I$(LOCALD)/include -I. -L$(LOCALD)/lib -Wall \
		-lpqxx -lboost_thread-mt -lboost_system-mt -pthread -lcryptopp \
		session.cc urlencode.cc base64.cc keys.cc post.cc html_utility.cc \
		-lboost_date_time-mt genpaths_fcgi.cc $(LOCALD)/lib/libfastcgipp.a \
		-o top.fcgi

copal_headers:
	cd copal && make all

copal_paths:
	cd paths && make cplpaths.hh

clean_copal:
	cd copal && make clean

clean_paths:
	cd paths && make clean

clean: clean_copal clean_paths
	rm top.fcgi

install:
	killall testing.fcgi; cp top.fcgi ~/testing.fcgi
