#ifndef ContainerWin_H
#define ContainerWin_H

#include <qwidget.h>


#include "../common/xchat.h"

class MainWindow;

class ContainerView: public QWidget
{
    Q_OBJECT

    public:
        ContainerView (QWidget* parent = 0, const char* name = 0,  MainWindow *w=0, server*s=0);
        virtual void setWindow(MainWindow *w);
        virtual MainWindow* window();

        server *getServer(){return m_server;}
        void setServer(server *s){m_server=s;}

    public slots:
        virtual void showView();
        virtual void closeView();
	virtual void setActiveView();
		
    private:
        MainWindow *m_window;
        server *m_server;
};

#endif
