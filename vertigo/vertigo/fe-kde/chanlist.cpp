#include "chanlist.h"

#include <kmessagebox.h>
#include <kpushbutton.h>
#include <klineedit.h>
#include <kdebug.h>
#include <knuminput.h>
#include <kconfig.h>
#include <qregexp.h>
#include <qptrlist.h>
#include <qcheckbox.h>

#include "../common/xchatc.h"
#include "../common/outbound.h"
#include "../common/util.h"

ChanlistView::ChanlistView(QWidget * parent, server * s)
    :  ChanlistViewBase(parent, "ChanList")
{
	setServer(s);
	enableItems(true);

	setFocusProxy(m_chanlistView);
	
	KConfig *c=new KConfig("vertigorc");
	m_minSpin->setValue(c->readNumEntry("minUsers",0));
	m_maxSpin->setValue(c->readNumEntry("maxUsers",0));
	m_textEdit->setText(c->readEntry("searchEdit"));
	c->sync();
	delete c;
	setMatchSettings();
	
	m_chanList.setAutoDelete(false);
}


ChanlistView::~ChanlistView()
{
}

void ChanlistView::enableItems(bool yes){
	if (yes)
	{
		m_stopButton->setEnabled(false);
		m_refreshButton->setEnabled(true);
		m_chanlistView->repaint();
	}
	else{
		m_stopButton->setEnabled(true);
		m_refreshButton->setEnabled(false);
	}

}

void ChanlistView::setMatchSettings()
{
        m_matchTopic=m_topicCheck->isChecked();
        m_matchChan=m_channelCheck->isChecked();
        m_minUsers=m_minSpin->value();
        m_maxUsers=m_maxSpin->value();
        if (!m_minUsers && !m_maxUsers)
                m_minUsers=-1;
	if (m_maxUsers < m_minUsers)
		m_maxUsers=9999;
        m_regExp.setPattern(m_textEdit->text());
}

void ChanlistView::setActiveView()
{
	m_textEdit->setFocus();
}

void ChanlistView::slotApplyButtonClicked()
{
        setMatchSettings();
        //m_chanlistView->setUpdatesEnabled(false);
        QPtrListIterator<ChanlistItem> it( m_chanList );
	ChanlistItem *item;
        while ( it.current() ) {
            item = it.current();
            if (!isMatch(item)){
                takeItem(item);
	    }
	    else{
	    	insertItem(item);
	    }
            ++it;
        }
        //m_chanlistView->setUpdatesEnabled(true);
	//m_chanlistView->repaint();
}




void ChanlistView::slotJoinButtonClicked(){
	if (!m_chanlistView->currentItem ())
		return;
	QString txt="join "+m_chanlistView->currentItem ()->text(0);
		
if (getServer()->connected && m_chanlistView->currentItem ()->text(0) != "*")
	{
	handle_command (getServer()->server_session, (char*)txt.latin1(), FALSE);
	}
}

 void ChanlistView::slotRefreshButtonClicked(){
	if (getServer()->connected)
        {
		m_chanList.clear();
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
	//m_chanlistView->setUpdatesEnabled(false);
	ChanlistItem *i=new ChanlistItem(m_chanlistView, chan, users, topic);
	m_chanList.append(i);
	if (isMatch(i))
	{
	//	m_chanlistView->setUpdatesEnabled(true);
	;		
	}	
	else{
		m_chanlistView->takeItem(i);
	//	m_chanlistView->setUpdatesEnabled(true);
	}

}


bool ChanlistView::isMatch(ChanlistItem *i)
{
	if (m_minUsers != -1)
	{
		if ((i->users() < m_minUsers ) || (i->users() > m_maxUsers ))
			return false;
	}
	
	if (m_matchChan)
	{
		if (m_regExp.search(i->text(0)) == -1)
			return false;
	}
	
	if (m_matchTopic)
	{
		if (m_regExp.search(i->text(2)) == -1)
			return false;
	}
	
	return true;
}

void ChanlistView::insertItem ( QListViewItem * i )
{
	ChanlistItem *ci=(ChanlistItem *)i;
	if (!ci->isTaken())
		return;
	ci->setTaken(false);
	m_chanlistView->insertItem(i);
}


void ChanlistView::takeItem ( QListViewItem * i )
{
	ChanlistItem *ci=(ChanlistItem *)i;
	if (ci->isTaken())
		return;
	ci->setTaken(true);
	m_chanlistView->takeItem(i);
}


ChanlistItem::ChanlistItem(QListView * list,  QString chan, QString u, QString topic):
KListViewItem(list, chan, u, topic)
{
	m_users=u.toInt();
	m_taken=false;
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
		ChanlistItem *ci=(ChanlistItem *)i;
                if (m_users==ci->users())
                        return 0;
                if (m_users< ci->users())
                        return -1;
                return 1;
	}
}
