#include <kdebug.h>

#include "tabwidget.h"
#include "ktabwidget.h"
#include "ktabbar.h"

#include "../common/xchat.h"
#include "../common/xchatc.h"
#include "../common/fe.h"
#include "../common/cfgfiles.h"


XChatTabWidget::XChatTabWidget(QWidget * parent, const char *name)
:KTabWidget(parent, name)
{
// TODO :: :when config arrives, add option to group tabs-- if false then enable this    
setTabReorderingEnabled(true);
    tabBar()->hide();
}

XChatTabWidget::~XChatTabWidget()
{
}

void XChatTabWidget::insertTab(QWidget * child, const QString & label,
			       int index)
{
    KTabWidget::insertTab(child, label.left(prefs.truncchans), index);
    kdDebug() << "insertTab" << count() << endl;
    if (count() > 1) {
	tabBar()->show();
	QWidget *w = cornerWidget(Qt::TopRight);
	if (w)
	    w->show();
	w = cornerWidget(Qt::TopLeft);
	if (w)
	    w->show();
    }
    if (prefs.newtabstofront || (count()==1))
    {
        showPage(child);
        //child->show();
        }
        /*else{
            if (count()==1)
                child->show();
        }*/

}

void XChatTabWidget::removePage(QWidget * w)
{
    //setCurrentPage (currentPageIndex ()-1);
    KTabWidget::removePage(w);
    if (count() < 2) {
	tabBar()->hide();
	QWidget *w = cornerWidget(Qt::TopRight);
	if (w)
	    w->hide();
	w = cornerWidget(Qt::TopLeft);
	if (w)
	    w->hide();
    }
}
