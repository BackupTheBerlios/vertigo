#include <kdebug.h>

#include "containerview.h"
#include "mainwindow.h"

ContainerView::ContainerView (QWidget * parent, const char *name, MainWindow *w, server *s) : QWidget (parent, name, WDestructiveClose)
{
    setWindow(w);
        setServer(s);
}

void ContainerView::setWindow(MainWindow *w)
{
    kdDebug() << "container setwindow"<<endl;
    m_window=w;
}

void ContainerView::showView()
{
        if (window())
                window()->showView(this);
}

void ContainerView::closeView()
{
        close();
}

void ContainerView::setActiveView()
{
	;	
}

MainWindow* ContainerView::window()
{
    return m_window;
}
