
#ifndef CL_H
#define CL_H

#include "../common/xchat.h"
#include <klistview.h>
#include "chanlistbase.h"

#include <qregexp.h>
#include <qptrlist.h>

class ChanlistItem;


class ChanlistView : public ChanlistViewBase
{
    Q_OBJECT

public:
    ChanlistView( QWidget* parent = 0, server * s=0);
    ~ChanlistView();
    void enableItems(bool yes);
    void appendChannel(QString chan,QString users,QString topic);
    bool isMatch(ChanlistItem*);

public slots:
    void slotApplyButtonClicked();
    void slotJoinButtonClicked();
    void slotRefreshButtonClicked();

private slots:    
    void setActiveView();
		
    
private:
	void insertItem ( QListViewItem * i );
	void takeItem ( QListViewItem * i );
	void setMatchSettings();
        bool m_matchTopic;
        bool m_matchChan;
        int m_minUsers;
        int m_maxUsers;
        QRegExp m_regExp;
	QPtrList <ChanlistItem> m_chanList;	
};

class ChanlistItem:public KListViewItem {
  public:
    ChanlistItem(QListView * list, QString chan, QString users, QString topic);
    ~ChanlistItem();
    int users(){return m_users;}
    bool isTaken(){return m_taken;}
    void setTaken(bool taken){m_taken=taken;}
    int compare ( QListViewItem * i, int col, bool ascending ) const;
 private:
    int m_users;
    bool m_taken;
};


#endif


