#ifndef XCHATTABWIDGET_H
#define XCHATTABWIDGET_H

#include <ktabwidget.h>

namespace Vertigo
{

    class TabWidget:public KTabWidget
    {
      Q_OBJECT public:
                 TabWidget(QWidget * parent = 0, const char *name = 0);
            ~TabWidget();

        void insertTab(QWidget * child, const QString & label, int index =
                       -1);
        void removePage(QWidget * w);
    };

};

#endif
