#include "mainwindow.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qapplication.h>
#include <qdragobject.h>
#include <qpushbutton.h>
#include "tabwidget.h"
#include <qsplitter.h>
#include <qwidgetstack.h>
#include <qwidget.h>
#include <qlistbox.h>
#include <qgrid.h>
#include <kdebug.h>
#include <sys/time.h>
#include <qtoolbutton.h>
#include <kapplication.h>
#include <kmainwindow.h>
#include <kmessagebox.h>
#include <kiconloader.h>
#include <kstatusbar.h>
#include <kconfig.h>
#include <kmenubar.h>
#include <kkeydialog.h>
#include <kaction.h>
#include <kstdaction.h>
#include <klocale.h>
#include <kedittoolbar.h>
#include <kcharselect.h>
#include <kprogress.h>

#include <kdcopactionproxy.h>


#include "../common/xchat.h"
#include "../common/xchatc.h"

#include "../common/fe.h"
#include "tabwidget.h"
#include "fe-kde.h"
#include "app.h"
#include "containerview.h"
#include "chanlist.h"
#include "rawlog.h"
#include "serverlist.h"
#include "configure.h"
#include "extraview.h"


MainWindow::MainWindow(QWidget * parent)
:  KMainWindow(parent, "MainWindow")
{

//m_statusBarWidget= new StatusBarWidget(statusBar(), "Progress Bar");
//statusBar()->addWidget(m_statusBarWidget);
	m_progressBar=new KProgress(statusBar(),"progressBar");
m_progressBar->setFormat("Connecting...");
m_progressBar->setTotalSteps(5);
m_progressBar->setMaximumHeight(m_progressBar->fontMetrics().lineSpacing());
//m_progressBar->setAlignment( AlignHCenter | AlignVCenter );
	statusBar()->addWidget(m_progressBar, 0, false);
m_progressBar->hide();
m_timer = new QTimer(this);
connect(m_timer, SIGNAL(timeout()), this, SLOT(forwardLoop()));
    /*nameFrame = new QFrame(this, "nameFrame");
    nameFrame->setFrameShape(QFrame::StyledPanel);
    nameFrame->setFrameShadow(QFrame::Sunken);
    nameFrameLayout = new QHBoxLayout(nameFrame, 2, 4, "nameFrameLayout");*/

    //nameLabel = new QLabel(this, "nameLabel");

   // nameFrameLayout->addWidget(nameLabel);
    statusBar()->insertItem("Name: ", 1,0, true);
	//statusBar()->addWidget(nameLabel, 0, true);

    //lagLabel = new QLabel(this, "lagLabel");
    //statusBar()->addWidget(lagLabel, 0, true);

    //numbersLabel = new QLabel(this, "numbers");
    //statusBar()->addWidget(numbersLabel, 0, true);
statusBar()->insertItem("-1 ms", 2,0, true);
statusBar()->insertItem("0 ops 0 users", 3,0, true);


    KIconLoader *loader = KGlobal::iconLoader();

new KAction(i18n("Switch to Tab 1"), 0, ALT + Key_1, this, SLOT(slotTab1()), actionCollection(), "switchtab1");
new KAction(i18n("Switch to Tab 2"), 0, ALT + Key_2, this, SLOT(slotTab2()), actionCollection(), "switchtab2");
new KAction(i18n("Switch to Tab 3"), 0, ALT + Key_3, this, SLOT(slotTab3()), actionCollection(), "switchtab3");
new KAction(i18n("Switch to Tab 4"), 0, ALT + Key_4, this, SLOT(slotTab4()), actionCollection(), "switchtab4");
new KAction(i18n("Switch to Tab 5"), 0, ALT + Key_5, this, SLOT(slotTab5()), actionCollection(), "switchtab5");
new KAction(i18n("Switch to Tab 6"), 0, ALT + Key_6, this, SLOT(slotTab6()), actionCollection(), "switchtab6");
new KAction(i18n("Switch to Tab 7"), 0, ALT + Key_7, this, SLOT(slotTab7()), actionCollection(), "switchtab7");
new KAction(i18n("Switch to Tab 8"), 0, ALT + Key_8, this, SLOT(slotTab8()), actionCollection(), "switchtab8");
new KAction(i18n("Switch to Tab 9"), 0, ALT + Key_9, this, SLOT(slotTab9()), actionCollection(), "switchtab9");
new KAction(i18n("Switch to Tab 10"), 0, ALT + Key_0, this, SLOT(slotTab10()), actionCollection(), "switchtab10");

new KAction(i18n("Next Tab"), 0, CTRL + Key_PageUp, this, SLOT(slotNextTab()), actionCollection(), "nexttab");

new KAction(i18n("Previous Tab"), 0, CTRL + Key_PageDown, this, SLOT(slotPreviousTab()), actionCollection(), "prevtab");


    new KAction(i18n("Show &Server List..."), "bookmark_folder", CTRL + Key_L, this, SLOT(slotServerList()), actionCollection(), "serverlist");

    KActionMenu *newMenu = new KActionMenu(i18n("New"),
	"filenew",
	actionCollection(), "new");

    KAction *a = new KAction(i18n("&Channel Tab"),
	"tab_new",
	CTRL + Key_T, this, SLOT(slotNewChannelTab()),
	actionCollection(), "new_channeltab");

    //if (bIsDetached)
//      a->setEnabled(false);

    newMenu->insert(a);

    a = new KAction(i18n("&Server Tab"), 0, this, SLOT(slotNewServerTab()), actionCollection(), "new_servertab");

    //   if (bIsDetached)
//      a->setEnabled(false);

    newMenu->insert(a);

    newMenu->insert(new KAction(i18n("&Channel Window"), "window_new", CTRL + Key_N, this, SLOT(slotNewChannelWindow()), actionCollection(), "new_channelwin"));

    newMenu->insert(new KAction(i18n("&Server Window"), 0, this, SLOT(slotNewServerWindow()), actionCollection(), "new_serverwin"));

    new KAction(i18n("&Load Extention..."), 0, this, SLOT(slotLoadExtention()), actionCollection(), "load_plugin_script");

    new KAction(i18n("&Detach Tab"), "tab_breakoff", CTRL + Key_I, this, SLOT(slotDetachTab()), actionCollection(), "detach_tab");

    new KAction(i18n("&Close Tab"), "tab_remove", CTRL + Key_W, this, SLOT(slotCloseTab()), actionCollection(), "close_tab");

    new KAction(i18n("Show &Channel List"), 0, this, SLOT(slotChanlist()), actionCollection(), "chanlist");

    new KAction(i18n("Show &Character Chart"),  "char", 0, this, SLOT(slotCharChart()), actionCollection(), "charchart");

    new KAction(i18n("Show &URL Grabber"), 0, this, SLOT(slotURLGrabber()), actionCollection(), "urlgrabber");

    new KAction(i18n("Show &Chat List"), 0, this, SLOT(slotChatList()), actionCollection(), "chats");

    new KAction(i18n("Show &File Transfers"), "queue", 0, this, SLOT(slotTransfers()), actionCollection(), "xfer");

    new KAction(i18n("Show &Notify List"), 0, this, SLOT(slotNotifyList()), actionCollection(), "notifylist");

    new KAction(i18n("Show &Ignore List"), 0, this, SLOT(slotIgnoreList()), actionCollection(), "ignorelist");

    new KAction(i18n("Show &Ban List"), 0, this, SLOT(slotBanlist()), actionCollection(), "banlist");
    
    new KAction(i18n("Show &Rawlog"), "output_win", 0, this, SLOT(slotRawlog()), actionCollection(), "rawlog");

    KStdAction::quit(this, SLOT(slotQuit()), actionCollection());

    KStdAction::cut(this, SLOT(slotConfigureKeys()), actionCollection());

    KStdAction::copy(this, SLOT(slotConfigureKeys()), actionCollection());

    KStdAction::paste(this, SLOT(slotConfigureKeys()), actionCollection());

    KToggleAction *mbarAction = KStdAction::showMenubar(this, SLOT(slotMenubar()),
	actionCollection());
    KToggleAction *tbarAction = KStdAction::showToolbar(this, SLOT(slotToolbar()),
	actionCollection());
    KToggleAction *sbarAction = KStdAction::showStatusbar(this, SLOT(slotStatusbar()),
	actionCollection());

    KStdAction::keyBindings(this, SLOT(slotConfigureKeys()), actionCollection());
    KStdAction::preferences(this, SLOT(slotPreferences()), actionCollection());
    KStdAction::configureToolbars(this, SLOT(slotConfigureToolbars()), actionCollection());
    KStdAction::configureNotifications(this, SLOT(slotConfigureNotifications()), actionCollection());
 // setXMLFile("vertigo.rc");
    createGUI("vertigo.rc");

    KConfig *config = KGlobal::config();

    config->setGroup("Window");

    if(config->readBoolEntry("MenubarVisible", true))
	menuBar()->show();
    else {
	mbarAction->setChecked(false);
	menuBar()->hide();
    }
    if(config->readBoolEntry("ToolbarVisible", true))
	toolBar()->show();
    else {
	tbarAction->setChecked(false);
	toolBar()->hide();

    }
    if(config->readBoolEntry("StatusbarVisible", false))
	statusBar()->show();
    else {
	sbarAction->setChecked(false);
	statusBar()->hide();
    }



    resize(prefs.mainwindow_width, prefs.mainwindow_height);
    move(prefs.mainwindow_left, prefs.mainwindow_top);

    MainWindowLayout = new QVBoxLayout(this, 12, 6, "MainWindowLayout");

    m_tabWidget = new TabWidget(this, "m_tabWidget");
    m_tabWidget->setTabPosition(QTabWidget::Top);

    QPushButton *tabRemove = new QPushButton(m_tabWidget);

    tabRemove->setIconSet(loader->loadIconSet("tab_remove", KIcon::Small));
    int s = tabRemove->sizeHint().height();

    tabRemove->setFixedSize(s, s);
    connect(tabRemove, SIGNAL(clicked()), this, SLOT(slotCloseTab()));

    QPushButton *tabNew = new QPushButton(m_tabWidget);

    tabNew->setIconSet(loader->loadIconSet("tab_new", KIcon::Small));
    connect(tabNew, SIGNAL(clicked()), this, SLOT(slotNewChannelTab()));

    s = tabNew->sizeHint().height();
    tabNew->setFixedSize(s, s);

    m_tabWidget->setCornerWidget(tabRemove, Qt::TopRight);
    m_tabWidget->setCornerWidget(tabNew, Qt::TopLeft);
    tabRemove->hide();
    tabNew->hide();
    connect(m_tabWidget, SIGNAL(currentChanged(QWidget *)), this, SLOT(slotTabChanged(QWidget *)));

   MainWindowLayout->addWidget(m_tabWidget);
 setCentralWidget(m_tabWidget);


    m_mainViewList = new QPtrList < MainView > ();


    
}
MainView *MainWindow::getMainView(session * sess)
{
    MainView *v;


 v = new MainView(this, sess);
    m_mainViewList->append(v);


    int lastTab = -1;
    session *s;
MainView *ov=v;

    for(v = m_mainViewList->first(); v; v = m_mainViewList->next()) {
	s = v->getSession();
	if(s && (s->server == sess->server)) {
	    lastTab = QMAX(m_tabWidget->indexOf(v) + 1, lastTab);
	}
    }

v=ov;
    m_tabWidget->insertTab(v, "(none)", lastTab);

	kdDebug() << "added view" << endl;
   	 return v;

}

MainWindow::~MainWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

void MainWindow::resizeEvent(QResizeEvent * r)
{
    /*KConfig *config = KGlobal::config();

       config->setGroup("Geometry");
       config->writeEntry("WindowSize", r->size());
       config->sync(); */
    kdDebug() << "mainwidow_width" << prefs.mainwindow_width << endl;
    kdDebug() << "mainwidow_height" << prefs.mainwindow_height << endl;
    prefs.mainwindow_width = r->size().width();
    prefs.mainwindow_height = r->size().height();
    prefs.mainwindow_left = x();
    prefs.mainwindow_top = y();
}

void MainWindow::slotTabChanged(QWidget * t)
{
    kdDebug() << "slotTabChanged" << endl;
    if(!t)
	return;

    ContainerView *cv=(ContainerView*)t;
    cv->setActiveView();
    
    if(t->isA("MainView")) {
	/* TODO: move to MainView::setActiveChange */ 
	MainView *v = (MainView *) t;

	current_sess = v->getSession();
	current_tab = current_sess;
	if(!current_sess->gui)
	    return;
	fe_set_title(current_sess);
	if(current_sess) {
	    setLag(current_sess->gui->lag);
	   setNumbers(current_sess->ops, current_sess->total); 
	    if(current_sess->gui->showbar) {
		showProgressBar();
	    }
	} else {
	    hideProgressBar();
	}
	if(current_sess->new_data || current_sess->nick_said || current_sess->msg_said) {
	    current_sess->nick_said = 0;
	    current_sess->msg_said = 0;
	    current_sess->new_data = 0;
	    m_tabWidget->setTabColor(t, Qt::black);
	}
    }
}

void MainWindow::showProgressBar()
{
    kdDebug() << "showProgressBar" << endl;
    //static int did=0;
	//if (did)
	//	return;
	if(!statusBar()->isHidden()) {
	m_progressBar->reset();
	m_timer->start(500, false);
	m_progressBar->show();
//statusBar()->addWidget(m_progressBar, 0, false);
//did =1;
	//statusBar()->message("Connecting...");
	//m_statusBarWidget->messageLabel->setText("Connected...");
	//m_progressBar->reset();
    }
}

void MainWindow::hideProgressBar()
{
    kdDebug() << "hideProgressBar" << endl;
    if(!statusBar()->isHidden()) {
	//statusBar()->clear();
//statusBar()->removeWidget(m_progressBar);
//      m_statusBarWidget->messageLabel->setText(QString::null);
	m_progressBar->hide();
	m_timer->stop();
    }
}

void MainWindow::forwardLoop()
{
    m_progressBar->setProgress(m_progressBar->progress() + 1);
    if(m_progressBar->progress() > m_progressBar->totalSteps()) {
   		m_progressBar->reset(); 
    }
}




void MainWindow::setNumbers(int ops, int total)
{
    kdDebug() << "setNumbers" << ops << " "<<total << endl;
    QString text = "%1 %2, %3 %4";

    text = text.arg(ops).arg(i18n("ops")).arg(total).arg(i18n("total"));
    statusBar()->changeItem(text, 3);
	//numbersLabel->setText(text);

}


void MainWindow::setLag(int lag)
{
    kdDebug() << "setLag" << lag << endl;
    QString text = "%1 %2";

    text = text.arg(lag).arg(i18n("ms"));
   statusBar()->changeItem(text, 2); 
	//lagLabel->setText(text);

}

void MainWindow::slotServerList()
{
    fe_serverlist_open(current_sess);
}

void MainWindow::slotNewServerWindow()
{
    int old = prefs.tabchannels;

    prefs.tabchannels = 0;
    new_ircwindow(NULL, NULL, SESS_SERVER);
    prefs.tabchannels = old;
}

void MainWindow::slotNewChannelWindow()
{
    int old = prefs.tabchannels;

    prefs.tabchannels = 0;
    new_ircwindow(current_sess->server, NULL, SESS_CHANNEL);
    prefs.tabchannels = old;
}

void MainWindow::slotNewServerTab()
{
    int old = prefs.tabchannels;

    prefs.tabchannels = 1;
    new_ircwindow(NULL, NULL, SESS_SERVER);
    prefs.tabchannels = old;
}

void MainWindow::slotNewChannelTab()
{
    int old = prefs.tabchannels;

    prefs.tabchannels = 1;
    new_ircwindow(current_sess->server, NULL, SESS_CHANNEL);
    prefs.tabchannels = old;
}

void MainWindow::slotDetachTab()
{

    QWidget *t = m_tabWidget->currentPage();

    if(viewCount() != 1) {
	MainWindow *w = xchatapp->createNewWindow();
	QWidget *t = m_tabWidget->currentPage();

    ContainerView *v=(ContainerView *)t;
    v->setWindow(w);

	QString label = m_tabWidget->tabLabel(t);
	t->disconnect(this);
	t->reparent(w, QPoint(0, 0), false);
	w->addView(v, label, -1);
    }
}


void MainWindow::closeView(ContainerView *v)
{
	v->closeView();

	if(!m_tabWidget->count()) {
	    close();
	}

	 if(v->isA("MainView")) {
		m_mainViewList->remove((MainView*)v);
    	}
}

void MainWindow::slotCloseTab()
{
    	QWidget *t = m_tabWidget->currentPage();
    	closeView((ContainerView *)t);
}

void MainWindow::slotQuit()
{
    hide();
    kapp->quit();
}

int MainWindow::lastSameServerTabIndex()
{
int lastTab = -1;
session *s;
MainView *v;
for(v = m_mainViewList->first(); v; v = m_mainViewList->next()) {
s = v->getSession();
if(s && (s->server == current_sess->server)) {
lastTab = QMAX(m_tabWidget->indexOf(v) + 1, lastTab);
   }
        }
return lastTab;
}

void MainWindow::setViewLabel(ContainerView *l, QString label)
{
		m_tabWidget->setTabLabel(l, label);
}


void MainWindow::setViewToolTip(ContainerView *l, QString label)
{
		m_tabWidget->setTabToolTip(l, label);
}

void MainWindow::setViewColor(ContainerView *l, QColor c)
{
		m_tabWidget->setTabColor(l, c);
}


void MainWindow::addView(ContainerView *l, QString name, int index)
{
	m_tabWidget->insertTab(l, name, index);
}


void MainWindow::slotChanlist()
{
if(!current_sess->server->gui->chanlist) {
        ChanlistView *l = new ChanlistView(this, current_sess->server);

        current_sess->server->gui->chanlist = l;
        addView(l, "channels", lastSameServerTabIndex());
    } else {
        current_sess->server->gui->chanlist->showView();
    }
}

void MainWindow::slotURLGrabber()
{
if(!xchatapp->urlGrabber()) {
        URLGrabberView *l = new URLGrabberView(this, this);
        xchatapp->setUrlGrabber(l);
        addView(l, "urls", -1);
    } else {
        xchatapp->urlGrabber()->showView();
    }
}

void MainWindow::slotChatList()
{
if(!xchatapp->chatList()) {
        ChatListView *l = new ChatListView(this, this);
        xchatapp->setChatList(l);
        addView(l, "chats", -1);
    } else {
        xchatapp->chatList()->showView();
    }
}

void MainWindow::slotTransfers()
{
if(!xchatapp->transfers()) {
        XferView *l = new XferView(this, this);
        xchatapp->setTransfers(l);
        addView(l, "transfers", -1);
    } else {
        xchatapp->transfers()->showView();
    }
}

void MainWindow::slotNotifyList()
{
if(!xchatapp->notifyList()) {
        NotifyListView *l = new NotifyListView(this, this);
        xchatapp->setNotifyList(l);
       addView(l, "notify", -1);
    } else {
        xchatapp->notifyList()->showView();
    }
}

void MainWindow::slotIgnoreList()
{
	/*
if(!xchatapp->ignoreList()) {
        IgnoreListView *l = new IgnoreListView(this, this);
        xchatapp->setIgnoreList(l);
        m_tabWidget->insertTab(l, "ignores", -1);
    } else {
        xchatapp->ignoreList()->showView();
    }*/
}

void MainWindow::slotBanlist()
{
if(!xchatapp->banList()) {
        BanListView *l = new BanListView(this, this);
        xchatapp->setBanList(l);
        addView(l, "bans", -1);
    } else {
        xchatapp->banList()->showView();
    }
}



void MainWindow::slotCharChart()
{
	 KCharSelect *l = xchatapp->charChart();
	 
       l->show();
        l->raise();
        l->setActiveWindow();

}


void MainWindow::slotRawlog()
{
    if(!current_sess->server->gui->rawlog) {
	RawlogView *l = new RawlogView(this, current_sess->server);

	current_sess->server->gui->rawlog = l;
	addView(l, "rawlog", lastSameServerTabIndex());
    } else {
	current_sess->server->gui->rawlog->window()->showView(current_sess->server->gui->rawlog);
    }

}

void MainWindow::slotMenubar()
{
    if(menuBar()->isVisible())
	menuBar()->hide();
    else
	menuBar()->show();
    KConfig *config = KGlobal::config();

    config->setGroup("Window");
    config->writeEntry("MenubarVisible", menuBar()->isVisible());
    config->sync();
}

void MainWindow::slotToolbar()
{
    if(toolBar()->isVisible())
	toolBar()->hide();
    else
	toolBar()->show();
    KConfig *config = KGlobal::config();

    config->setGroup("Window");
    config->writeEntry("ToolbarVisible", toolBar()->isVisible());
    config->sync();
}

void MainWindow::slotStatusbar()
{
    if(statusBar()->isVisible())
	statusBar()->hide();
    else
	statusBar()->show();
    KConfig *config = KGlobal::config();

    config->setGroup("Window");
    config->writeEntry("StatusbarVisible", statusBar()->isVisible());
    config->sync();
}




void MainWindow::slotConfigureKeys()
{
    KKeyDialog::configure(actionCollection(), this);
}

void MainWindow::slotConfigureToolbars()
{
    KEditToolbar edit(factory());

    //connect(&edit, SIGNAL( newToolbarConfig() ), this, SLOT( slotNewToolbarCofig() ));
    edit.exec();
}

void MainWindow::slotPreferences()
{
    ConfigureDialog *n = new ConfigureDialog(this);

    n->show();
}

void MainWindow::slotTab10()
{
	m_tabWidget->setCurrentPage(10);
}

void MainWindow::slotTab1()
{
    m_tabWidget->setCurrentPage(0);
}

void MainWindow::slotTab2()
{
    m_tabWidget->setCurrentPage(1);
}

void MainWindow::slotTab3()
{
    m_tabWidget->setCurrentPage(2);
}

void MainWindow::slotTab4()
{
    m_tabWidget->setCurrentPage(3);
}

void MainWindow::slotTab5()
{
    m_tabWidget->setCurrentPage(4);
}

void MainWindow::slotTab6()
{
    m_tabWidget->setCurrentPage(5);
}

void MainWindow::slotTab7()
{
    m_tabWidget->setCurrentPage(6);
}

void MainWindow::slotTab8()
{
    m_tabWidget->setCurrentPage(7);
}

void MainWindow::slotTab9()
{
    m_tabWidget->setCurrentPage(8);
}





void MainWindow::slotNextTab()
{
	int i=m_tabWidget->currentPageIndex()+1;
	if (i== m_tabWidget->count())
		i=0;
	m_tabWidget->setCurrentPage(i);
}


void MainWindow::slotPreviousTab()
{
	int i=m_tabWidget->currentPageIndex()-1;
	if (i==-1)
		i=m_tabWidget->count()-1;
	m_tabWidget->setCurrentPage(i);
}



void MainWindow::showView(ContainerView *v)
{
	m_tabWidget->showPage(v);
}

int MainWindow::viewCount()
{
    return m_tabWidget->count();
}

void MainWindow::focusInEvent(QFocusEvent *)
{
    kdDebug() << "focusinwindow" << endl;
    slotTabChanged(m_tabWidget->currentPage());
}

void MainWindow::windowActivationChange(bool oldActive)
{
    kdDebug() << caption() << "::windowActivationChange :: " << oldActive << endl;
    if(!oldActive) {
	slotTabChanged(m_tabWidget->currentPage());
    }
}

bool MainWindow::isCurrent(session *sess)
{
	    QWidget *t = m_tabWidget->currentPage();

		    if(t&&t->isA("MainView")) {
			    MainView *m = (MainView *) t;
				if (m->getSession()==sess)
				{
					return true;
				}
				}
				return false;
}

void MainWindow::closeEvent(QCloseEvent * e)
{

    kdDebug() << "vc=" << viewCount() << " wc=" << xchatapp->windowCount() << endl;

    if(viewCount() > 1) {
	KConfig *config = KGlobal::config();
	KConfigGroupSaver cs(config, QString::fromLatin1("Notification Messages"));

	if(!config->hasKey("MultipleTabConfirm")) {
	    if(KMessageBox::warningContinueCancel(this, i18n("You have multiple tabs open in this window, are you sure you wish to close it?"), i18n("Confirmation"), i18n("&Close"), "MultipleTabConfirm") == KMessageBox::No) {
		e->ignore();
		return;
	    }
	}
    }

/*    else if ((viewCount() == 1) && (xchatapp->windowCount() == 1)
	     ) {
	KConfig *config = KGlobal::config();
	KConfigGroupSaver cs(config,
			     QString::fromLatin1("Notification Messages"));

	if (!config->hasKey("SingleTabConfirm")) {
	    if (KMessageBox::
		warningContinueCancel(this,
				      i18n
				      ("You have a single vertigo window open, closing it may quit vertigo. Are you sure you want to close it?"),
				      i18n("Confirmation"), i18n("&Quit"),
				      "SingleTabConfirm") ==
		KMessageBox::No) {
		e->ignore();
		return;
	    }
	}
    }
*/
    xchatapp->removeMainWindow();
    hide();
    xchatapp->flushX();
    KMainWindow::closeEvent(e);
}


