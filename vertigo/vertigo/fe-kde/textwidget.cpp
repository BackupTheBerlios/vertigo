#include "textwidget.h"
#include <qscrollview.h>

#include <qwidget.h>
#include <qbrush.h>
#include <qcolor.h>
#include <qcursor.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qptrlist.h>
#include <qapplication.h>
#include <qdatetime.h>

TextEntry::TextEntry(QString leftText, QString rightText)
{
    m_leftText = leftText;
    m_rightText = rightText;
    m_linesTaken=0;
	m_leftX=m_y=m_rightX=0;
}

void
  TextEntry::addRightWrap(const QString &string)
{
	m_textWrap.append(string);
}

 void
  TextEntry::clearRightWrap()
 {
	m_textWrap.clear();
 }

 QStringList
  TextEntry::rightWrapList()
 {
	return m_textWrap;
 }


int TextEntry::linesTaken()
{
	return m_linesTaken;
}


QString TextEntry::text (const int col)
{
	if (col==0)
	{
		return m_leftText;
	}
	return m_rightText;
}

void
  TextEntry::setLinesTaken(int a)
 {
	m_linesTaken=a;
 }

void 
  TextEntry::setLeftX( int x)
{
	m_leftX=x;
}

void
  TextEntry::setRightX( int x)
{
    m_rightX=x;
}

void
  TextEntry::setY( int ay)
{
    m_y=ay;
}

int TextEntry::leftX()
{
	return m_leftX;
}

int TextEntry::rightX()
{
	return m_rightX;
}

int TextEntry::y()
{
	return m_y;
}


void TextEntry::setTimeStamp(const QString &s)
{
	m_timeStamp=s;
}

QString TextEntry::timeStamp()
{
    return m_timeStamp;
}


TextView::TextView(QWidget * parent, const char *name):QScrollView(parent, name,
	   WStaticContents |WResizeNoErase |
	   WRepaintNoErase)
{
	m_relayoutBeforePaint=false;
	viewport()->setMouseTracking(true);
    m_sepWidth = 1;
    m_margin = 1;
    m_maxSubLines = 6;
    m_minWrapWidth = 30;
    m_wrapBoundary = 15;

	m_showTimeStamp = true;
    m_timeStampFormat = "[hh:mm]";

    m_cursorSplit = new QCursor(Qt::SplitHCursor);
    m_cursorIbeam = new QCursor(Qt::IbeamCursor);
	m_currentCursor=m_cursorIbeam;
	
	m_updateSepOnly=false;
	m_scrollType=AlwaysScroll;
    m_autoResizeColumns=false;
    m_doubleBuffer=true;
    m_reversedLayout=QApplication::reverseLayout();

    m_buffer = new TextBuffer;
    m_buffer->renderState = new TextRenderState;
    clearBuffer();
    clearRenderState(true);

    if (m_doubleBuffer) {
	m_buffer->pixmap = new QPixmap(14,14);
	m_buffer->pixmap->setOptimization(QPixmap::BestOptim);
    }

    m_entryList.setAutoDelete(true);
    viewport()->setBackgroundMode(QWidget::NoBackground);

    setHScrollBarMode(QScrollView::AlwaysOff);
    setDragAutoScroll(false);
    viewport()->setFocusProxy( this );
    viewport()->setFocusPolicy( WheelFocus );

    setFontName("Verdana 10");
}

TextView::~TextView()
{
	delete m_cursorSplit;
	delete m_cursorIbeam;
	delete m_buffer->renderState;
	delete m_buffer->pixmap;
	delete m_buffer;
}

// TODO: delete buffer, etc..

void TextView::clear()
{
    int s = m_buffer->sepValue;

    clearBuffer();
    clearRenderState();
    m_buffer->sepValue = s;
    recalcLayout();

    viewport()->repaint();
}

void TextView::clearBuffer()
{
	m_buffer->sepValue = 150;
    m_buffer->sepActive=false;
    m_buffer->textHeight = 0;
    m_buffer->font = 0;
    m_buffer->fm = 0;
    m_buffer->layoutEnabled=true;
    m_buffer->layoutTimerId=0;
	m_buffer->sepEnd=false;
	m_buffer->updatesEnabled=true;
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

void TextView::scrollDown()
{
    if (m_scrollType == AutoScroll);	//TODO
    //if m_contentsY()==contentsHeight() - visibleHeight():
    else if (m_scrollType == AlwaysScroll)
	setContentsPos(contentsX(), contentsHeight() - visibleHeight());
    else
	return;
}

void TextView::resizeView(int h)
{
    int w = width();
	//qWarning("resizeView h=%d height=%d",h,height());
    if (h < height()) {
	/*h = height() - 10;
        if (contentsHeight() != h) {
    resizeContents(w, h);
    }*/
	}
    else
	//h = h + 5;
    if (contentsHeight() != h) {
/*    if (m_buffer->sepEnd)
            {
                setContentsPos(contentsX(), contentsHeight() - visibleHeight());
            }*/

	m_buffer->updatesEnabled=false;
	resizeContents(w, h); // may cause flicker.. 
	m_buffer->updatesEnabled=true;

    }
}


void TextView::viewportResizeEvent(QResizeEvent * pe)
{
   //qWarning("viewport resize sizew=%d sizeh=%d oldsizew=%d oldsizeh=%d",pe->size().width(), pe->size().height(), pe->oldSize().width(), pe->oldSize().height());
  //  QScrollView::resizeEvenEnvironmentst(pe);
    if (!m_buffer->sepActive && m_buffer->layoutEnabled) {
		scheduleLayout();
    }
}

void TextView::recalcLayout()
{
    //int ret = 0;
	m_buffer->layoutEnabled=false;

    TextEntry *ent;
    m_buffer->textHeight=0;
    for (ent = m_entryList.first(); ent; ent = m_entryList.next()) {
	m_buffer->textHeight = m_buffer->textHeight + recalcEntryLayout(ent);
    }

   // m_buffer->textHeight = ret;
    resizeView(m_buffer->textHeight);
    m_buffer->layoutEnabled=true;
}

void TextView::scheduleLayout()
{
	if (!m_buffer->layoutTimerId)
		m_buffer->layoutTimerId = startTimer( 20 );

}

void TextView::timerEvent ( QTimerEvent *e )
{
   // kdDebug() << "timer event " << e->timerId() << endl;
    killTimer(e->timerId());
	if ( e->timerId() == m_buffer->layoutTimerId ) {
		m_buffer->layoutTimerId=0;
		recalcLayout();
		            updateContents(0,
                   contentsY(),
                   visibleWidth(), visibleHeight());

		//repaint();
	}
}

void
  TextView::setFontName(const QString & name)
{
    int
	i = name.findRev(' ');
    int
	s = 10;

    if (m_buffer->font) {
	delete
	    m_buffer->
	    font;
	delete
	    m_buffer->
	    fm;
    }
    m_buffer->font = new QFont();
    m_buffer->font->setFamily(name.left(i));

    if (i != -1)
	s = name.mid(i + 1).toInt();

    m_buffer->font->setPointSize(s);
    m_buffer->fm = new QFontMetrics(*(m_buffer->font));
    m_buffer->lineHeight = m_buffer->fm->lineSpacing();
    m_buffer->lineDescent =
	m_buffer->fm->descent() + 1 + m_buffer->fm->leading();

}


int
  TextView::textWidth(const QString & text)
{
    int
	i = 0, nc = 0, ret = 0, tlen = text.length();
    bool
	col = false, cont = false;
    QChar
	c;

    if (tlen == 0)
	return 0;
    while (tlen > i) {
	cont = false;
	c = text.at(i);
	if ((col && c.isDigit() && nc < 2)
	    || (col && c == ',' && text.at(i + 1).isDigit() && nc < 3)) {
	    if (c == ',')
		nc = 0;
	    else
		nc++;
	} else {
	    col = false;
	    if (c == ATTR_COLOR) {
		col = true;
		nc = 0;
	    } else if ((c == ATTR_BEEP)
		       || (c == ATTR_RESET) || (c == ATTR_REVERSE)
		       || (c == ATTR_BOLD) || (c == ATTR_UNDERLINE)) ;
	    else
		ret += charWidth(c);
	}
	i++;
    }
    return ret;
}

int
  TextView::charWidth(const QChar & c)
{
    return m_buffer->fm->width(c);
}

QString TextView::wrapText(QString & text, int wrapLen, int &curLen,  bool clearState)
{
    int i = 0, lastSpace = -1, tlen = text.length();

    QString ret = QString::null;
    QChar c;

    curLen=0;
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

    else if (lastSpace == -1 
			|| (lastSpace < (i - m_wrapBoundary))) {
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

void TextView::appendText(const QString &text, bool indent, bool scroll)
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
	appendTextIndent(text, QString::null, scroll);
}



void

    TextView::appendTextIndent(const QString & leftText,
			       const QString & rightText, bool scroll)
{
    TextEntry *ent = new TextEntry(rightText, leftText);
 	m_entryList.append(ent);

    int oldHeight = m_buffer->textHeight;
    int i = recalcEntryLayout(ent, m_autoResizeColumns);

    if (i == -1)		// sep moved-- need to relayout from scratch
	recalcLayout();
    else {
	m_buffer->textHeight += i;
	resizeView(m_buffer->textHeight);
    }

    QTime t = QTime::currentTime();

    ent->setTimeStamp(t.toString(m_timeStampFormat));

    if (m_buffer->textHeight < visibleHeight())
	if (i == -1)
	    updateContents();
	else
	    updateContents(0, oldHeight, visibleWidth(),
			   m_buffer->textHeight - oldHeight);
    else {
	if (scroll)
	    scrollDown();
    }
}


void TextView::keyPressEvent(QKeyEvent * e)
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
	          updateContents(0,
                   contentsY(),
                   visibleWidth(), visibleHeight());


}





int TextView::recalcEntryLayout(TextEntry * ent, bool resizeCols)
{
    int w = QMAX(1, textWidth(ent->text(0)));
    int leftX = 0, rightX=0;

    if (m_reversedLayout) {
    	if (w + m_margin >= visibleWidth()-m_buffer->sepValue) {
		if (resizeCols) {
			m_buffer->sepValue = -1*(w + m_margin * 2)-visibleWidth();
			//if (m_showTimeStamp)
		   	//	 m_buffer->sepValue -= textWidth(ent->timeStamp()) * 2;
			return -1;
		}
		/*else
		{

		}*/
	}
	//leftX = m_buffer->sepValue + m_sepWidth + m_margin;
	//startX = QMIN(startX, visibleWidth() - m_margin - w);
    } else {
	if (w + m_margin >= m_buffer->sepValue) {
	    if (resizeCols) {
		m_buffer->sepValue = w + m_margin * 2;
		//if (m_showTimeStamp)
		//    m_buffer->sepValue += textWidth(ent->timeStamp()) * 2;
		return -1;
	    }
	    else {
		leftX = m_margin;
		rightX = 2*m_margin+w+m_sepWidth;
	    }
	}
	else {
	    leftX =
		m_buffer->sepValue - w - m_margin;
	    rightX = m_buffer->sepValue + m_sepWidth + m_margin;
	}
    }

    ent->
	setLeftX( leftX);
	ent->setY( m_buffer->textHeight);


	ent->clearRightWrap();

    if (m_reversedLayout) {
    	//rightX=1;         // XXX: fix QMIN/QMAX case..
	w = m_buffer->sepValue - m_margin;
    } else {
    	//rightX=m_buffer->sepValue + m_sepWidth + m_margin;
	w = visibleWidth() - rightX  -m_margin*2;
    }

	//qWarning("w=%d m_minWrapWidth=%d visW=%d rightX=%d", w, m_minWrapWidth, width(),rightX);

    if (w < m_minWrapWidth) {

	ent->setLinesTaken(1);
	ent->addRightWrap(ent->text(1));
	ent->setRightX(rightX);
	return m_buffer->lineHeight;
    } else {
	int ret = 0;
	QString r = ent->text(1);
	int subLines = 0;
	int curTextWidth=0;

	while ((subLines < m_maxSubLines) && (r.isEmpty() == 0)) {
		ent->addRightWrap(wrapText(r, w, curTextWidth));
	    ret = ret + m_buffer->lineHeight;
	    subLines = subLines + 1;
		//qWarning("addRightRegion rightX=%d h=%d", rightX, ret-m_buffer->lineHeight);
	    if (m_reversedLayout) {
		w = m_buffer->sepValue - m_margin;
	    } else {
		w = visibleWidth() - (m_buffer->sepValue + m_sepWidth +
				      m_margin);
	    }
	}
	ent->setLinesTaken(subLines);
	ent->setRightX(rightX);
	return ret;
    }

  // should never reach here
    return -1;
}


void TextView::paintTextChunk(QString text, int x, int y, QPainter &p,
			       bool clearState)
{
    QChar c;
    int i = 0, curLen = 0, tlen = text.length();
    int colNum = 0, oldbg = 0, oldfg = 1;

    QString col = QString::null;
    QString curText = QString::null;

    int lastSpace = 0;

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
	    else if ((c == ATTR_BEEP) || (c == '\n'))
	    ;
	    else {
		curLen = curLen + charWidth(c);
		curText = curText.append(c);
		//i++;
	    }
	    i++;


	}
    }


	if (curLen)
	    paintText(x, y, curLen, m_buffer->lineHeight, curText, p);


    if (clearState)
	clearRenderState(true);

}



void TextView::paintText(int x, int y, int w, int h, QString text,
			  QPainter &p)
{

    if (text.isEmpty())
	return;
//qWarning("painting text x=%d y=%d w=%d h=%d",x,y,w,h);
 p.setPen(convColor(m_buffer->renderState->fg));
if (m_buffer->renderState->underline)
{
p.drawLine(x, y + 1, x + w - 1, y + 1);
}

    p.drawText(x, y, text);

    if (m_buffer->renderState->bold)
	p.drawText(x + 1, y, text);

}


QColor TextView::convColor(ushort col)
{
QColor ret;
  switch (col) {
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
	default:
	ret=Qt::white;
    }
    return ret;
}


void TextView::drawSeperator(QPainter &p, int y, int h)
{
	if (m_buffer->sepActive)
        p.fillRect(m_buffer->sepValue,y,m_sepWidth,h, QBrush(Qt::white));
    else
        p.fillRect(m_buffer->sepValue,y,m_sepWidth,h, QBrush(Qt::gray));
}

void TextView::viewportPaintEvent ( QPaintEvent * pe )
{
	//QRegion unpainted=pe->region();
	QRect ur=pe->rect();//unpainted.boundingRect();

    TextEntry *ent;
	QRect lr;
	bool painted=false;
	int a=0,le=0,h,dummy;
	QString stamp;
	QPainter p(viewport());
	p.setFont(*(m_buffer->font));

	//int pp=0,ppp=0;


    if (!m_buffer->updatesEnabled)
    {
        return;
    }

	if (m_updateSepOnly)
	{
		m_updateSepOnly=false;
		drawSeperator(p, ur.y(), ur.height());
		return;
	}


	if (m_relayoutBeforePaint)
	{
		recalcLayout();
		m_relayoutBeforePaint=false;
	}

    /*if (m_buffer->sepRegion.isNull()) {
		m_buffer->sepRegion = QRegion(m_buffer->sepValue, 0, m_sepWidth, visibleHeight());
    }*/


    if (m_buffer->textHeight < visibleHeight() )                                    {                                                                                   p.fillRect(0, m_buffer->textHeight-contentsY(),visibleWidth(), visibleHeight()-m_buffer->textHeight+contentsY(),QBrush(Qt::black));                             drawSeperator(p,m_buffer->textHeight-contentsY(), visibleHeight()-m_buffer->textHeight+contentsY());                                                        }


    for (ent = m_entryList.last(); ent; ent = m_entryList.prev()) {
		a=ent->y()-contentsY();
		dummy=0;
		le=ent->linesTaken()*m_buffer->lineHeight;
		//qWarning("painting: lr.y=%d ur.y=%d lr.height=%d ur.height=%d text=%s",lr.y(),ur.y(), lr.height(), ur.height(), ent->text(0).latin1());

	
		if (( a < ur.y()-(m_buffer->lineHeight*m_maxSubLines)) || (a+le> ur.y()+ur.height()+(m_buffer->lineHeight*m_maxSubLines)))
		{
		if (painted)
		{
			//qWarning("done painting.. y=%d h=%d  ur=%d urh=%d",lr.y(),ur.y(),ur.y(),ur.height());
			goto done;
		}
		else
		{
			//ppp++;
			continue;
		}
		}
		//pp++;
		if (painted==false)
			painted=true;
		
		h=a;
		if (m_doubleBuffer)
			{
				m_buffer->pixmap->resize(QMAX(m_buffer->pixmap->width(), visibleWidth()),
						QMAX(m_buffer->pixmap->height(), le));
				m_buffer->painter.begin(m_buffer->pixmap, false);
				m_buffer->painter.setFont(*(m_buffer->font));
				m_buffer->painter.fillRect(0,0,visibleWidth(), le, QBrush(Qt::black));
				if (m_showTimeStamp) {
					stamp = ent->timeStamp();
					paintTextChunk(wrapText(stamp, ent->leftX()-m_margin*2, dummy), m_margin, m_buffer->lineHeight-m_buffer->lineDescent, m_buffer->painter);
				}
				paintTextChunk(ent->text(0), ent->leftX(), m_buffer->lineHeight-m_buffer->lineDescent, m_buffer->painter, true);
				QStringList strings=ent->rightWrapList();
				h=0;
				for ( int i = 0; i < strings.size(); i++ )
				{		
					paintTextChunk(strings[i], ent->rightX(), h+m_buffer->lineHeight-m_buffer->lineDescent, m_buffer->painter, false);
					h=h+m_buffer->lineHeight;
				}
				drawSeperator(m_buffer->painter, 0, le);
				m_buffer->painter.end();
				p.drawPixmap(0, a, *(m_buffer->pixmap), 0, 0, visibleWidth(), le);
				clearRenderState(true);
		}
		else
		{
			p.fillRect(0,a,visibleWidth(), le, QBrush(Qt::black));
			if (m_showTimeStamp) {
                    stamp = ent->timeStamp();
                    paintTextChunk(wrapText(stamp, ent->leftX()-m_margin*2,dummy), m_margin, a+m_buffer->lineHeight-m_buffer->lineDescent, p);
            }
			paintTextChunk(ent->text(0), ent->leftX(), a+m_buffer->lineHeight-m_buffer->lineDescent, p,true);
			QStringList strings=ent->rightWrapList();
			for ( int i = 0; i < strings.size(); i++ )
            {
                paintTextChunk(strings[i], ent->rightX(), h+m_buffer->lineHeight-m_buffer->lineDescent, p,false);
                h=h+m_buffer->lineHeight;
			}
			drawSeperator(p, a, le);
			clearRenderState(true);
		}
    }

done:
//	qWarning("textHeight=%d   visibleHeight=%d width=%d visiblewidth=%d",m_buffer->textHeight , visibleHeight(),width(),visibleWidth());

	return;
}

int myabs(int a)
{
    if (a < 0)
	return (-1 * a);
    return a;
}

void TextView::contentsMousePressEvent(QMouseEvent * e)
{
    if (e->button() == Qt::LeftButton) {
	int x = e->x();
	if ((x > m_buffer->sepValue - m_sepWidth - 3) &&
	    (x < m_buffer->sepValue + m_sepWidth + 3)) {
	    m_buffer->sepActive = true;
		m_updateSepOnly=true;
	    updateContents(m_buffer->sepValue, contentsY(),
			   m_sepWidth, visibleHeight());
		if (contentsY()+visibleHeight()==contentsHeight())
			m_buffer->sepEnd=true;
	}
    }
}

void TextView::contentsMouseMoveEvent(QMouseEvent * e)
{
    int x = e->x();

    if (e->state() == Qt::LeftButton) {
	if (m_buffer->sepActive) {
	    if ((x > 10) && ((width() / 2) > x)) {
		int d = myabs(m_buffer->sepValue - x);
		if (d > 5) {
			m_buffer->sepValue = x;
			//scheduleLayout();
			recalcLayout();
			/*if (m_buffer->sepEnd)
			{
				setContentsPos(contentsX(), contentsHeight() - visibleHeight());
			}*/
			updateContents(0,
				   contentsY(),
				   visibleWidth(), visibleHeight());
			
		}
	    }
	}
    }

    else {
	if ((x >
	     (m_buffer->sepValue -
	      m_sepWidth-2)) && (x < (m_buffer->sepValue + m_sepWidth+2))) {
		if (m_currentCursor != m_cursorSplit )	
		{
			viewport()->setCursor(*m_cursorSplit);
			m_currentCursor=m_cursorSplit;
		}
	}
	else {
        if (m_currentCursor != m_cursorIbeam )
        {
			 viewport()->setCursor(*m_cursorIbeam);
			m_currentCursor=m_cursorIbeam;
		}
    }
	}

}

void TextView::contentsMouseReleaseEvent(QMouseEvent * e)
{
    if (m_buffer->sepActive) {
	m_buffer->sepActive = false;
		m_updateSepOnly=true;
		m_buffer->sepEnd=false;
        updateContents(m_buffer->sepValue, contentsY(),
               m_sepWidth, visibleHeight());
	}
}
