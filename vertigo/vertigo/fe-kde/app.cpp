// kde includes
#include <dcopclient.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>
#include <kpassivepopup.h>
#include <kaboutdata.h>

// qt includes
#include <qvbox.h>
#include <qcolor.h>

// system
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// app includes
#include "app.h"
#include "fe-kde.h"
#include "mainwindow.h"
#include "serverlist.h"
#include "textwidget.h"


// backend includes
#include "../common/xchat.h"
#include "../common/xchatc.h"
#include "../common/fe.h"
#include "../common/cfgfiles.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "fe-kde.h"
#include "mainwindow.h"
#include "serverlist.h"
#include "textwidget.h"

App::App():KApplication()
{

	KGlobal::locale()->insertCatalogue("xchat-common");
    loadUserIcons();

    m_idleList = new QPtrList < TimerData > ();
    m_timeoutList = new QPtrList < TimerData > ();
	m_socketList = new QPtrList < SocketData > ();

    m_idleList->setAutoDelete(true);
    m_timeoutList->setAutoDelete(true);
    m_socketList->setAutoDelete(true);

	m_nextSocketID=1;

    m_mainWindowCount = 0;
    m_otherWindowCount = 0;
    
	// global widgets
	m_serverList = 0;
    m_charChart=0;
	m_banList=0;
	m_ignoreList=0;
	m_notifyList=0;
	m_transfers=0;
	m_chatList=0;
	m_urlGrabber=0;

	// color palette
    m_palette.resize(23);
    m_palette.setAutoDelete(true);
    loadDefaultPalette();
    loadPalette();
}

MainWindow *App::createNewWindow()
{
    MainWindow *win = new MainWindow(0);

    m_mainWindowCount++;
    win->show();
    return win;
}

MainWindow *App::getMainWindow(bool forceWindow)
{
    if(!m_mainWindowCount || forceWindow) {
		return createNewWindow();
    }
    return current_sess->gui->win;
}

Serverlist *App::serverList()
{
    if(!m_serverList)
		m_serverList = new Serverlist(0);
    return m_serverList;
}

KCharSelect *App::charChart()
{
	if(!m_charChart)
	{
		m_charChart= new KCharSelect(0,"Character Chart");
		m_charChart->enableFontCombo (false);
	}
	return m_charChart;
}

URLGrabberView *App::urlGrabber()
{
	return m_urlGrabber;
}

ChatListView *App::chatList()
{
	return m_chatList;
}

XferView *App::transfers()
{
	return m_transfers;
}

NotifyListView *App::notifyList()
{
	return m_notifyList;
}

IgnoreListView *App::ignoreList()
{
	return m_ignoreList;
}

BanListView *App::banList()
{
	return m_banList;
}

void App::setUrlGrabber(URLGrabberView* v)
{
	m_urlGrabber=v;
}

void App::setChatList(ChatListView*v)
{
	m_chatList=v;
}

void App::setTransfers(XferView*v)
{
	m_transfers=v;
}

void App::setNotifyList(NotifyListView* nl)
{
	m_notifyList=nl;
}

void App::setBanList(BanListView* v)
{
	m_banList=v;
}

void App::removeServerlistWindow()
{
    m_serverList = 0;
    m_otherWindowCount--;
}


void App::removeMainWindow()
{
    m_mainWindowCount--;
}

//! Return number of windows we have open.

int App::windowCount()
{
    return m_mainWindowCount + m_otherWindowCount;
}

//! Load palette from default colors.

void App::loadDefaultPalette()
{
    m_palette.insert(0, new QColor(207, 207, 207));
    m_palette.insert(1, new QColor(0, 0, 0));
    m_palette.insert(2, new QColor(0, 0, 204));
    m_palette.insert(3, new QColor(0, 204, 0));
    m_palette.insert(4, new QColor(221, 0, 0));
    m_palette.insert(5, new QColor(170, 0, 0));
    m_palette.insert(6, new QColor(187, 0, 187));
    m_palette.insert(7, new QColor(255, 170, 0));
    m_palette.insert(8, new QColor(238, 221, 34));
    m_palette.insert(9, new QColor(51, 222, 85));
    m_palette.insert(10, new QColor(0, 204, 204));
    m_palette.insert(11, new QColor(51, 238, 255));
    m_palette.insert(12, new QColor(0, 0, 255));
    m_palette.insert(13, new QColor(238, 34, 238));
    m_palette.insert(14, new QColor(119, 119, 119));
    m_palette.insert(15, new QColor(153, 153, 153));
    m_palette.insert(16, new QColor(164, 223, 255));
    m_palette.insert(17, new QColor(0, 0, 0));
    m_palette.insert(18, new QColor(223, 223, 223));
    m_palette.insert(19, new QColor(0, 0, 0));
    m_palette.insert(20, new QColor(140, 16, 16));
    m_palette.insert(21, new QColor(0, 0, 255));
    m_palette.insert(22, new QColor(245, 0, 0));
}


//! Return global color palette.

Palette App::palette()
{
    return m_palette;
}

//! Load xchat palette from disk.

void App::loadPalette()
{
    int i, l, fh, res;
    char prefname[256];
    struct stat st;
    char *cfg;
    unsigned long red, green, blue;

    snprintf(prefname, sizeof(prefname), "%s/palette.conf", get_xdir());
    fh = open(prefname, O_RDONLY | OFLAGS);
    if(fh != -1) {
	fstat(fh, &st);
	cfg = (char*)malloc(st.st_size + 1);
	if(cfg) {
	    cfg[0] = '\0';
	    l = read(fh, cfg, st.st_size);
	    if(l >= 0)
		cfg[l] = '\0';

	    for(i = 0; i < 23; i++) {
		snprintf(prefname, sizeof prefname, "color_%d_red", i);
		red = cfg_get_int(cfg, prefname);

		snprintf(prefname, sizeof prefname, "color_%d_grn", i);
		green = cfg_get_int(cfg, prefname);

		snprintf(prefname, sizeof prefname, "color_%d_blu", i);
		blue = cfg_get_int_with_result(cfg, prefname, &res);

		if(res) {
		    m_palette[i]->setRgb(red >> 8, green >> 8, blue >> 8);
		}
	    }
	    free(cfg);
	}
	close(fh);
    }

}

//! Load userlist icons from disk.

void App::loadUserIcons()
{
    QString dir = KGlobal::dirs()->findResource("data", "vertigo/pics/");

    m_opPix = new QPixmap(dir + "op.png");
    m_hopPix = new QPixmap(dir + "hop.png");
    m_voicePix = new QPixmap(dir + "voice.png");
    m_redPix = new QPixmap(dir + "red.png");
    m_purplePix = new QPixmap(dir + "purple.png");
}

//! Return the correct icon for a given user on a particular server.

QPixmap *App::getUserIcon(server * serv, User * user)
{
    char *pre;
    int level;

    if(!user)
	return 0;

    /* these ones are hardcoded */
    switch (user->prefix[0]) {
	case 0:
	    return NULL;
	case '@':
	    return m_opPix;
	case '%':
	    return m_hopPix;
	case '+':
	    return m_voicePix;
    }

    /* find out how many levels above Op this user is */
    pre = strchr(serv->nick_prefixes, '@');
    if(pre && pre != serv->nick_prefixes) {
	pre--;
	level = 0;
	while(1) {
	    if(pre[0] == user->prefix[0]) {
		switch (level) {
		    case 0:
			return m_redPix;	/* 1 level above op */
		    case 1:
			return m_purplePix;	/* 2 levels above op */
		}
		break;		/* 3+, no icons */
	    }
	    level++;
	    if(pre == serv->nick_prefixes)
		break;
	    pre--;
	}
    }
    return 0;
}


//! Returns the user level based on what icon an user has been
//! assigned- useful for quickly comparing one user and another.

int App::getUserLevel(QPixmap *p)
{
	if (p==NULL)
		return 1;
	else if (p==m_voicePix)
		return 2;
	else if (p==m_hopPix)
		return 3;
	else if (p==m_opPix)
		return 4;
	else if (p==m_redPix)
		return 5;
	else if (p==m_purplePix)
		return 6;
	else
		return 1;
}



int App::addTimeoutFunction(int interval, void *fn, void *args)
{
    bool idle = false;

    kdDebug() << "timer int=" << interval << endl;
    TimerData *dat = new TimerData;

    if(interval == 0)
	idle = true;

    dat->func = (TimerFunc) fn;
    dat->args = args;
    dat->timer = new QTimer(this);

    if(idle)
	connect(dat->timer, SIGNAL(timeout()), this, SLOT(execIdleFunction()));
    else
	connect(dat->timer, SIGNAL(timeout()), this, SLOT(execTimeoutFunction()));

    dat->timer->start(interval, idle);

    if(idle)
	m_idleList->append(dat);
    else
	m_timeoutList->append(dat);

    return dat->timer->timerId();
}

void App::removeTimeoutFunction(int timerId)
{
    TimerData *dat;

    for(dat = m_timeoutList->first(); dat; dat = m_timeoutList->next()) {
	if(dat->timer->timerId() == timerId) {
	    delete dat->timer;

	    m_timeoutList->remove(dat);
	}
    }

}

void App::execTimeoutFunction()
{
    QTimer *t = (QTimer *) sender();

    if(!t)
        return;

    TimerData *dat;
    int ret=1;

    for(dat = m_timeoutList->first(); dat; dat = m_timeoutList->next()) {
        if(dat->timer->timerId() == t->timerId()) {
            TimerFunc fn = dat->func;
	    ret=(*fn) (dat->args);
	    if (!ret)
	    removeTimeoutFunction(t->timerId());
        }
    }
}


void App::addIdleFunction(void *fn, void *args)
{
    addTimeoutFunction(0, fn, args);
}

void App::execIdleFunction()
{
    if(m_idleList->isEmpty()) {
	kdDebug() << "!!!!!whoops!!" << endl;
	return;
    }

	
    TimerData *dat = m_idleList->first();

    for(; dat; dat = m_idleList->next()) {
        TimerFunc fn = dat->func;
		(*fn) (dat->args);
		delete dat->timer;
        m_idleList->remove(dat);
    }
}

int App::addInputFunction(int fd, int flags, void *func, void *data)
{
    SocketData *dat = new SocketData;

	kdDebug() << "=== adding input function: fd="<< fd << " flags="<<flags<<endl;

    dat->func = (SocketFunc) func;
    dat->args = data;
    dat->readn = 0;
    dat->writen = 0;
    dat->excen = 0;

    if(flags & FIA_READ) {
	kdDebug() << "      adding read socket notifier: fd="<< fd << " flags="<<flags<<endl;
	QSocketNotifier *readn = new QSocketNotifier(fd, QSocketNotifier::Read, this);
	QObject::connect(readn, SIGNAL(activated(int)), this, SLOT(socketReady(int)));

	dat->readn = readn;
    }

    if(flags & FIA_WRITE) {
	kdDebug() << "      adding write socket notifier: fd="<< fd << " flags="<<flags<<endl;
	QSocketNotifier *writen = new QSocketNotifier(fd, QSocketNotifier::Write, this);
	QObject::connect(writen, SIGNAL(activated(int)), this, SLOT(socketReady(int)));

	dat->writen = writen;
    }
    if(flags & FIA_EX) {
	QSocketNotifier *excen = new QSocketNotifier(fd, QSocketNotifier::Exception, this);
	kdDebug() << "      adding excep socket notifier: fd="<< fd << " flags="<<flags<<endl;
	QObject::connect(excen, SIGNAL(activated(int)), this, SLOT(socketReady(int)));

dat->excen = excen;
    }

    dat->id = m_nextSocketID++;
    kdDebug() << "new id=" << dat->id << endl;
    m_socketList->append(dat);
    return (dat->id);
}

void App::removeInputFunction(int id)
{
    SocketData *dat;
kdDebug() << "   ===  remove socket notifier: id="<< id <<endl;
    for(dat = m_socketList->first(); dat; dat = m_socketList->next()) {
	if(dat->id == id) {
		kdDebug() << "  found dat!"<<endl;
	    if(dat->readn)
			//dat->readn->setEnabled(false);
		delete dat->readn;

	    if(dat->writen)
			//dat->writen->setEnabled(false);
		delete dat->writen;

	    if(dat->excen)
			//dat->excen->setEnabled(false);
		delete dat->excen;

	    m_socketList->remove(dat);
	}
    }
}

void App::socketReady(int fd)
{
    QSocketNotifier *sn = (QSocketNotifier *) sender();
    int cond = 0;
	kdDebug() << "--- socketReady fd="<<fd<<endl;
    if(!sn)
	return;

    SocketData *dat;

    for(dat = m_socketList->first(); dat; dat = m_socketList->next()) {
	if(dat->readn == sn)
	    cond = 1;
	else if(dat->writen == sn)
	    cond = 2;
	else if(dat->excen == sn) {
	    cond = 4;
	    kdDebug() << "execn found!" << endl;
	}

	if(cond != 0) {
	    kdDebug() << "readReady found" << endl;
	    SocketFunc fn = dat->func;
	    (*fn) (NULL, cond, dat->args);
		
	    //kdDebug() << "return from func==" << n << endl;
	    break;

	}
	cond = 0;
    }

}

void App::getString(char *prompt, char *def, void *callback, void *ud)
{
    InputDialog *i = new InputDialog(this, prompt, def, callback, ud);

    connect(i, SIGNAL(dialogDone()), this, SLOT(dialogInputHandled()));
    i->show();
}

void App::getInt(char *prompt, int def, void *callback, void *ud)
{
    InputDialog *i = new InputDialog(this, prompt, def, callback, ud);

    connect(i, SIGNAL(dialogDone()), this, SLOT(dialogInputHandled()));
    i->show();
}

void App::dialogInputHandled()
{
    InputDialog *i = (InputDialog *) sender();

    if(i->getType() == InputDialog::StringDialog) {
	StringInputFunc fn = i->getStringFunc();

	if(fn)
	    (*fn) (i->cancelled(), (char*)i->getString().latin1(), i->getUserData());
    } else if(i->getType() == InputDialog::IntDialog) {
	IntInputFunc fn = i->getIntFunc();

	if(fn)
	    (*fn) (i->cancelled(), i->getInt(), i->getUserData());
    }
    delete i;
}

CleanupHandler::CleanupHandler()
{

}

void CleanupHandler::cleanup()
{
    //idea taken from kmail
    xchatapp->dcopClient()->suspend();
    QTimer::singleShot(0, this, SLOT(startCleanupLoop()));
    xchatapp->enter_loop();
}

void CleanupHandler::startCleanupLoop()
{
    KPassivePopup *popup = new KPassivePopup();
    QVBox *box = popup->standardView(xchatapp->aboutData()->programName(),
	QString::null, xchatapp->miniIcon());
    new QLabel("Waiting for network connections to close...", box);
  //  KProgress *m_progress = new KProgress(3, box, "cleanupProgress");
    popup->setView(box);
   // m_progress->setProgress(0);
    QApplication::syncX();
    popup->adjustSize();
    popup->show();
    xchatapp->processEvents();
    m_timer = new QTimer(box);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(forwardLoop()));
    m_timer->start(3000, false);

    
}

void CleanupHandler::forwardLoop()
{
 //   if (!m_progress)
   //       return;
  //  m_progress->setProgress(m_progress->progress() + 1);
   // if(m_progress->progress() > m_progress->totalSteps()) {
	//m_timer->stop();
	xchatapp->exit_loop();
    //}
}

InputDialog::InputDialog(QObject * parent, char *prompt, int def, void *callback, void *ud)
:QDialog()
{
    //setParent(parent);
    m_cancelled = true;
    m_type = IntDialog;

    m_intFunc = (IntInputFunc)callback;
    m_stringFunc = 0;
    m_userData = ud;

    QString num;

    num.setNum(def);
    setup(QString(prompt), num);
}

InputDialog::InputDialog(QObject * parent, char *prompt, char *def, void *callback, void *ud)
:QDialog()
{

    // setParent(parent);
    m_cancelled = true;
    m_type = StringDialog;

    m_stringFunc = (StringInputFunc)callback;
    m_intFunc = 0;
    m_userData = ud;

    setup(QString(prompt), QString(def));
}

void InputDialog::setup(QString prompt, QString def)
{
    setCaption(prompt);

    Form2Layout = new QVBoxLayout(this, 2, 1, "Form2Layout");

    layout3 = new QHBoxLayout(0, 0, 6, "layout3");

    textLabel1 = new QLabel(this, "textLabel1");
    textLabel1->setText(prompt);
    layout3->addWidget(textLabel1);
    if(m_type == IntDialog) {
	spinBox1 = new QSpinBox(this, "spinBox1");
	spinBox1->setValue(def.toInt());
	layout3->addWidget(spinBox1);
    } else {
	lineEdit1 = new QLineEdit(this, "lineEdit1");
	lineEdit1->setText(def);
	layout3->addWidget(lineEdit1);
    }

    Form2Layout->addLayout(layout3);

    layout1 = new QHBoxLayout(0, 0, 6, "layout1");

    pushButton1 = new QPushButton(this, "pushButton1");
    pushButton1->setDefault(FALSE);
    pushButton1->setText(tr("O&K"));
    layout1->addWidget(pushButton1);

    pushButton2 = new QPushButton(this, "pushButton2");
    pushButton2->setDefault(TRUE);
    pushButton2->setText(tr("&Cancel"));

    layout1->addWidget(pushButton2);
    Form2Layout->addLayout(layout1);

    resize(QSize(239, 75).expandedTo(minimumSizeHint()));
    connect(pushButton1, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(pushButton2, SIGNAL(clicked()), this, SLOT(cancelledClicked()));
}

InputDialog::~InputDialog()
{
}

void InputDialog::okClicked()
{
    m_cancelled = false;
    emit dialogDone();
}

void InputDialog::cancelledClicked()
{
    m_cancelled = true;
    emit dialogDone();
}


