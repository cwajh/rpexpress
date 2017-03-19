Templated pages are written in Copal. This is a language inspired by Amber
(https://github.com/burner/sweet.hpp.git), but completely rewritte and with
some meaningful differences.

Persistence is handled by ODB with the permissive license LoC limit.

Interface with the HTTP server is via FastCGI++.

These are all brought together by a script that generates a URL regex-matcher
from a config file (paths/paths.lst).

Makefile works for me but probably not for you.

Once compiled, Invoke the binary with spawn-fcgi and point an nginx at it.

Full list of libraries you will need:
	c++14
	Boost utf8 encode/decode.
	fastcgi++ (which pulls in much more Boost-y stuff)
	libodb
	libcryptopp
	a keys.cc with your own generated sitewide peppers

Kinda want to remove Boost but until the new fastcgi++ is ready for prime time
it'd be a hassle. Maybe some wcstrtombs thing can work?

Unsolved: hitting static file, then backfilling to dynamic file?
