#ifndef FE_KDE_H
#define FE_KDE_H

#include <kcompletion.h>

#include "mainview.h"
#include "mainwindow.h"
#include "extraview.h"
#include "textwidget.h"

namespace Vertigo
{
	class RawlogView;
	class ChanlistView;
	class MainView;
	class MainWindow;
}


extern
 "C" {
    typedef struct server_gui {
    	Vertigo::RawlogView *rawlog;
		Vertigo::ChanlistView  *chanlist;
    } server_gui;

    typedef struct session_gui {
	Vertigo::MainView *view;
	Vertigo::MainWindow *win;
	KCompletion *comp;
	KCompletion *cmdcomp;
	int lag;
	bool showbar;
    } session_gui;
}
#endif
