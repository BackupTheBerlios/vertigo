#ifndef FE_KDE_H
#define FE_KDE_H

#include <kcompletion.h>

#include "mainview.h"
#include "mainwindow.h"
#include "extraview.h"
#include "textwidget.h"

extern
 "C" {
    typedef struct server_gui {
    	RawlogView *rawlog;
		ChanlistView  *chanlist;
    } server_gui;

    typedef struct session_gui {
	MainView *view;
	MainWindow *win;
	KCompletion *comp;
	KCompletion *cmdcomp;
	int lag;
	bool showbar;
    } session_gui;
}
#endif
