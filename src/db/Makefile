RPEXPRESS_HH = muse.hh mun.hh artwork.hh upload.hh userpic.hh trait.hh

DB_HEADERS = baby_phrase.hh rpexpress.hh
DB_CXX = $(DB_HEADERS:.hh=.odb.cc)
DB_OBJECTS = $(DB_HEADERS:.hh=.odb.o)

rpexpress.hh: $(RPEXPRESS_HH)
	cat $(RPEXPRESS_HH) > rpexpress.hh

odb_support.a: $(DB_OBJECTS)
	ar rcs odb_support.a $(DB_OBJECTS)

%.odb.cc: %.hh
	odb --hxx-suffix hh --ixx-suffix ii --cxx-suffix cc --sql-suffix .odbsql \
		--odb-file-suffix .odb. --generate-query --generate-schema \
		--hxx-prologue "#include \"prologue.hh\"" -I /home/rpexpress/local/include \
		--std c++11 --sloc-limit 10000 -d pgsql $*.hh
clean:
	rm -f *.odb.ii *.odb.cc *.odb.hh *.odbsql *.a *.odb.o rpexpress.hh
