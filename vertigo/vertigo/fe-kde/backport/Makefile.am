noinst_LIBRARIES = libbackport.a

INCLUDES = $(all_includes)

EXTRA_DIST = ktabbar.h ktabwidget.h

libbackport_a_SOURCES = ktabbar.cpp ktabwidget.cpp

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/vertigo.pot
