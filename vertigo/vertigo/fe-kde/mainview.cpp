#include <dcopclient.h>
#include <klistview.h>
#include <qtoolbutton.h>
#include <kdebug.h>
#include <klocale.h>

#include "textwidget.h"
#include "app.h"
#include "mainview.h"

#include "../common/xchat.h"
#include "../common/xchatc.h"
#include "../common/inbound.h"
#include "../common/outbound.h"

#include "fe-kde.h"
#include "mainwindow.h"
#include "helperwidgets.h"
#include "containerview.h"

MainView::MainView(QWidget * parent, session * sess)
:  MainViewIface("MainView"),  ContainerView(parent,"MainView")
{
  Form1Layout = new QVBoxLayout(this, 1, 1, "Form1Layout");

    layout1 = new QHBoxLayout(0, 1, 1, "layout1");

    setSession(sess);

	m_topicEdit = new TopicEdit(this,"topic");


  m_topicEdit->
      setSizePolicy(QSizePolicy
                    (QSizePolicy::Minimum, QSizePolicy::Maximum, false));


   m_oldTopicParaPos=0;
   m_oldTopicIndexPos=0;

layout1->addWidget(m_topicEdit);

    channelButton = new QPushButton(this, "channelButton");
    layout1->addWidget(channelButton);
    Form1Layout->addLayout(layout1);

    channelButton->hide();

    m_splitter = new QSplitter(this, "m_splitter");
    m_splitter->setOrientation(QSplitter::Horizontal);
    m_splitter->setOpaqueResize(true);


    QWidget *m_textPanel = new QWidget(m_splitter, "layout5");

    layout5 = new QVBoxLayout(m_textPanel, 1, 1, "layout5");

    m_textView = new TextView(m_textPanel, "m_textView");

    layout5->addWidget(m_textView);

    layout3 = new QHBoxLayout(0, 1, 1, "layout3");

    m_inputEdit = new InputLineEdit(m_textPanel,"input");
    layout3->addWidget(m_inputEdit);

    QPushButton *pushButton1 = new QPushButton(m_textPanel, "pushButton1");
    layout3->addWidget(pushButton1);
    layout5->addLayout(layout3);

    m_userlistPanel = new QWidget(m_splitter, "layout4");

    QVBoxLayout *layout4 = new QVBoxLayout(m_userlistPanel, 1, 1, "layout4");

    m_userList = new KListView(m_userlistPanel);
	m_userList->addColumn("");
 m_userList->addColumn ( "User");
 m_userList->addColumn ( "Host");
    m_userList->setHScrollBarMode(QScrollView::AlwaysOff);

    layout4->addWidget(m_userList);


    if(prefs.userlistbuttons) {
	QGridLayout *gridLayout = new QGridLayout(m_userlistPanel, 1, 2);
	QToolButton *n;
	struct popup *pop;
	GSList *list = button_list;
	int a = 0, b = 0;

	while(list) {
	    pop = (struct popup *)list->data;
	    if(pop->cmd[0]) {
		n = new QToolButton(m_userlistPanel);
		n->setText(pop->name);
		gridLayout->addWidget(n, b, a);
		a++;
		if(a == 2) {
		    a = 0;
		    b++;
		}
	    }
	    list = list->next;
	}
	layout4->addLayout(gridLayout);
    }

    m_userlistPanel->hide();

    Form1Layout->addWidget(m_splitter);

    channelButton->setText(i18n("&Channel"));
    pushButton1->setText(i18n("Te&xt"));

    connect(m_topicEdit, SIGNAL(returnPressed()), this, SLOT(topicEnter()));


    connect(m_inputEdit, SIGNAL(returnPressed()), this, SLOT(entryEnter()));
    connect(m_inputEdit, SIGNAL(tabPressed()), this, SLOT(entryTab()));
    connect(m_inputEdit, SIGNAL(downPressed()), this, SLOT(entryDown()));
    connect(m_inputEdit, SIGNAL(upPressed()), this, SLOT(entryUp()));

    m_splitter->setCollapsible(m_textPanel, false);
    m_splitter->setCollapsible(m_userlistPanel, true);

m_splitter->setSizes(QValueList < int >() << width()- m_userlistPanel->minimumWidth() <<  m_userlistPanel->minimumWidth());


    if(!xchatapp->dcopClient()->isRegistered()) {
	xchatapp->dcopClient()->registerAs("xchatmainview");
	xchatapp->dcopClient()->setDefaultObject(objId());
    }

}

MainView::~MainView()
{
}

MainWindow *MainView::window(){
    return m_session->gui->win;
}

void MainView::setWindow(MainWindow *w){
    kdDebug() << "mainview::set Window"<<endl;
    ContainerView::setWindow(w);
	m_session->gui->win=w;
}

void MainView::closeView()
{
	ContainerView::closeView();
	delete m_session->gui->comp;
        delete m_session->gui->cmdcomp;
        delete m_session->gui;
        m_session->gui = 0;
        kill_session_callback(m_session);
}





void MainView::topicEnter()
{
	kdDebug() << "topicEnter"<<endl;
	
    if(m_session)
	{
		m_topicEdit->getCursorPosition(&m_oldTopicParaPos, &m_oldTopicIndexPos);
		kdDebug() << "topic: |"<<unescapeText(m_topicEdit->text()).latin1()<<"|"<<endl;
		set_topic(m_session, (char*)unescapeText(m_topicEdit->text()).latin1());
	}
}

void MainView::setTopic(QString t)
{
	kdDebug() << "setting topic: "<<t<<endl;
    m_topicEdit->setTopicText(t);
	m_topicEdit->setCursorPosition(m_oldTopicParaPos, m_oldTopicIndexPos);
	m_oldTopicParaPos=0;
	m_oldTopicIndexPos=0;
}

void MainView::giveInputFocus()
{
    m_inputEdit->setFocus();
    //topicStack->raiseWidget(1);
    //topicStack->update();
}

void MainView::entryDown()
{
    char *c = history_down(&m_session->history);

    if(c)
	m_inputEdit->setText(c);
}

void MainView::entryUp()
{
    char *c = history_up(&m_session->history, (char*)m_inputEdit->text().ascii());

    if(c)
	m_inputEdit->setText(c);
}

void MainView::entryEnter()
{
    if(!m_inputEdit->text().isEmpty()) {
	QString d = m_inputEdit->text();

	m_inputEdit->clear();
	handle_multiline(m_session, (char*)d.latin1(), 1, 0);
    }
}

void MainView::entryTab()
{
    QString t = m_inputEdit->text();
    int pos = m_inputEdit->cursorPosition();
    int firstSpace = t.find(' ');

    kdDebug() << "pos==" << pos << "  firstSpace==" << firstSpace << endl;
    if((t[0] == '/') && ((firstSpace == -1) || (pos <= firstSpace))) {
	if(pos != firstSpace)
	    pos--;
	kdDebug() << "do command comp of: " << t.mid(1, pos) << endl;
	QString n = m_session->gui->cmdcomp->makeCompletion(t.mid(1, pos));

	kdDebug() << "comp found:  " << n << endl;
	if(n.isEmpty()) {
	    //handle_multiline(m_session, "/help", 0, 0);
	    return;
	}
	if(m_session->gui->cmdcomp->hasMultipleMatches()) {
	    kdDebug() << "mult matches: n=" << n << QString("/%1%2").arg(n).arg(t.mid(firstSpace)) << endl;
	    m_inputEdit->setText(QString("/%1%2").arg(n).arg(t.mid(firstSpace)));
	} else {
	    m_inputEdit->setText(QString("/%1%2 ").arg(n).arg(t.mid(firstSpace)));
	}
    } else {
	int lastSpace = t.findRev(' ', pos - 1);
	int start = (lastSpace == -1) ? 0 : lastSpace + 1;
	QString n = m_session->gui->comp->makeCompletion(t.mid(start, pos - start));

	if(n.isEmpty())
	    return;
	if(!m_session->gui->comp->hasMultipleMatches() && ((firstSpace == -1) || (pos <= firstSpace)))
	    m_inputEdit->setText(n + ": " + t.mid(pos));
	else {
	    m_inputEdit->setText(t.left(start) + n + t.mid(pos));
	    m_inputEdit->setCursorPosition(start + n.length());
	}
    }
}

#if 0
void aaa()
{
    QString t = m_inputEdit->text();
    int pos = m_inputEdit->cursorPosition();
    int firstSpace = t.find(' ');

    firstSpace = (firstSpace == -1) ? 30000 : firstSpace;	// quick kludge
    int lastSpace = t.findRev(' ', pos - 1);
    int start = (lastSpace == -1) ? 0 : lastSpace + 1;
    QString n;

    if((t[0] == '/') && (pos < firstSpace)) {
	kdDebug() << "comping " << t.mid(start + 1, pos) << endl;
	n = m_session->gui->cmdcomp->makeCompletion(t.mid(start + 1, pos));
    } else {
	n = m_session->gui->comp->makeCompletion(t.mid(start, pos - start));
    }
    kdDebug() << "start=" << start << " pos=" << pos << " firstSpace=" << firstSpace << " lastSpace=" << lastSpace << " tocomp=" << n << endl;
    if(n.isEmpty()) {
	return;
    }
    if(pos > firstSpace) {
	if(!m_session->gui->comp->hasMultipleMatches())
	    m_inputEdit->setText(n + ": " + t.mid(pos));
	else
	    m_inputEdit->setText(n + t.mid(pos));
    } else {
	m_inputEdit->setText(t.left(start) + n + t.mid(pos));
    }
}
#endif

void MainView::completionMatches(const QStringList & matchlist)
{
    if(matchlist.count() > 1) {
	QString a = matchlist.join(" ");

	m_textView->appendText(a, false, true);
    }
}

void MainView::clearUserlist()
{
    m_userList->clear();
}

void MainView::showUserlist()
{
    if(m_userlistPanel->isHidden())
        m_userlistPanel->show();
    
}

void MainView::hideUserlist()
{
    if(!m_userlistPanel->isHidden())
            m_userlistPanel->hide();
}

void MainView::insertUser(server * s, User * u, int index, int sel)
{
showUserlist();
    UserlistItem *i=new UserlistItem(m_userList, xchatapp->getUserIcon(s, u), u);
	i->setUser(u);
}

bool MainView::removeUser(User * u)
{
    bool wasSelected = false;
	UserlistItem *i = (UserlistItem *) m_userList->firstChild();

	while( i ) {
	if(i->getUser() == u) {
	    if(i->isSelected ())
		wasSelected = true;
	    delete i;
	    break;
	}
	i = (UserlistItem *)i->nextSibling();
    }
    return wasSelected;

}

void MainView::rehashUser(User *u)
{
UserlistItem *i = (UserlistItem *) m_userList->firstChild();
while( i ) {
if (i->getUser() == u)
{
i->setUser(u);
	return;
}
i = (UserlistItem *)i->nextSibling();
}
}
