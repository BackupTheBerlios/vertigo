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
    	XChatRawlogView *rawlog;
	XChatChanlistView  *chanlist;
    } server_gui;

    typedef struct session_gui {
	XChatMainView *view;
	XChatMainWindow *win;
	KCompletion *comp;
	KCompletion *cmdcomp;
	int lag;
	bool showbar;
    } session_gui;
}
#endif
