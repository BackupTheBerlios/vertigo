noinst_LIBRARIES = libbackport.a

INCLUDES = -I./res $(all_includes)

libbackport_a_SOURCES = kmdichildarea.cpp kmdichildfrm.cpp kmdichildfrmcaption.cpp \
				kmdichildview.cpp kmdimainfrm.cpp kmditaskbar.cpp kmdidockcontainer.cpp \
				kdockwidget_compat.cpp dockmainwindow_compat.cpp kmultitabbar.cpp \
				kmditoolviewaccessor.cpp kmdidocumentviewtabwidget.cpp kmdifocuslist.cpp \
				ktabbar.cpp ktabwidget.cpp


KDE_OPTIONS = nofinal
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/vertigo.pot
