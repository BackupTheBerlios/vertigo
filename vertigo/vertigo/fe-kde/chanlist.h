
#ifndef CL_H
#define CL_H

#include "../common/xchat.h"
#include <klistview.h>
#include "chanlistbase.h"

class ChanlistView : public ChanlistViewBase
{
    Q_OBJECT

public:
    ChanlistView( QWidget* parent = 0, server * s=0);
    ~ChanlistView();
    void enableItems(bool yes);
    void appendChannel(QString chan,QString users,QString topic);
    bool isMatch(QString chan,QString users,QString topic);

public slots:
    void slotApplyButtonClicked();
    void slotJoinButtonClicked();
    void slotRefreshButtonClicked();


};

class ChanlistItem:public KListViewItem {
  public:
    ChanlistItem(QListView * list, QString chan, QString users, QString topic);
    ~ChanlistItem();
    int compare ( QListViewItem * i, int col, bool ascending ) const;
};

#endif


