#ifndef XCHATTABWIDGET_H
#define XCHATTABWIDGET_H

#include "ktabwidget.h"


class XChatTabWidget:public KTabWidget {
  Q_OBJECT public:
     XChatTabWidget(QWidget * parent = 0, const char *name = 0);
    ~XChatTabWidget();

    void insertTab(QWidget * child, const QString & label, int index = -1);
    void removePage(QWidget * w);
};

#endif

