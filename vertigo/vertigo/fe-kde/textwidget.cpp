#include "textwidget.h"

#include <qclipboard.h>
#include <qapplication.h>
#include <qtimer.h>
#include <qscrollview.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qptrlist.h>
#include <qstyle.h>
#include <qcursor.h>
#include <qnamespace.h>
#include <qdatetime.h>
#include <qdragobject.h>

#include <qtextcodec.h>
#include <kcharsets.h>
#include <kglobal.h>
#include <qdatetime.h>
#include <kdebug.h>
#include <krootpixmap.h>

#include "app.h"
#include "../common/xchat.h"
#include "../common/xchatc.h"
#include "../common/text.h"


#define NoSelection -1

int myabs(int a)
{
    if (a < 0)
	return (-1 * a);
    return a;
}

/*
STextViewportWidget::STextViewportWidget(QWidget * parent)
:  QWidget(parent)
{
    clearWFlags(WResizeNoErase);
}


STextViewportWidget::~STextViewportWidget()
{
}
*/



TextView::TextView(QWidget * parent, const char *name):QScrollView(parent,
	    name,
	    0)
{
//STextViewportWidget *v = new STextViewportWidget(this);
// addChild(v);

    m_palette=xchatapp->palette();

    viewport()->setMouseTracking(true);
    setSizeIncrement(20, 10);
    setFocusPolicy(QWidget::WheelFocus);
    setHScrollBarMode(QScrollView::AlwaysOff);

    m_buffer = new TextBuffer;
    clearBuffer();

    m_buffer->renderState = new TextRenderState;
    clearRenderState(true);

    m_maxEntries = 200;
    m_styledSep = false;	//buggty
    if (m_styledSep)
	m_sepWidth = style().pixelMetric(QStyle::PM_SplitterWidth, this);
    else
	m_sepWidth = 1;
    m_marginWidth = 1;
    m_maxSubLines = 6;
    m_minWrapWidth = 30;
    m_showTimeStamp = true;
    m_timeStampFormat = "[hh:mm]";
    m_wrapBoundary = 15;
    m_autoResizeColumns = true;
    m_scrollType = AlwaysScroll;
    m_doubleBuffer = false;

    m_cursorSplit = new QCursor(Qt::SplitHCursor);
    m_cursorIbeam = new QCursor(Qt::IbeamCursor);

    viewport()->setEraseColor(convColor(19));

//viewport()->setBackgroundMode(QWidget::NoBackground);

    if (m_doubleBuffer) {
	viewport()->setBackgroundMode(QWidget::NoBackground);
	m_buffer->pixmap = new QPixmap();
    }
    if (isReverseLayout())
	m_buffer->sepValue = visibleWidth();


	m_buffer->layoutTimerId=0;

    recalcLayout();
    setFontName("Bitstream Vera Sans 9");

    /*  for (int i = 0; i < 2; i++) {
       appendTextIndent("yo", "dfgfdg <\037flood>");
       appendTextIndent("fgfgdgs", "dggfgf  fdfgfg<d\0034fd\007f>");
       appendTextIndent("yod", "gfgdsdg  <flooddddddd>");
       appendTextIndent("yobishahffffffffffffff","vbbvn <dfdf> <dfsdfdf>");
       appendTextIndent("yo", "\0037\002<flood> ndsh facker");
       appendTextIndent("yo","\0037<flood> ndsh facker");
       appendTextIndent("dfgsdfgsdg", "\00306<df\0036,7dfdf\026dddd>");
       appendTextIndent("yo", "ffg\00306,2<flo\003\0034ffod>");
       appendTextIndent("vdbcvbf","<dfdf>");

       appendTextIndent("yo", "\037<flo\002od>\0035\037ddd");
       appendTextIndent("yo", "\037<flood>\0035\037ddd");
       appendTextIndent("ertert","<dfdf>");
       appendTextIndent("yodfsdsffdssdfsdfs\00305fdsdfsdfdsffdfdsfsdgd", "<flood>");
       } */

}

void TextView::viewportResizeEvent(QResizeEvent * pe)
{
    QScrollView::resizeEvent(pe);
    if (!m_buffer->sepActive && m_buffer->layoutEnabled) {
	scheduleLayout();
    }

}

void TextView::scheduleLayout()
{
	if (!m_buffer->layoutTimerId)
	m_buffer->layoutTimerId = startTimer( 100 );
	
}

void TextView::timerEvent ( QTimerEvent *e )
{
    kdDebug() << "timer event " << e->timerId() << endl;
    killTimer(e->timerId());
	if ( e->timerId() == m_buffer->layoutTimerId ) {
		m_buffer->layoutTimerId=0;
		recalcLayout();
		repaint();
	}
}

void TextView::setFontName(const QString & name)
{
    int i = name.findRev(' ');
    int s = 10;

    if (m_buffer->font) {
	delete m_buffer->font;
	delete m_buffer->fm;
    }
    m_buffer->font = new QFont();
    m_buffer->font->setFamily(name.left(i));

    if (i != -1)
	s = name.mid(i + 1).toInt();

    m_buffer->font->setPointSize(s);
    m_buffer->fm = new QFontMetrics(*(m_buffer->font));
    m_buffer->lineHeight = m_buffer->fm->height();
    m_buffer->lineDescent =
	m_buffer->fm->descent() + 1 + m_buffer->fm->leading();

}

void TextView::clear()
{
    int s = m_buffer->sepValue;

    clearBuffer(false, false);
    clearRenderState();
    m_buffer->sepValue = s;
    recalcLayout();

    viewport()->repaint();
}

void TextView::clearBuffer(bool resetFonts, bool resetRenderState)
{
    m_buffer->sepValue = 0;
    m_buffer->sepActive = false;

    if (resetRenderState)
	m_buffer->renderState = 0;

    if (resetFonts) {
	m_buffer->font = 0;
	m_buffer->fm = 0;
	m_buffer->lineHeight = 0;
	m_buffer->lineDescent = 0;
    }

    m_buffer->numEnts = 0;
    m_buffer->startEnt = 0;
    m_buffer->endEnt = 0;
    m_buffer->selStartX = m_buffer->selStartY = m_buffer->selEndX =
	m_buffer->selEndY = 0;

    m_buffer->pageTopEnt = 0;
    m_buffer->pageBottomEnt = 0;

    m_buffer->lastStartEnt = 0;
    m_buffer->lastEndEnt = 0;
    m_buffer->lastStartOffset = m_buffer->lastEndOffset = 0;

    m_buffer->onlySep = false;
    m_buffer->verticalSliderPressed = false;
    m_buffer->skiprender = false;

    m_buffer->paintDirty = true;
    m_buffer->pixmap = 0;
}


void TextView::clearRenderState(bool attribs)
{
    m_buffer->renderState->bg = 19;
    m_buffer->renderState->fg = 18;
    m_buffer->renderState->parsingCol = false;
    m_buffer->renderState->numCol = 0;
    m_buffer->renderState->parsingBG = false;

    if (attribs) {
	m_buffer->renderState->bold = false;
	m_buffer->renderState->underline = false;
    }
}

void TextView::appendLines(const QString & rawLines, bool scroll)
{
    QStringList lineList = QStringList::split('\n', rawLines);

    for (QStringList::Iterator dd = lineList.begin(); dd != lineList.end();
	 ++dd) {
	appendText(*dd, true, false);
	if (scroll)
		scrollDown();
    }
    /*
    if (scroll) {
	scrollDown();
    }*/

}

void TextView::appendText(QString text, bool indent, bool scroll)
{
    //kdDebug() << "appendText: "<<text<<endl;
    if (indent) {
	int i = text.find('\t');

	if (i != -1)
	    appendTextIndent(text.mid(i + 1), text.left(i), scroll);
	else
	    appendTextIndent(text, QString::null, scroll);
    }
    else
	newTextEntry(text, QString::null, scroll);
}

void TextView::appendTextIndent(QString r, QString l, bool scroll)
{
    newTextEntry(r, l, scroll);
}


void TextView::newTextEntry(QString r, QString l, bool scroll)
{
    TextEntry *a = new TextEntry;
    int oldHeight = m_buffer->textHeight;

    QTime t = QTime::currentTime();

    a->timeStamp = t.toString(m_timeStampFormat);

    a->rightText = r;
    a->leftText = l;
    a->leftWidth = a->leftX = a->leftY = a->rightX = a->rightWidth = 0;
    a->selStartOffset = a->selEndOffset = 0;
    a->linesTaken = 1;
    a->next = 0;

    m_buffer->numEnts++;

    if (m_buffer->startEnt) {
	m_buffer->endEnt->next = a;
	m_buffer->endEnt = a;
    }
    else
	m_buffer->startEnt = m_buffer->endEnt = a;

    if (m_buffer->numEnts > m_maxEntries) {
	TextEntry *ent = m_buffer->startEnt->next;

	m_buffer->textHeight -=
	    (m_buffer->startEnt->linesTaken * m_buffer->lineHeight);
	delete m_buffer->startEnt;

	m_buffer->startEnt = ent;
    }

    int i = recalcEntryLayout(a, m_autoResizeColumns);

    if (i == -1)		// sep moved-- need to relayout from scratch
	recalcLayout();
    else {
	m_buffer->textHeight += i;
	resizeView(m_buffer->textHeight);
    }

    if (m_buffer->textHeight < visibleHeight())
	if (i == -1)
	    updateContents();
	else
	    updateContents(0, oldHeight, contentsWidth(),
			   m_buffer->textHeight - oldHeight);
    else {
	if (scroll)
	    scrollDown();
    }
}

int TextView::recalcEntryLayout(TextEntry * ent, bool resizeCols)
{
    int leftWidth = textWidth(ent->leftText), rightStart = 0, rightWidth =
	0;

    ent->leftWidth = leftWidth;

    if (isReverseLayout()) {
	if ((m_buffer->sepValue >= visibleWidth() - leftWidth)
	    && resizeCols) {
	    m_buffer->sepValue =
		visibleWidth() - 4 * leftWidth -
		m_marginWidth - m_sepWidth;
	    //recalcLayout();
	    return -1;

	}
	else {
	    ent->leftX = m_buffer->sepValue;
	    rightStart = m_marginWidth;
	    rightWidth = m_buffer->sepValue - m_marginWidth - rightStart;
	}
    }

    else {
	if (leftWidth + m_marginWidth >= m_buffer->sepValue) {
	    if (resizeCols) {
		m_buffer->sepValue = leftWidth + m_marginWidth * 2;
		if (m_showTimeStamp)
		    m_buffer->sepValue += textWidth(ent->timeStamp) * 2;
		//recalcLayout();
		return -1;

	    }
	    else {
		ent->leftX = 0;
		ent->rightX = leftWidth + m_sepWidth + m_marginWidth;
	    }
	}
	else {
	    ent->leftX =
		m_buffer->sepValue - leftWidth - m_sepWidth +
		m_marginWidth;
	    ent->rightX = m_buffer->sepValue + m_sepWidth + m_marginWidth;
	}
    }

    rightWidth = visibleWidth() - ent->rightX;
    ent->rightWidth = rightWidth;

    if (rightWidth < m_minWrapWidth) {
	ent->linesTaken = 1;
	return m_buffer->lineHeight;
    }
    else {
	int ret = 0;
	QString r = ent->rightText;
	int subLines = 0;

	while ((subLines < m_maxSubLines) and(r.isEmpty() == 0)) {
	    wrapText(r, rightWidth);
	    ret = ret + m_buffer->lineHeight;
	    subLines = subLines + 1;
	    rightWidth =
		visibleWidth() - (m_buffer->sepValue +
				  m_sepWidth + m_marginWidth);
	}
	ent->linesTaken = subLines;
	return ret;
    }
    // should never reach here
    return 0;
}


void TextView::paintArea(QPainter * p)
{

    m_buffer->pageTopEnt = 0;
    m_buffer->pageBottomEnt = 0;

    int h = m_buffer->lineHeight;
    TextEntry *ent = m_buffer->startEnt;
    TextEntry *lastEnt = 0;
    int startEdge =
	isVerticalSliderPressed()? contentsY() / 1.5 : contentsY() - 30;
    int endEdge =
	isVerticalSliderPressed()? (contentsY() +
				    visibleHeight()) * 1.5 : contentsY() +
	visibleHeight() + 30;

if (m_buffer->onlySep){
goto onlysep;
}


while (ent) {
	if ((h > startEdge) && (h < endEdge)) {
	    //m_buffer->verticalSliderPressed) {
	    if (m_buffer->pageTopEnt == 0) {
		m_buffer->pageBottomEnt = 0;
		m_buffer->pageTopEnt = ent;
	    }
	    h = paintEntry(ent, p, h);
	}
	else {
	    if (m_doubleBuffer)
		h = paintEntry(ent, p, h);
	    else
		h = h + (ent->linesTaken * m_buffer->lineHeight);
	    if (!m_buffer->pageBottomEnt)
		m_buffer->pageBottomEnt = lastEnt;
	}
	lastEnt = ent;
	ent = ent->next;
    }
    if (!m_buffer->pageTopEnt)
	m_buffer->pageTopEnt = m_buffer->startEnt;
    if (!m_buffer->pageBottomEnt)
	m_buffer->pageBottomEnt = m_buffer->endEnt;

onlysep:

    if (m_styledSep) {
style().drawPrimitive(QStyle::PE_Splitter, p,
      QRect(m_buffer->sepValue, 0, m_sepWidth,
    contentsHeight()), colorGroup(), 0);
    }
    else {
QColor c = (m_buffer->sepActive) ? Qt::white : Qt::darkGray;

p->fillRect(m_buffer->sepValue, contentsY(), m_sepWidth,
    visibleHeight(), QBrush(c));
    }

    if (m_buffer->onlySep) {
m_buffer->onlySep = false;
    }

    
}

int TextView::paintEntry(TextEntry * ent,
			  QPainter * p, int y,
			  int selStart, int selEnd, bool onlysel)
{
    QString text = ent->leftText, curText = QString::null;
    int tlen = ent->leftWidth;
    int x = ent->leftX;
    int seloffstart = -1, seloffend = -1;

    if (y != -1)
	ent->leftY = y - m_buffer->lineHeight;
    else
	y = ent->leftY + m_buffer->lineHeight - contentsY();

    if (!onlysel) {
//      QString stamp = ent->timeStamp;
	if (m_showTimeStamp) {
	    QString stamp = ent->timeStamp;

	    paintTextChunk(wrapText(stamp, x), m_marginWidth, y, p);
	}
    }

    if (tlen) {
	int ll = text.length();

	if ((selStart <= ll) && (selStart != -1)) {
	    seloffstart = selStart - 1;
	    if (selEnd > ll)
		seloffend = ll;
	    else
		seloffend = selEnd - 1;
	}
	paintTextChunk(text, x, y, p, true, seloffstart, seloffend, true,
		       onlysel);
	if (onlysel && ((seloffstart != -1) || (seloffend != -1)))	// xxx: what if it sel starts in left and goes to right?
	    return y;
    }


    text = ent->rightText;
    x = ent->rightX;
    int av = ent->rightWidth;

    if (ent->linesTaken == 1) {
	paintTextChunk(text, x, y, p, 1, selStart, selEnd, true, onlysel);
	y = y + m_buffer->lineHeight;
    }
    else {
	int subLines = 0;

	curText = QString::null;
	while (subLines < ent->linesTaken) {
	    curText = wrapText(text, av, false);
	    paintTextChunk(curText, x, y, p, false);
	    y = y + m_buffer->lineHeight;
	    subLines = subLines + 1;
	    x = m_buffer->sepValue + m_sepWidth + m_marginWidth;
	    av = visibleWidth() - x;
	}
	clearRenderState(true);
    }
    return y;
}

void TextView::paintTextChunk(QString text, int x, int y, QPainter * p,
			       bool clearState, int selstart, int selend,
			       bool dourls, bool onlysel)
{
//kdDebug() << "painting "<<text<<endl;
    QChar c;
    int i = 0, curLen = 0, tlen = text.length();
    int colNum = 0, oldbg = 0, oldfg = 1;

    QString col = QString::null;
    QString curText = QString::null;

    bool dosel = false, insel = false;
    int lastSpace = 0;

    if (((selstart != NoSelection) || (selend != NoSelection)) &&
	(selstart != selend))
	dosel = true;

    if (!tlen)
	return;

    while (tlen > i) {
	c = text.at(i);
	if ((m_buffer->renderState->parsingCol && c.isDigit() &&
	     m_buffer->renderState->numCol < 2) ||
	    (m_buffer->renderState->parsingCol && c == ',' &&
	     text.at(i + 1).isDigit() &&
	     m_buffer->renderState->numCol < 3)) {
	    if (c == ',') {
		m_buffer->renderState->parsingBG = true;
		if (m_buffer->renderState->numCol) {
		    m_buffer->renderState->numCol = 0;
		    colNum = col.toUShort();
		    col = QString::null;
		    if (colNum == 99)
			colNum = 18;
		    else
			colNum = colNum % 16;
		    m_buffer->renderState->fg = colNum;
		}
	    }
	    else {
		col = col.append(c);
		if (m_buffer->renderState->numCol < 7)
		    m_buffer->renderState->numCol =
			m_buffer->renderState->numCol + 1;
	    }
	    i = i + 1;
	}
	else {
	    if (m_buffer->renderState->parsingCol) {
		m_buffer->renderState->parsingCol = false;
		if (m_buffer->renderState->numCol) {
		    m_buffer->renderState->numCol = 0;
		    colNum = col.toUShort();
		    col = QString::null;
		    if (colNum == 99)
			colNum = 18;
		    else
			colNum = colNum % 16;

		    if (m_buffer->renderState->parsingBG) {
			m_buffer->renderState->bg = colNum;
			m_buffer->renderState->parsingBG = false;
		    }
		    else
			m_buffer->renderState->fg = colNum;
		}
		else {
		    paintText(x, y, curLen, m_buffer->lineHeight,
			      curText, p);
		    x = x + curLen;
		    curText = QString::null;
		    curLen = 0;
		    clearRenderState(true);
		}
	    }

	    else if (dosel) {
		if (i == selstart) {
		    paintText(x, y, curLen, m_buffer->lineHeight,
			      curText, p);
		    x = x + curLen;

		    curText = QString::null;
		    curLen = 0;

		    oldbg = m_buffer->renderState->bg;
		    oldfg = m_buffer->renderState->fg;
		    m_buffer->renderState->fg = 1;
		    //m_buffer->renderState->fg = m_buffer->renderState->bg;
		    m_buffer->renderState->bg = 9;
		    insel = true;
		}
		else if (i == selend) {
		    paintText(x, y, curLen, m_buffer->lineHeight,
			      curText, p);
		    x = x + curLen;

		    curText = QString::null;
		    curLen = 0;

		    m_buffer->renderState->fg = oldfg;
		    m_buffer->renderState->bg = oldbg;
		    insel = false;
		}

		else {
		    if (!insel && !onlysel)
			m_buffer->skiprender = true;
		    else
			m_buffer->skiprender = false;
		}
	    }

	    if (c == ATTR_COLOR) {
		paintText(x, y, curLen, m_buffer->lineHeight, curText, p);
		x = x + curLen;
		curText = QString::null;
		curLen = 0;
		m_buffer->renderState->parsingCol = 1;
		m_buffer->renderState->numCol = 0;
	    }
	    else if (c == ATTR_BOLD) {
		paintText(x, y, curLen, m_buffer->lineHeight, curText, p);
		x = x + curLen;
		curText = QString::null;
		curLen = 0;
		m_buffer->renderState->bold = !m_buffer->renderState->bold;
	    }
	    else if (c == ATTR_UNDERLINE) {
		paintText(x, y, curLen, m_buffer->lineHeight, curText, p);
		x = x + curLen;
		curText = QString::null;
		curLen = 0;
		m_buffer->renderState->underline = true;
	    }
	    else if (c == ATTR_RESET) {
		//kdDebug() << "got reset at "<<i<<" tlen="<<tlen<<endl;
		if (i != tlen - 1) {
		    paintText(x, y, curLen, m_buffer->lineHeight, curText,
			      p);
		    x = x + curLen;
		    curText = QString::null;
		    curLen = 0;
		    clearRenderState(true);
		}

	    }
	    else if (c == ATTR_REVERSE) {
		paintText(x, y, curLen, m_buffer->lineHeight, curText, p);
		x = x + curLen;
		curText = QString::null;
		curLen = 0;
		int t = m_buffer->renderState->bg;

		m_buffer->renderState->bg = m_buffer->renderState->fg;
		m_buffer->renderState->fg = t;

	    }
	    else if ((c == ATTR_BEEP) || (c == '\n'));

	   /* else if (c == ' ') {
		if (isUrl(text.mid(lastSpace, i - lastSpace))) {
		    QString b = curText.left(curText.findRev(' ') + 1);

		    int xx = textWidth(b);
		    bool olde;

		    //kdDebug() << "b2= "<<b<<endl; 
		    paintText(x, y, xx, m_buffer->lineHeight, b, p);
		    x = x + xx;
		    oldfg = m_buffer->renderState->fg;
		    olde = m_buffer->renderState->underline;
		    m_buffer->renderState->underline = true;
		    if (m_buffer->renderState->fg == 1)
			m_buffer->renderState->fg = 2;
		    b = text.mid(lastSpace, i - lastSpace);
		    // kdDebug() << "b3= "<<b<<endl; 
		    xx = textWidth(b);
		    //  kdDebug() << "calling paintTextChunk!!"<<endl;
		    paintTextChunk(b, x, y, p, false, -1, -1, false,
				   false);
		    x = x + xx;
		    m_buffer->renderState->fg = oldfg;
		    m_buffer->renderState->underline = olde;
		    curText = QString::null;
		    curLen = 0;
		}
		lastSpace = i + 1;
		curLen = curLen + charWidth(c);
		curText = curText.append(c);
		//i++;
	    }*/
	    else {
		curLen = curLen + charWidth(c);
		curText = curText.append(c);
		//i++;
	    }
	    i++;


	}
    }

//kdDebug() << "test= "<<dourls << " text="<<text.mid(lastSpace, i - lastSpace)<<endl; 

   /* if (dourls && (isUrl(text.mid(lastSpace, i - lastSpace)))) {
	QString b = curText.left(curText.findRev(' ') + 1);

	//kdDebug() << "b= "<<b<<endl; 
	int xx = textWidth(b);

// todo: XXX might have to strip ctrl chars or run through paintTextCHunk agaain!
	paintText(x, y, xx, m_buffer->lineHeight, b, p);
	x = x + xx;
	b = text.mid(lastSpace, i - lastSpace);
	//kdDebug() << "b4= "<<b<<endl; 
	xx = textWidth(b);
	m_buffer->renderState->underline = true;
	if (m_buffer->renderState->fg == 1)
	    m_buffer->renderState->fg = 2;
	paintText(x, y, xx, m_buffer->lineHeight, b, p);
    }
    else {*/
	if (curLen)
	    paintText(x, y, curLen, m_buffer->lineHeight, curText, p);
    //}

    if (clearState)
	clearRenderState(true);

}

bool TextView::isUrl(QString txt)
{
    if (txt.isEmpty())
	return false;
    /*if (txt[0] == 'w')
	return true;
    return false;*/
    text_word_check ((char*)txt.latin1());

}

void TextView::paintText(int x, int y, int w, int h, QString text,
			  QPainter * p)
{
//kdDebug()    << "paintText: "<<text<<endl;

    if (text.isEmpty()
	|| m_buffer->skiprender)
	return;

    p->setFont(*(m_buffer->font));	// TODO: only set if curfont != font

    //if (m_buffer->renderState->bg)
//	p->fillRect(x, y - m_buffer->lineHeight + m_buffer->lineDescent, w,
//		    h, QBrush(convColor(m_buffer->renderState->bg)));


if (m_buffer->renderState->underline)
{
p->drawLine(x, y + 1, x + w - 1, y + 1);
    p->setPen(convColor(m_buffer->renderState->fg));
}

p->setPen(Qt::black);
if (m_buffer->renderState->underline)

p->drawLine(x, y + 2, x + w - 1, y + 2);
    p->drawText(x+1, y+1, text);
   
   if (m_buffer->renderState->bold)
	  p->drawText(x+2, y+1, text);

    p->setPen(convColor(m_buffer->renderState->fg));
    p->drawText(x, y, text);

    
    if (m_buffer->renderState->bold)
	p->drawText(x + 1, y, text);

}


QColor TextView::convColor(ushort col)
{
    QColor ret = *(m_palette[col]);
	
   /* switch (col) {
	//case 0: already white.
    case 1:
	ret = Qt::white;
	break;
    case 2:
	ret = Qt::darkBlue;
	break;
    case 3:
	ret = Qt::darkGreen;
	break;
    case 4:
	ret = Qt::red;
	break;
    case 5:
	ret = Qt::darkRed;
	break;
    case 6:
	ret = Qt::darkMagenta;
	break;
    case 7:
	ret = QColor("orange");
	break;
    case 8:
	ret = Qt::yellow;
	break;
    case 9:
	ret = Qt::green;
	break;
    case 10:
	ret = Qt::darkCyan;
	break;
    case 11:
	ret = Qt::cyan;
	break;
    case 12:
	ret = Qt::blue;
	break;
    case 13:
	ret = Qt::magenta;
	break;
    case 14:
	ret = Qt::darkGray;
	break;
    case 15:
	ret = Qt::lightGray;
	break;
    }*/
    return ret;
}

void TextView::recalcLayout()
{
    int ret = 0;
	m_buffer->layoutEnabled=false;

    TextEntry *ent = m_buffer->startEnt;

    while (ent) {
	ret = ret + recalcEntryLayout(ent);
	ent = ent->next;
    }
    m_buffer->textHeight = ret;
    resizeView(ret);
	m_buffer->layoutEnabled=true;
}

void TextView::resizeView(int h)
{
    int w = width();

    if (h < height()) {
	h = height() - 5;
	if (m_doubleBuffer)
	    m_buffer->paintDirty = true;
    }
    else
	h = h + 5;
    if (contentsHeight() != h) {
	resizeContents(w, h);
	if (m_doubleBuffer) {
	    //kdDebug() << "resizeView:: h="<<h<< " "<<m_buffer->pixmap->height()
	    /*int neww= w>m_buffer->pixmap->width()? w : m_buffer->pixmap->width();
	       int newh=h>m_buffer->pixmap->height()? h : m_buffer->pixmap->height();       
	       if ((newhh != m_buffer->pixmap->height()) 
	       ||   (neww != m_buffer->pixmap->width()))
	     */

	    if ((h > m_buffer->pixmap->height()) ||
		(w > m_buffer->pixmap->width()))
		m_buffer->pixmap->resize(w, h);
	    m_buffer->paintDirty = true;
	}
    }
}

QString TextView::wrapText(QString & text, int wrapLen, bool clearState)
{
    int i = 0, curLen = 0, lastSpace = -1, tlen = text.length();
    QString ret = QString::null;
    QChar c;

    if (tlen == 0)
	return QString::null;

    while ((tlen > i) && (wrapLen > curLen)) {
	c = text.at(i);
	if ((m_buffer->renderState->parsingCol && c.isDigit() &&
	     (m_buffer->renderState->numCol < 2)) ||
	    (m_buffer->renderState->parsingCol && c == ',' &&
	     text.at(i + 1).isDigit() &&
	     (m_buffer->renderState->numCol < 3))) {
	    if (c == ',')
		m_buffer->renderState->numCol = 0;
	    else
		m_buffer->renderState->numCol++;
	}
	else {
	    m_buffer->renderState->parsingCol = false;
	    if (c == ATTR_COLOR) {
		m_buffer->renderState->parsingCol = true;
		m_buffer->renderState->numCol = 0;
		i++;
	    }
	    else if ((c == ATTR_BEEP)
		     || (c == ATTR_RESET) || (c == ATTR_REVERSE) ||
		     (c == ATTR_BOLD) || (c == ATTR_UNDERLINE)) {
		;
	    }
	    else {
		curLen = curLen + charWidth(c);
		if (c.isSpace())
		    lastSpace = i;
	    }
	}
	i++;
    }

    if (curLen < wrapLen) {
	ret = text;
	text = QString::null;
    }

    else if (lastSpace == -1 || (lastSpace < (i - m_wrapBoundary))) {
	ret = text.left(i);	// no wrap
	text = text.mid(i);
    }

    else {
	ret = text.left(lastSpace + 1);
	text = text.mid(lastSpace + 1);
    }

    if (clearState)
	clearRenderState();

    return ret;

}

void TextView::scrollDown()
{
    if (m_scrollType == AutoScroll);	//TODO
    //if m_contentsY()==contentsHeight() - visibleHeight():
    else if (m_scrollType == AlwaysScroll)
	setContentsPos(contentsX(), contentsHeight() - visibleHeight());
    else
	return;
}


int TextView::textWidth(QString text)
{
    int i = 0, nc = 0, ret = 0, tlen = text.length();
    bool col = false, cont = false;
    QChar c;

    if (tlen == 0)
	return 0;
    while (tlen > i) {
	cont = false;
	c = text.at(i);
	if ((col && c.isDigit() && nc < 2) ||
	    (col && c == ',' && text.at(i + 1).isDigit() && nc < 3)) {
	    if (c == ',')
		nc = 0;
	    else
		nc++;
	}
	else {
	    col = false;
	    if (c == ATTR_COLOR) {
		col = true;
		nc = 0;
	    }
	    else if ((c == ATTR_BEEP)
		     || (c == ATTR_RESET) ||
		     (c == ATTR_REVERSE) || (c == ATTR_BOLD) ||
		     (c == ATTR_UNDERLINE));
	    else
		ret += charWidth(c);
	}
	i++;
    }
    return ret;
}

int TextView::charWidth(QChar c)
{
    return m_buffer->fm->width(c);
}


TextEntry *TextView::findEntAt(int x, int y, bool exact)
{

    TextEntry *ent = m_buffer->pageTopEnt;
    int tstartX, twidth, tstartY, theight, av;
    TextEntry *endEnt =
	m_buffer->pageBottomEnt ? m_buffer->pageBottomEnt->next : 0;

    while ((ent != endEnt) && ent) {
	tstartX = ent->leftX;
	twidth = ent->leftWidth;
	tstartY = ent->leftY;
	theight = m_buffer->lineHeight;

	if (exact && (tstartX < x) && (tstartX + twidth > x) &&
	    (tstartY < y) && (tstartY + theight > y))
	    return ent;
	else if ((tstartX + twidth > x) && (tstartY < y) &&
		 (tstartY + theight > y))
	    return ent;

	tstartX = ent->rightX;
	av = visibleWidth() - tstartX;

	if ((ent->linesTaken == 1) || (av < m_minWrapWidth)) {
	    if (exact && (tstartX < x) &&
		(tstartX + textWidth(ent->rightText) > x) && (tstartY < y)
		&& (tstartY + theight > y))
		return ent;
	    else if ((tstartY < y) and(tstartY + theight > y))
		return ent;
	}
	else {
	    QString text = ent->rightText;
	    int subLines = 0;
	    QString curText = QString::null;

	    while (subLines < ent->linesTaken) {
		curText = wrapText(text, av, 0);
		if (exact && (tstartX < x) &&
		    (tstartX + textWidth(curText) > x) &&
		    (tstartY < y) and(tstartY + theight > y))
		    return ent;
		else if ((tstartY < y) and(tstartY + theight > y))
		    return ent;

		subLines++;

		tstartX = m_buffer->sepValue + m_sepWidth + m_marginWidth;
		av = visibleWidth() - tstartX;
		tstartY = tstartY + m_buffer->lineHeight;
	    }
	}
	ent = ent->next;
    }
    return 0;
}

	//find nth offset in ent where x exists
int TextView::findOffsetAt(TextEntry * ent, int x, int y)
{
    int i = findCharAtX(ent->leftText, x - ent->leftX);

    if (i != -1)
	return i;

    int av = visibleWidth() - ent->rightX;

    if ((ent->linesTaken == 1) || (av < m_minWrapWidth)) {
	i = findCharAtX(ent->rightText, x - ent->rightX);
	if (i != -1)
	    return ent->leftText.length() + i;
    }
    else			//dowrap
    {
	QString text = ent->rightText;
	int subLines = 0;
	int curLen = 0;
	QString curText = QString::null;

	int tstartX = ent->rightX;
	int tstartY = ent->leftY;	// leftY= same vert coords as rightY

	while (subLines < ent->linesTaken) {
	    curText = wrapText(text, av, 0);

	    if ((tstartY < y) && (tstartY + m_buffer->lineHeight > y)) {
		i = findCharAtX(curText, x - tstartX);
		if (i != -1)
		    return ent->leftText.length() + i + curLen;
	    }

	    curLen = curLen + curText.length();
	    tstartX = m_buffer->sepValue + m_sepWidth + m_marginWidth;
	    av = visibleWidth() - tstartX;
	    tstartY = tstartY + m_buffer->lineHeight;
	    subLines++;
	}
    }
    return -1;
}

int TextView::findCharAtX(QString text, int x)
{
    int i = 0, nc = 0, ret = 0, tlen = text.length();
    bool col = false, cont = false;
    QChar c;

    if (tlen == 0)
	return 0;

    while ((tlen > i) && (x > ret)) {
	cont = false;
	c = text.at(i);

	if ((col && c.isDigit() && nc < 2) ||
	    (col && c == ',' & text.at(i + 1).isDigit() & nc < 3)) {
	    if (c == ',')
		nc = 0;
	    else
		nc++;
	}
	else {
	    col = false;
	    if (c == ATTR_COLOR) {
		col = true;
		nc = 0;
	    }
	    else if ((c == ATTR_BEEP)
		     || (c == ATTR_RESET) ||
		     (c == ATTR_REVERSE) ||
		     (c == ATTR_BOLD) or(c == ATTR_UNDERLINE));

	    else
		ret = ret + charWidth(c);
	}
	i++;
    }

    if (x > ret)
	return -1;

    return i;

}

void TextView::updateSelection()
{
    int lowX, lowY, highX, highY;
    int startOffset, endOffset;
    TextEntry *selStartEnt, *selEndEnt;

    if (m_buffer->selStartY > m_buffer->selEndY) {
	lowX = m_buffer->selEndX;
	lowY = m_buffer->selEndY;
	highX = m_buffer->selStartX;
	highY = m_buffer->selStartY;
    }
    else {
	lowX = m_buffer->selStartX;
	lowY = m_buffer->selStartY;
	highX = m_buffer->selEndX;
	highY = m_buffer->selEndY;
    }
    selStartEnt = findEntAt(lowX, lowY);

    if (selStartEnt)
	startOffset = findOffsetAt(selStartEnt, lowX, lowY);
    else
	return;

    selEndEnt = findEntAt(lowX, lowY);
    if (selEndEnt)
	endOffset = findOffsetAt(selEndEnt, highX, highY);
    else {
	selEndEnt = m_buffer->endEnt;
	if (selEndEnt)
	    endOffset =
		selEndEnt->rightText.length() +
		selEndEnt->leftText.length();
	else
	    return;
    }

    if ((selStartEnt == selEndEnt) && (startOffset > endOffset)) {
	int tmp = startOffset;

	startOffset = endOffset;
	endOffset = tmp;
    }

    if ((m_buffer->lastStartEnt == selStartEnt) &&
	(m_buffer->lastEndEnt == selEndEnt) &&
	(m_buffer->lastStartOffset == startOffset) &&
	(m_buffer->lastEndOffset == endOffset))
	return;

    clearSelection();

    selStartEnt->selStartOffset = startOffset;
    selStartEnt->selEndOffset = endOffset;

    if (selStartEnt != selEndEnt) {
	selStartEnt->selEndOffset =
	    selStartEnt->rightText.length() +
	    selStartEnt->leftText.length();

	if (endOffset != 0) {
	    selEndEnt->selStartOffset = 0;
	    selEndEnt->selEndOffset = endOffset;
	}
	TextEntry *ent = m_buffer->startEnt->next;

	while (ent && (ent != m_buffer->endEnt)) {
	    ent->selStartOffset = 0;
	    ent->selEndOffset =
		ent->rightText.length() + ent->leftText.length();
	    ent = ent->next;
	}
    }

    drawSelection(selStartEnt, startOffset, selEndEnt, endOffset);
}


void TextView::drawSelection(TextEntry * startEnt, int startOffset,
			      TextEntry * endEnt, int endOffset)
{
    QPainter *p = new QPainter();
    TextEntry *ent;
    int start, end;

    kdDebug() << "drawing selection from " << startEnt->
	leftText << " offset=" << startOffset << " to" << endEnt->
	leftText << " offset=" << endOffset << endl;

    p->begin(viewport());
    if ((m_buffer->lastStartEnt == 0) && (startEnt == endEnt)) {
	m_buffer->lastStartOffset = startOffset;
	m_buffer->lastEndOffset = endOffset;
	goto lamejump;
    }
    if ((m_buffer->lastStartEnt == startEnt) &&
	(m_buffer->lastEndEnt == endEnt)) {
	if ((m_buffer->lastStartOffset == startOffset) ||
	    (m_buffer->lastEndOffset == endOffset)) {
	  lamejump:
	    ent = endEnt;
	    if (endOffset > m_buffer->lastEndOffset) {
		end = endOffset;
		start = m_buffer->lastEndOffset;
	    }
	    else if (endOffset < m_buffer->lastEndOffset) {
		end = m_buffer->lastEndOffset;
		start = endOffset;
	    }
	    else if (startOffset < m_buffer->lastStartOffset) {
		end = m_buffer->lastStartOffset;
		start = startOffset;
		ent = startEnt;
	    }
	    else if (startOffset > m_buffer->lastStartOffset) {
		end = startOffset;
		start = m_buffer->lastStartOffset;
		ent = startEnt;
	    }
	    else {
		end = endOffset;
		start = startOffset;
	    }
	}
	else {
	    ent = startEnt;
	    if ((m_buffer->lastStartOffset == startOffset))
		ent = endEnt;
	    end = QMAX(m_buffer->lastEndOffset, endOffset);
	    start = QMIN(m_buffer->lastStartOffset, startOffset);
	}
	paintEntry(ent, p, -1, start, end, 1);
    }

    m_buffer->lastStartEnt = startEnt;
    m_buffer->lastEndEnt = endEnt;
    m_buffer->lastStartOffset = startOffset;
    m_buffer->lastEndOffset = endOffset;

    p->end();
}

bool TextView::clearSelection()
{
    TextEntry *ent = m_buffer->lastStartEnt;
    bool ret = false;

    while (ent) {
	if (ent->selStartOffset != -1)
	    ret = true;
	ent->selStartOffset = -1;
	ent->selEndOffset = -1;

	if (ent == m_buffer->lastEndEnt)
	    break;

	ent = ent->next;
    }
    return ret;
}

void TextView::contentsMousePressEvent(QMouseEvent * e)
{
    if (e->button() == Qt::LeftButton) {
	int x = e->x();

	if ((x > m_buffer->sepValue - m_sepWidth - 3) &&
	    (x < m_buffer->sepValue + m_sepWidth + 3)) {
	    m_buffer->sepActive = true;
	    if (!m_doubleBuffer)
		m_buffer->onlySep = true;
	    updateContents(m_buffer->sepValue, contentsY(),
			   m_sepWidth, visibleHeight());
	}

	else {
	    if ((m_buffer->selStartX == x) &&
		(m_buffer->selStartY == e->y())) {
		if (clearSelection()) {
		    TextEntry *ent = m_buffer->lastStartEnt;
		    QPainter *p = new QPainter();

		    while (ent != m_buffer->lastEndEnt)
			paintEntry(ent, p, -1);
		    p->end();
		    m_buffer->lastStartEnt = 0;
		    m_buffer->lastEndEnt = 0;
		}
	    }
	    m_buffer->selStartX = x;
	    m_buffer->selStartY = e->y();
	    m_buffer->hasSelection = true;

	}
    }
}

void TextView::contentsMouseMoveEvent(QMouseEvent * e)
{
    int x = e->x();

    if (e->state() == Qt::LeftButton) {
	if (m_buffer->sepActive) {
	    //if ((x > 10) && ((visibleWidth() - 20) > x)) {
	    if ((x > 10) && ((width() / 2) > x)) {
		int d = myabs(m_buffer->sepValue - x);

		//kdDebug()<< "d="<<d<<endl;
		if (d > 30) {
		    m_buffer->sepValue = x;
		    m_buffer->paintDirty = true;
		    recalcLayout();
		    updateContents(contentsX(),
				   contentsY(),
				   visibleWidth(), visibleHeight());
		}
	    }
	}
	else if (m_buffer->hasSelection) {
	    m_buffer->selEndX = x;
	    m_buffer->selEndY = e->y();
	    updateSelection();
	}
    }

    else {
	if ((x >
	     (m_buffer->sepValue -
	      m_sepWidth)) and(x < (m_buffer->sepValue + m_sepWidth))) {
	    //TODO: only setCursor on change.. need to cache
	    viewport()->setCursor(*m_cursorSplit);
	}
	else {
	    viewport()->setCursor(*m_cursorIbeam);
	}
    }

}

void TextView::contentsMouseReleaseEvent(QMouseEvent * e)
{
    if (m_buffer->sepActive) {
	m_buffer->sepActive = false;
	int a = contentsHeight();

	recalcLayout();
	if (a != contentsHeight()) {
	    updateContents(contentsX(),
			   contentsY(), visibleWidth(), visibleHeight());
	}
	else {
	    m_buffer->paintDirty = true;
	    updateContents(m_buffer->sepValue,
			   contentsY(), m_sepWidth, visibleHeight());
	}
    }
}

void TextView::contentsKeyPressEvent(QKeyEvent * e)
{
    switch (e->key()) {
    case Key_Down:
	setContentsPos(contentsX(), contentsY() + 15);
	break;
    case Key_Up:
	setContentsPos(contentsX(), contentsY() - 15);
	break;
    case Key_PageUp:
	setContentsPos(contentsX(), contentsY() - visibleHeight());
	break;
    case Key_PageDown:
	setContentsPos(contentsX(), contentsY() + visibleHeight());
	break;
    case Key_Home:
	setContentsPos(contentsX(), 0);
	break;
    case Key_End:
	setContentsPos(contentsX(), contentsHeight() - visibleHeight());
	break;
    default:
	break;
    }
}

void TextView::drawContents(QPainter * p, int clipx, int clipy,
			     int clipw, int cliph)
{
    if (m_doubleBuffer) {
	if (m_buffer->paintDirty) {
	    QPainter *pa = new QPainter();

	    pa->begin(m_buffer->pixmap);
	    pa->fillRect(0, 0, contentsWidth(), contentsHeight(),
			 QBrush(convColor(19)));
	    //pa->fillRect(clipx, clipy, clipw, cliph, QBrush(Qt::white));
	    paintArea(pa);
	    pa->end();
	    p->drawPixmap(clipx, clipy, *(m_buffer->pixmap),
			  clipx, clipy, clipw, cliph + 5);
	    m_buffer->paintDirty = false;
	}
	else {
	    p->drawPixmap(clipx, clipy, *(m_buffer->pixmap),
			  clipx, clipy, clipw, cliph + 5);
	}
    }
    else
	paintArea(p);
}
