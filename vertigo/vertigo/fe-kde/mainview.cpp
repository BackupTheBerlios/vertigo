#include "mainview.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qapplication.h>
#include <qdragobject.h>
#include <qregexp.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qwidgetstack.h>
#include <qwidget.h>
#include <klistbox.h>
#include <qgrid.h>
#include <kdebug.h>
#include <sys/time.h>
#include <kmainwindow.h>
#include <kiconloader.h>
#include <kaction.h>
#include <kstdaction.h>
#include <klocale.h>
#include <kedittoolbar.h>
#include <ksqueezedtextlabel.h>
#include <qstyle.h>
#include <qtoolbutton.h>
#include <dcopclient.h>

#include "textwidget.h"
#include "app.h"

#include "../common/xchat.h"
#include "../common/xchatc.h"
#include "../common/inbound.h"
#include "../common/outbound.h"

#include "fe-kde.h"


MainView::MainView(MainWindow * w, session * sess)
:  QWidget(w, "MainView"), MainViewIface("MainView"),  ContainerView(w)
{
    Form1Layout = new QVBoxLayout(this, 1, 1, "Form1Layout");

    layout1 = new QHBoxLayout(0, 1, 1, "layout1");

    setSession(sess);

    /*topicStack = new QWidgetStack(this);
    topicStack->setFrameShape(QFrame::LineEditPanel);
    topicStack->setFrameShadow(QFrame::Sunken);
*/
    //topicLabel = new TopicLabel(topicStack, 2);
	topicEdit = new TopicEdit(this);
	
	
  topicEdit->
      setSizePolicy(QSizePolicy
                    (QSizePolicy::Minimum, QSizePolicy::Maximum, false));


   m_oldTopicParaPos=0;
   m_oldTopicIndexPos=0;
	   


//topicLabel->setMinimumWidth(1);
    //topicLabel->setMinimumHeight(1);
    //  topicStack->setMinimumHeight(1);

    //topicLabel->setTextFormat(Qt::RichText);
    //topicLabel->setAlignment(QLabel::WordBreak | QLabel::AlignVCenter);

    //topicStack->addWidget(topicLabel, 1);
    //topicLineEdit = new TopicLineEdit(topicStack, 1);
    //  topicLineEdit->
    //    setSizePolicy(QSizePolicy
    //                ( QSizePolicy::Minimum, QSizePolicy::Maximum, false));

//    topicLineEdit->setMinimumHeight(1);
    /*topicLineEdit->setFrameShape(QFrame::NoFrame);

    topicStack->setPaletteBackgroundColor(topicLineEdit->paletteBackgroundColor());

    topicStack->addWidget(topicLineEdit, 2);
    topicStack->raiseWidget(1);
    topicStack->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum, false));

    layout1->addWidget(topicStack);
*/
layout1->addWidget(topicEdit);

    pushButton5 = new QPushButton(this, "pushButton5");
    layout1->addWidget(pushButton5);
    Form1Layout->addLayout(layout1);

    // topicStack->hide();
    pushButton5->hide();

    splitter1 = new QSplitter(this, "splitter1");
    splitter1->setOrientation(QSplitter::Horizontal);
    splitter1->setOpaqueResize(true);

//splitter1->setSizes(QValueList<int>() << 100 << 3);

    m_textPanel = new QWidget(splitter1, "layout5");

    layout5 = new QVBoxLayout(m_textPanel, 1, 1, "layout5");

    textEdit1 = new TextView(m_textPanel, "textEdit1");
/*
    textEdit1->
	setSizePolicy(QSizePolicy
		      (QSizePolicy::Minimum, QSizePolicy::Expanding,
		       false));
*/
    layout5->addWidget(textEdit1);

    layout3 = new QHBoxLayout(0, 1, 1, "layout3");

    m_inputLine = new InputLineEdit(m_textPanel);
    layout3->addWidget(m_inputLine);

    pushButton1 = new QPushButton(m_textPanel, "pushButton1");
    layout3->addWidget(pushButton1);
    layout5->addLayout(layout3);

    m_userlistPanel = new QWidget(splitter1, "layout4");

    layout4 = new QVBoxLayout(m_userlistPanel, 1, 1, "layout4");

    m_userList = new UserlistView(m_userlistPanel);
    /*  m_userList->
       setSizePolicy(QSizePolicy
       ((QSizePolicy::SizeType) 1,
       (QSizePolicy::SizeType) 7, 0, 0,
       m_userList->sizePolicy().hasHeightForWidth()));
       m_userList->setMaximumSize(QSize(32767, 32767));
       m_userList->setSelectionMode(QListView::Extended); */
    //m_userList->addColumn("User");
    //m_userList->setColumnWidthMode(0, QListView::Manual);
    //m_userList->setHScrollBarMode(QScrollView::AlwaysOff);

    layout4->addWidget(m_userList);

//    gridLayout = new QGrid(2, m_userlistPanel);

    //  pushButton10_2_2 = new QPushButton(gridLayout, "pushButton10_2_2");

    if(prefs.userlistbuttons) {
	gridLayout = new QGridLayout(m_userlistPanel, 1, 2);
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

    //layout4->addWidget(gridLayout);

    Form1Layout->addWidget(splitter1);

    pushButton5->setText(i18n("&Channel"));
    pushButton1->setText(i18n("Te&xt"));

    //pushButton10_2_2->setText(i18n("pushButton10"));

    connect(topicEdit, SIGNAL(returnPressed()), this, SLOT(topicEnter()));

    //connect(topicEdit, SIGNAL(textChanged(const QString &)), topicLabel, SLOT(slotTextChanged(const QString &)));

    connect(m_inputLine, SIGNAL(returnPressed()), this, SLOT(entryEnter()));
    connect(m_inputLine, SIGNAL(tabPressed()), this, SLOT(entryTab()));
    connect(m_inputLine, SIGNAL(downPressed()), this, SLOT(entryDown()));
    connect(m_inputLine, SIGNAL(upPressed()), this, SLOT(entryUp()));

    splitter1->setCollapsible(m_textPanel, false);
    splitter1->setCollapsible(m_userlistPanel, true);

//splitter1->setSizes(QValueList<int>() << width() << 1);

splitter1->setSizes(QValueList < int >() << width()- m_userlistPanel->minimumWidth() <<  m_userlistPanel->minimumWidth());


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
m_session->gui->win=w;
}




//following taken from konversation

void replaceDecoration(QString & line, char decoration, char replacement)
{
    int pos;
    bool decorated = false;

    while((pos = line.find(decoration)) != -1) {
	line.replace(pos, 1, (decorated) ? QString("</%1>").arg(replacement) : QString("<%1>").arg(replacement));
	decorated = !decorated;
    }
}

QString convertText(const QString & line)
{
    QString filteredLine(line);

// Replace all & with &amp;
    filteredLine.replace('&', "&amp;");
    // Replace all < with &lt;
    filteredLine.replace(QRegExp("\\<"), "&lt;");
    // Replace all > with &gt;
    filteredLine.replace(QRegExp("\\>"), "&gt;");
    // Replace all 0x0f (reset color) with \0x031,0
    filteredLine.replace(QRegExp("\017"), "\0031,0");

    // replace \003 codes with rich text color codes
    QRegExp colorRegExp("\003([0-9]|1[0-5])(,([0-9]|1[0-5])|)");

    // TODO: Make Background colors work somehow. The code is in comments until we
    //       find some way to use it
//  bool bgColor=false;
    bool firstColor = true;
    QString colorString;
    int pos;

    while((pos = colorRegExp.search(filteredLine)) != -1) {
	// TODO: make these configurable
	const char *colorCodes[] = { "ffffff", "000000", "000080", "008000", "ff0000", "a52a2a",
	    "800080", "ff8000",
	    "808000", "00ff00", "008080", "00ffff", "0000ff", "ffc0cb",
	    "a0a0a0", "c0c0c0"
	};

	colorString = (firstColor) ? "" : "</font>";

	int foregroundColor = colorRegExp.cap(1).toInt();

	colorString += "<font color=\"#" + QString(colorCodes[foregroundColor]) + "\">";

	filteredLine.replace(pos, colorRegExp.cap(0).length(), colorString);
	firstColor = false;
    }

    if(!firstColor)
	filteredLine += "</font>";
//  if(bgColor) colorString+="</td></tr></table>";

    kdDebug() << filteredLine << endl;

    // Replace all text decorations
    replaceDecoration(filteredLine, '\x02', 'b');
    replaceDecoration(filteredLine, '\x09', 'i');
    replaceDecoration(filteredLine, '\x13', 's');
    replaceDecoration(filteredLine, '\x15', 'u');
    replaceDecoration(filteredLine, '\x16', 'b');	// should be reverse
    replaceDecoration(filteredLine, '\x1f', 'u');

/*
  // Replace multiple Spaces with "<space>&nbsp;"
  do
  {
    pos=filteredLine.find("  ");
    if(pos!=-1) filteredLine.replace(pos+1,1,"&nbsp;");
  } while(pos!=-1);
*/
    return filteredLine;
}

QString escapeText(QString t)
{
    t.replace('\x02', "%B");
    t.replace('\x03', "%C");
    t.replace('\x09', "%I");
    t.replace('\x1f', "%U");
    return t;
}

QString unescapeText(QString t)
{
	kdDebug () << "unescape "<< t<<endl;
    t.replace("%B", "\x02");
    t.replace("%C", "\x03");
    t.replace("%I", "\x09");
    t.replace("%U", "\x1f");
    return t;
}

void MainView::topicEnter()
{
	kdDebug() << "topicEnter"<<endl;
	
    if(m_session)
	{
		topicEdit->getCursorPosition(&m_oldTopicParaPos, &m_oldTopicIndexPos);
		kdDebug() << "topic: |"<<unescapeText(topicEdit->text()).latin1()<<"|"<<endl;
		set_topic(m_session, (char*)unescapeText(topicEdit->text()).latin1());
	}
}

void MainView::setTopic(QString t)
{
	kdDebug() << "setting topic: "<<t<<endl;
    topicEdit->setTopicText(t);
	topicEdit->setCursorPosition(m_oldTopicParaPos, m_oldTopicIndexPos);
	m_oldTopicParaPos=0;
	m_oldTopicIndexPos=0;
	//topicLabel->setText(convertText(t));
    //topicLineEdit->setText(escapeText(t));
    /*if(topicStack->isHidden()) {
	topicStack->show();
	//pushButton5->show();
    }*/
    //QToolTip::add(topicLineEdit, topicLabel->text());
}

void MainView::giveInputFocus()
{
    m_inputLine->setFocus();
    //topicStack->raiseWidget(1);
    //topicStack->update();
}

void MainView::entryDown()
{
    char *c = history_down(&m_session->history);

    if(c)
	m_inputLine->setText(c);
}

void MainView::entryUp()
{
    char *c = history_up(&m_session->history);

    if(c)
	m_inputLine->setText(c);
}

void MainView::entryEnter()
{
    if(!m_inputLine->text().isEmpty()) {
	QString d = m_inputLine->text();

	m_inputLine->clear();
	handle_multiline(m_session, (char*)d.latin1(), 1, 0);
    }
}

void MainView::entryTab()
{
    QString t = m_inputLine->text();
    int pos = m_inputLine->cursorPosition();
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
	    m_inputLine->setText(QString("/%1%2").arg(n).arg(t.mid(firstSpace)));
	} else {
	    m_inputLine->setText(QString("/%1%2 ").arg(n).arg(t.mid(firstSpace)));
	}
    } else {
	int lastSpace = t.findRev(' ', pos - 1);
	int start = (lastSpace == -1) ? 0 : lastSpace + 1;
	QString n = m_session->gui->comp->makeCompletion(t.mid(start, pos - start));

	if(n.isEmpty())
	    return;
	if(!m_session->gui->comp->hasMultipleMatches() && ((firstSpace == -1) || (pos <= firstSpace)))
	    m_inputLine->setText(n + ": " + t.mid(pos));
	else {
	    m_inputLine->setText(t.left(start) + n + t.mid(pos));
	    m_inputLine->setCursorPosition(start + n.length());
	}
    }
}

#if 0
void aaa()
{
    QString t = m_inputLine->text();
    int pos = m_inputLine->cursorPosition();
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
	    m_inputLine->setText(n + ": " + t.mid(pos));
	else
	    m_inputLine->setText(n + t.mid(pos));
    } else {
	m_inputLine->setText(t.left(start) + n + t.mid(pos));
    }
}
#endif

void MainView::completionMatches(const QStringList & matchlist)
{
    if(matchlist.count() > 1) {
	QString a = matchlist.join(" ");

	textEdit1->appendText(a, false, true);
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


/*
WidgetStack::WidgetStack(QWidget * parent):QWidgetStack(parent)
{
}

WidgetStack::~WidgetStack()
{
}

QSize WidgetStack::sizeHint() const
{
    return (QSize(1, 1));
}
*/


TopicEdit::TopicEdit(QWidget * parent):QTextEdit(parent)
{
	setVScrollBarMode(QScrollView::AlwaysOff);
	setHScrollBarMode(QScrollView::AlwaysOff);
    setFocusPolicy(QWidget::WheelFocus);
	setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum ) );
	setWordWrap(QTextEdit::NoWrap);
}

TopicEdit::~TopicEdit()
{
}
QSize TopicEdit::sizeHint() const
{
  /*  QFontMetrics fm( font() );
    int h = QMAX(fm.lineSpacing(), 14) + 2*1;
    int w = fm.width( 'x' ) * 17; // "some"
    int m = frameWidth() * 2;
    return (style().sizeFromContents(QStyle::CT_LineEdit, this,
                     QSize( w + m, h + m ).
                     expandedTo(QApplication::globalStrut())));*/
					 return minimumSizeHint();
}

QSize TopicEdit::minimumSizeHint() const
{
    QFontMetrics fm = fontMetrics();
    int h = fm.height() + QMAX( 2*1, fm.leading() );
    int w = fm.maxWidth();
    int m = frameWidth() * 2;
    return QSize( w + m, h + m );
}




void TopicEdit::setTopicText(QString s)
{
	m_topicText=s;
	setText(convertText(s));
	QToolTip::add(this, "<b></b>"+text());
}

void TopicEdit::keyPressEvent( QKeyEvent *e )
{
 if (( e->key()==Key_Enter ) || (e->key()==Key_Return)) {
	emit returnPressed();
	return;
 }
 QTextEdit::keyPressEvent( e );
 
 
}

void TopicEdit::focusInEvent ( QFocusEvent * )
{
	setText(escapeText(m_topicText));
}

void TopicEdit::focusOutEvent ( QFocusEvent * )
{
	setText(convertText(m_topicText));
}


/*
QSize TopicEdit::sizeHint() const
{
    return (QSize(1, fontMetrics().height()));
	}
*/	


InputLineEdit::InputLineEdit(QWidget * parent):QLineEdit(parent)
{
    installEventFilter(this);
}

InputLineEdit::~InputLineEdit()
{
}

bool InputLineEdit::eventFilter(QObject * o, QEvent * e)
{
    switch (e->type()) {
	case QEvent::KeyPress:
	{
	    QKeyEvent *ev = (QKeyEvent *) e;

	    switch (ev->key()) {
		case Key_Up:
		    emit upPressed();
		    return true;
		case Key_Down:
		    emit downPressed();
		    return true;
		case Key_Return:
		case Key_Enter:
		    emit returnPressed();
		    return true;
		    break;
		case Key_Tab:
		    emit tabPressed();
		    return true;
		    break;
		default:
		    return QLineEdit::eventFilter(o, e);
	    }
	    break;
	}
	default:
	    return QLineEdit::eventFilter(o, e);
    }
    return QLineEdit::eventFilter(o, e);
}

UserlistView::UserlistView(QWidget * parent):KListView(parent)
{
addColumn ( "");
addColumn ( "User");
addColumn ( "Host");
setFullWidth(true);
setAllColumnsShowFocus(true);
;//setSelectionMode(QListBox::Extended);
}

UserlistView::~UserlistView()
{
}

/*
QSize UserlistView::minimumSizeHint() const
{
    QSize n = QListBox::minimumSizeHint();
    int w=maxItemWidth()+style().pixelMetric(QStyle::PM_ScrollBarExtent)+2;
    if (w<120)
    n.setWidth(w);
    else
	    n.setWidth(120);
    return n;
}*/

/*
void UserlistView::resizeEvent(QResizeEvent * r)
{
    KListView::resizeEvent(r);
    setColumnWidth(0, visibleWidth());
}*/


UserlistItem::UserlistItem(UserlistView * list, QPixmap *pix, User *user)
: KListViewItem(list)
{
	m_pixmap=pix;
	if (pix)
	setPixmap(0, *pix);
	setUser(user);
}

UserlistItem::~UserlistItem()
{
}


void UserlistItem::setUser(User *user)
{	
	m_user=user;
	kdDebug() << "--> New User Item: nick="<<user->nick<<" host="<<user->hostname<<endl;
    setText(1, user->nick);
	    setText(2, user->hostname);

}

QPixmap *UserlistItem::getPixmap()
{
	return m_pixmap;
}

int UserlistItem::compare ( QListViewItem * i, int col, bool ascending ) const
{
	if (col==0)
	{
		UserlistItem *xi=(UserlistItem *)i;
		int l=xchatapp->getUserLevel(xi->getPixmap());
		int my=xchatapp->getUserLevel(m_pixmap);
		if (my == l)
		{
			return KListViewItem::compare(i,1, ascending);
		}
		else if (my>l){
			return -1;
		}
		else{
			return 1;
		}
	}
	else{
		return KListViewItem::compare(i,col, ascending);
	}
}

void UserlistItem::paintCell ( QPainter * p, const QColorGroup & cg, int col, int width, int align )
{
	    if (col==0)
		{
		KListViewItem::paintCell(p, cg, col, width, AlignCenter);
		}
		else{
			KListViewItem::paintCell(p, cg, col, width, align);
		}
}

/*void UserlistItem::paint(QPainter * painter)
{

    UserlistView *view = static_cast < UserlistView * >(listBox());

    int itemHeight = height(view);
    int yPos;

//if (isAlternate() && !(isCurrent()))
//      painter->fillRect(0,0, view->width(), itemHeight,  QBrush(view->alternateBackground()));

    if(m_pixmap && !m_pixmap->isNull()) {
	yPos = (itemHeight - m_pixmap->height()) / 2;
	painter->drawPixmap(5, yPos, *m_pixmap);
    }

    if(!text().isEmpty()) {
	QFontMetrics fm = painter->fontMetrics();

	yPos = ((itemHeight - fm.height()) / 2) + fm.ascent();
	painter->drawText(5 + 5 + 5, yPos, text());
    }
}

int UserlistItem::height(const QListBox * lb) const
{
    return lb->fontMetrics().lineSpacing() + 2;
}

int UserlistItem::width(const QListBox * lb) const
{
    return 5 + lb->fontMetrics().width(text()) + 5 + 5 + 5;
    //+ listBox()->style().pixelMetric(QStyle::PM_ScrollBarExtent);
}*/
