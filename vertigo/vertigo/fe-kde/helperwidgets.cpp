#include "helperwidgets.h"
#include "app.h"


#include <qscrollview.h>
#include <qtooltip.h>
#include <qregexp.h>

#include <klistview.h>
#include <kdebug.h>


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


TopicEdit::TopicEdit(QWidget * parent, QString name):QTextEdit(parent, name)
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
	return minimumSizeHint();
}

QSize TopicEdit::minimumSizeHint() const
{
    QFontMetrics fm = fontMetrics();
    int m = frameWidth() * 2;
    return QSize( fm.maxWidth() + m, fm.height() + QMAX( 2*1, fm.leading()) +  m );
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


InputLineEdit::InputLineEdit(QWidget * parent, QString name):QLineEdit(parent, name)
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


UserlistItem::UserlistItem(KListView * list, QPixmap *pix, User *user)
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
