## Makefile.am for vertigo

# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
SUBDIRS = . pics

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = vertigo

# set the include path for X, qt and KDE
INCLUDES         = $(GLIB_CFLAGS) $(all_includes)

# the library search path.
vertigo_LDFLAGS = $(KDE_RPATH) $(all_libraries)


# the libraries to link against.
vertigo_LDADD   = ../common/libxchatcommon.a $(GLIB_LIBS) $(LIB_KFILE) $(LIB_KPARTS)

# which sources should be compiled for vertigo
vertigo_SOURCES = fe-kde.cpp app.cpp configure.cpp tabwidget.cpp textwidget.cpp serverlist.cpp containerview.cpp rawlogbase.ui chanlistbase.ui extraview.cpp mainview.cpp mainview.skel mainwindow.cpp

# these are the headers for your project
noinst_HEADERS   = fe-kde.h textwidget.h configure.h tabwidget.h serverlist.h extraview.h containerview.h mainview.h mainwindow.cpp app.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/vertigo.pot

KDE_ICON = vertigo

# this is where the kdelnk file will go
kdelnkdir   = $(kde_appsdir)/Network
kdelnk_DATA = vertigo.desktop

misc_DATA = eventsrc
miscdir = $(kde_datadir)/vertigo

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/vertigo
rc_DATA = serverlist.rc vertigo.rc
