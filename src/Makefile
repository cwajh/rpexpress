export LOCALD = /home/rpexpress/local
export CXXFLAGS = --std=c++1y -g -I$(PWD) -I$(LOCALD)/include -I. -Wall

TOP_CCS = session.cc urlencode.cc base64.cc keys.cc post.cc html_utility.cc genpaths_fcgi.cc
TOP_OBJECTS = session.o urlencode.o base64.o keys.o post.o html_utility.o genpaths_fcgi.o
TOP_LIBS = -lpqxx -lboost_thread-mt -lboost_system-mt -lcryptopp -lodb -lodb-pgsql -lboost_date_time-mt

top.fcgi: genpaths_fcgi.cc copal_headers copal_paths cwajh.hh request_context.hh $(TOP_OBJECTS)
	g++ --std=c++1y -g -L$(LOCALD)/lib -Wall $(TOP_OBJECTS) $(LOCALD)/lib/libfastcgipp.a \
		$(TOP_LIBS) -pthread -o top.fcgi

# pull in dependency info for *existing* .o files
-include $(TOP_OBJECTS:.o=.d)

# compile and generate dependency info
%.o: %.cc
	g++ -c $(CXXFLAGS) $*.cc -o $*.o
	g++ -MM $(CXXFLAGS) $*.cc > $*.d

copal_headers:
	cd copal && make all

copal_paths:
	cd paths && make cplpaths.hh

clean_copal:
	cd copal && make clean

clean_paths:
	cd paths && make clean

clean: clean_copal clean_paths
	rm -f top.fcgi *.o *.d

install:
	killall testing.fcgi; cp top.fcgi ~/testing.fcgi
