export LOCALD = /home/rpexpress/local
export CXXFLAGS = --std=c++1y -g -I$(PWD) -I$(LOCALD)/include -I. -Wall -Wno-unknown-pragmas

TOP_CCS = session.cc urlencode.cc base64.cc keys.cc post.cc html_utility.cc genpaths_fcgi.cc user.cc crypto.cc \
	bbcode/bbcode.cc bbcode/parse_trace.cc next_pkey.cc
TOP_OBJECTS = $(TOP_CCS:.cc=.o)
TOP_DEPFILES = $(TOP_CCS:.cc=.d)
TOP_LIBS = -lboost_thread-mt -lboost_system-mt -lcryptopp -lodb -lodb-pgsql -lboost_date_time-mt

top.fcgi: genpaths_fcgi.cc copal_headers copal_paths odb_stuff $(TOP_OBJECTS)
	g++ --std=c++1y -g -L$(LOCALD)/lib -Wall $(TOP_OBJECTS) $(LOCALD)/lib/libfastcgipp.a \
		db/odb_support.a $(TOP_LIBS) -pthread -o top.fcgi

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

odb_stuff:
	cd db && make odb_support.a

clean_copal:
	cd copal && make clean

clean_paths:
	cd paths && make clean

clean_odb:
	cd db && make clean

clean: clean_copal clean_paths clean_odb
	rm -f top.fcgi *.o *.d $(TOP_OBJECTS) $(TOP_DEPFILES)

install:
	killall testing.fcgi; cp top.fcgi ~/testing.fcgi; cp ../static/theme.css ~/www/static/theme.css
