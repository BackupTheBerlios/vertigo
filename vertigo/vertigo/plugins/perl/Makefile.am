
EXTRA_DIST=dp_misc.pl ignore.pl sample_script.pl xchat-perl18.html makefile.msc

libdir = $(xchatlibdir)/plugins

lib_LTLIBRARIES = perl.la
perl_la_SOURCES = perl.c
perl_la_LDFLAGS = -avoid-version -module 
perl_la_LIBADD = $(PERL_LDFLAGS)
CFLAGS = @CFLAGS@ -Wno-unused
INCLUDES = $(PERL_CFLAGS) $(GLIB_CFLAGS) -I$(srcdir)/..
