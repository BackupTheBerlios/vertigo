#include "chanlist.h"

#include <kmessagebox.h>
#include <kpushbutton.h>
#include <klineedit.h>
#include <knuminput.h>
#include <kconfig.h>

#include "../common/xchatc.h"
#include "../common/outbound.h"
#include "../common/util.h"

ChanlistView::ChanlistView(QWidget * parent, server * s)
    :  ChanlistViewBase(parent, "ChanList")
{
	setServer(s);

    connect( m_applyButton, SIGNAL( clicked() ), this, SLOT( slotApplyButtonClicked() ) );
    connect( m_joinButton, SIGNAL( clicked() ), this, SLOT( slotJoinButtonClicked() ) );
    connect( m_refreshButton, SIGNAL( clicked() ), this, SLOT( slotRefreshButtonClicked() ) );
    m_chanlistView->setAllColumnsShowFocus(true);

	KConfig *c=new KConfig("vertigorc");
	m_minSpin->setValue(c->readNumEntry("minUsers",0));
	m_maxSpin->setValue(c->readNumEntry("maxUsers",0));
	m_textEdit->setText(c->readEntry("searchEdit"));
	c->sync();
	delete c;
	
}


ChanlistView::~ChanlistView()
{
}

void ChanlistView::enableItems(bool yes){
	if (yes)
	{
		setEnabled(true);
		m_chanlistView->scrollBy(0,1);
	}
	else{
	setEnabled(false);
	}

}


void ChanlistView::slotApplyButtonClicked()
{
}

void ChanlistView::slotJoinButtonClicked(){

}

 void ChanlistView::slotRefreshButtonClicked(){
	if (getServer()->connected)
        {
		m_chanlistView->clear();
		handle_command (getServer()->server_session, "list", FALSE);
		enableItems(false);
	}
	else
                KMessageBox::error  	(   this, "Not connected to a server",
		"error");
}


void ChanlistView::appendChannel(QString chan,QString users,QString topic)
{
	m_chanlistView->setUpdatesEnabled(false);
	ChanlistItem *i=new ChanlistItem(m_chanlistView, chan, users, topic);
	
	if (isMatch(chan, users, topic))
	{
		m_chanlistView->setUpdatesEnabled(true);
			
	}	
	else{
		m_chanlistView->takeItem(i);
		m_chanlistView->setUpdatesEnabled(true);
	}

}

bool ChanlistView::isMatch(QString chan,QString users,QString topic)
{
	return true;
}


ChanlistItem::ChanlistItem(QListView * list,  QString chan, QString users, QString topic):
KListViewItem(list, chan, users, topic)
{

}

ChanlistItem::~ChanlistItem()
{
}

int ChanlistItem::compare ( QListViewItem * i, int col, bool ascending ) const
{
	if (col!=1)
	{
		return KListViewItem::compare(i,col,ascending);
		
	}
	{
		int a=key( col, ascending ).toInt();
		int b=i->key( col, ascending ).toInt();
		if (a==b)
			return 0;
		if (a<b)
			return -1;
		return 1;
	}
}
