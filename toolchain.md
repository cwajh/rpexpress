Templated pages are written in Copal. This is a language inspired by Amber
(https://github.com/burner/sweet.hpp.git), but completely rewritte and with
some meaningful differences.

These are all brought together by a script that generates a URL regex-matcher
from a config file (paths/paths.lst).

Database is currently directly-accessed Postgres. But that will soon become
some ORM thingy (ODB most likely).

Makefile works. Invoke the binary with spawn-fcgi and point an nginx at it.

Full list of libraries you will need:
	Boost utf8 encode/decode.
	fastcgi++ (which pulls in much more Boost-y stuff)
	libpqxx
	libcryptopp
	(libodb soon?)

Kinda want to remove Boost but until the new fastcgi++ is ready for prime time
it'd be a hassle. Maybe some wcstrtombs thing can work?

Unsolved: hitting static file, then backfilling to dynamic file?
