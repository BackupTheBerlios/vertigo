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


TextEntry::TextEntry(QString leftText, QString rightText)
{
    m_leftText = leftText;
    m_rightText = rightText;
    m_linesTaken=0;
   // m_rightRegion=QRegion(); // qt 3.1 bug..
   // m_textWrap=QStringList();
}


void
  TextEntry::setLeftRegion(const QRegion & region)
{
    m_leftRegion = region;
}


QRegion TextEntry::leftRegion()
{
    return m_leftRegion;
}


void
  TextEntry::addRightRegion(const QRegion & region)
{
    m_rightRegion = m_rightRegion.unite(region);
}

void
  TextEntry::clearRightRegion()
 {
	m_rightRegion=QRegion();
 }


QRegion TextEntry::rightRegion()
{
    return m_rightRegion;
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


uint TextEntry::linesTaken()
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
  TextEntry::setLinesTaken(uint a)
 {
	m_linesTaken=a;
 }

TextView::TextView(QWidget * parent, const char *name):QScrollView(parent, name,
	   WResizeNoErase |
	   WRepaintNoErase)
{

 viewport()->setMouseTracking(true);
    m_sepWidth = 2;
    m_margin = 1;
    m_maxSubLines = 6;
    m_minWrapWidth = 30;
    m_wrapBoundary = 15;

    m_cursorSplit = new QCursor(Qt::SplitHCursor);
    m_cursorIbeam = new QCursor(Qt::IbeamCursor);

m_scrollType=AlwaysScroll;
    m_autoResizeColumns=false;
    m_doubleBuffer= true;
    m_reversedLayout=QApplication::reverseLayout();

    m_buffer = new TextBuffer;
    m_buffer->renderState = new TextRenderState;
    clearBuffer();
    clearRenderState(true);
    //m_buffer->sepRegion=QRegion(m_buffer->sepValue, 0, m_sepWidth, visibleHeight());

    if (m_doubleBuffer) {
	m_buffer->pixmap = new QPixmap(128,128);
	m_buffer->painter = new QPainter();
    }


    m_entryList.setAutoDelete(true);
    viewport()->setBackgroundMode(QWidget::NoBackground);

    setHScrollBarMode(QScrollView::AlwaysOff);
    setFontName("Verdana 10");

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
    m_buffer->layoutEnabled=false;
    m_buffer->layoutTimerId=0;


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

    if (h < height()) {
	h = height() - 5;
    }
    else
	h = h + 5;
    if (contentsHeight() != h) {
	resizeContents(w, h);
    }
}


void TextView::viewportResizeEvent(QResizeEvent * pe)
{
   qWarning("viewport resize");
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
		m_buffer->sepRegion=QRegion();
		recalcLayout();
		updateContents();
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
    TextEntry *ent = new TextEntry(leftText, rightText);
 	m_entryList.append(ent);

    int oldHeight = m_buffer->textHeight;
    int i = recalcEntryLayout(ent, m_autoResizeColumns);

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
	    updateContents(0, oldHeight, visibleWidth(),
			   m_buffer->textHeight - oldHeight);
    else {
	if (scroll)
	    scrollDown();
    }
}

int TextView::recalcEntryLayout(TextEntry * ent, bool resizeCols)
{
    int w = textWidth(ent->text(0));
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
	setLeftRegion(QRegion
		      (leftX, m_buffer->textHeight, w,
		       m_buffer->lineHeight));


	ent->clearRightRegion();
	ent->clearRightWrap();

    if (m_reversedLayout) {
    	//rightX=1;         // XXX: fix QMIN/QMAX case..
	w = m_buffer->sepValue - m_margin;
    } else {
    	//rightX=m_buffer->sepValue + m_sepWidth + m_margin;
	w = visibleWidth() - rightX;
    }


    if (w < m_minWrapWidth) {
	ent->setLinesTaken(1);
	ent->addRightRegion(QRegion(rightX, m_buffer->textHeight, textWidth(ent->text(1)),  m_buffer->lineHeight));
	return m_buffer->lineHeight;
    } else {
	int ret = 0;
	QString r = ent->text(1);
	int subLines = 0;
	int curTextWidth=0;

	while ((subLines < m_maxSubLines) and(r.isEmpty() == 0)) {
	    ent->addRightWrap(wrapText(r, w, curTextWidth));
	    ret = ret + m_buffer->lineHeight;
	    subLines = subLines + 1;
	    ent->addRightRegion(QRegion(rightX, m_buffer->textHeight, curTextWidth,  ret));
	    if (m_reversedLayout) {
		w = m_buffer->sepValue - m_margin;
	    } else {
		w = visibleWidth() - (m_buffer->sepValue + m_sepWidth +
				      m_margin);
	    }
	}
	ent->setLinesTaken(subLines);
	return ret;
    }

  // should never reach here
    return -1;
}


void TextView::paintTextChunk(QString text, int x, int y, QPainter * p,
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
			  QPainter * p)
{

    if (text.isEmpty())
	return;
//qWarning("painting text x=%d y=%d w=%d h=%d",x,y,w,h);
    p->setFont(*(m_buffer->font));	// TODO: only set if curfont != font
 p->setPen(convColor(m_buffer->renderState->fg));
if (m_buffer->renderState->underline)
{
p->drawLine(x, y + 1, x + w - 1, y + 1);
}


    p->drawText(x, y, text);


    if (m_buffer->renderState->bold)
	p->drawText(x + 1, y, text);

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
    }
    return ret;
}

void

    TextView::drawContents(QPainter * p, int clipx, int clipy, int clipw,
			   int cliph)
{
//contentsToViewport(clipx,clipy,clipx,clipy);
    QRegion unpainted(clipx, clipy, clipw, cliph);
    //unpainted.translate(0, -clipy);
   // unpainted += QRegion();
//qWarning("draw =x=%d y=%d w=%d h=%d",clipx, clipy, clipw, cliph);

    TextEntry *ent;
	QRect lr;
	int oly;

    if (m_buffer->sepRegion.isNull()) {
	m_buffer->sepRegion =
	    QRegion(m_buffer->sepValue, 0, m_sepWidth, visibleHeight());
    }




    for (ent = m_entryList.first(); ent; ent = m_entryList.next()) {
    	//QMemArray<QRect> a=ent->rightRegion().rects();
	lr=ent->leftRegion().boundingRect();
	if (( lr.y() < clipy-20) || (lr.y()+lr.height()> clipy+cliph+20))
		continue;

	//qWarning("lr=x=%d y=%d w=%d h=%d",lr.x(), lr.y(), lr.width(), lr.height());
	if (m_doubleBuffer)
    	{
    		m_buffer->pixmap->resize(QMAX(m_buffer->pixmap->width(), lr.width()),
					QMAX(m_buffer->pixmap->height(), lr.height()));
		//m_buffer->pixmap->fill(Qt::blue);
		m_buffer->painter->begin(m_buffer->pixmap, false);

		m_buffer->painter->fillRect(0,0, lr.width(), lr.height(), QBrush(Qt::blue));
		paintTextChunk(ent->text(0), 0, m_buffer->lineHeight-m_buffer->lineDescent, m_buffer->painter);
		m_buffer->painter->end();
		//m_buffer->pixmap->save("test.png","PNG");
		p->setClipRegion(ent->leftRegion());
		lr.moveBy(0, clipy);
		p->drawPixmap(lr.x(), lr.y(), *(m_buffer->pixmap), 0, 0, lr.width(), lr.height()  );
    	}
	else
	{
		//p->setClipRegion(ent->leftRegion());
		//lr.moveBy(0, clipy);
		p->fillRect(lr, QBrush(Qt::yellow));
		//paintTextChunk(ent->text(0), lr.x(), lr.y()+m_buffer->lineHeight-m_buffer->lineDescent,p);
	}
	unpainted -= ent->leftRegion();

	lr=ent->rightRegion().boundingRect();
	//qWarning("rr=x=%d y=%d w=%d h=%d",lr.x(), lr.y(), lr.width(), lr.height());
	if (m_doubleBuffer)
    	{
    		m_buffer->pixmap->resize(QMAX(m_buffer->pixmap->width(), lr.width()),
					QMAX(m_buffer->pixmap->height(), lr.height()));
		m_buffer->painter->begin(m_buffer->pixmap, false);
		QRegion a=ent->rightRegion();
		a.translate(-lr.x(), -lr.y());
		m_buffer->painter->setClipRegion(a);
		m_buffer->painter->fillRect(0,0,lr.width(),lr.height(), QBrush(Qt::red));
		QMemArray<QRect> rects = a.rects();
		QStringList strings=ent->rightWrapList();
		for ( int i = 0; i < rects.size(); i++ )
		{
			//m_buffer->painter->setPen(Qt::black);
			//m_buffer->painter->drawRect(rects[i]);
			paintTextChunk(strings[i], 0, rects[i].y()+m_buffer->lineHeight-m_buffer->lineDescent, m_buffer->painter);

		}
		//paintTextChunk(ent->text(1), 0, lr.y()+m_buffer->lineHeight-m_buffer->lineDescent,m_buffer->painter);
		m_buffer->painter->end();
		//m_buffer->pixmap->save("test.png","PNG");
		p->setClipRegion(ent->rightRegion());
		p->drawPixmap(lr.x(), lr.y(), *(m_buffer->pixmap), 0, 0, lr.width()+lr.x(), lr.height()  );
    	}
	else{
		//p->setClipRegion(ent->rightRegion());
		p->fillRect(lr, QBrush(Qt::red));
		QMemArray<QRect> rects = ent->rightRegion().rects();
		QStringList strings=ent->rightWrapList();
		for ( int i = 0; i < rects.size(); i++ )
		{
			//m_buffer->painter->setPen(Qt::black);
			//m_buffer->painter->drawRect(rects[i]);
			paintTextChunk(strings[i], rects[i].x(), rects[i].y()+m_buffer->lineHeight-m_buffer->lineDescent,p);

		}

		//paintTextChunk(ent->text(1), lr.x(), lr.y()+m_buffer->lineHeight-m_buffer->lineDescent,p);
 }
	unpainted -=ent->rightRegion();

    }


QRegion sep=m_buffer->sepRegion;
//qWarning("sep=x=%d y=%d w=%d h=%d",sep.boundingRect().x(), sep.boundingRect().y(), sep.boundingRect().width(), sep.boundingRect().height());
       p->setClipRegion(sep);
    p->fillRect(sep.boundingRect(), QBrush(Qt::green));
unpainted -= m_buffer->sepRegion;

unpainted.translate(0, -clipy);//-unpainted.boundingRect().y());
    p->setClipRegion(unpainted);
   // qWarning("up=x=%d y=%d w=%d h=%d",unpainted.boundingRect().x(), unpainted.boundingRect().y(), unpainted.boundingRect().width(), unpainted.boundingRect().height());
   // p->setPen(Qt::black);
  // p->drawRect(unpainted.boundingRect());
  //p->translate(0,clipy);
  p->fillRect(0,0,unpainted.boundingRect().width(), unpainted.boundingRect().height(), QBrush(Qt::red));
  //  p->fillRect(unpainted.boundingRect(), QBrush(Qt::red));


//sep.translate(0,-unpainted.boundingRect().y());

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
	    updateContents(m_buffer->sepValue, contentsY(),
			   m_sepWidth, visibleHeight());
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
			m_buffer->sepRegion =
	    QRegion(m_buffer->sepValue, 0, m_sepWidth, contentsHeight());

		    recalcLayout();
		    updateContents(contentsX(),
				   contentsY(),
				   visibleWidth(), contentsHeight());
		}
	    }
	}
    }

    else {
	if ((x >
	     (m_buffer->sepValue -
	      m_sepWidth)) && (x < (m_buffer->sepValue + m_sepWidth))) {
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
	/*int a = contentsHeight();

	recalcLayout();
	if (a != contentsHeight()) {
	    updateContents(contentsX(),
			   contentsY(), visibleWidth(), visibleHeight());
	}
	else {
	    updateContents(m_buffer->sepValue,
			   contentsY(), m_sepWidth, visibleHeight());
	}*/
    }
}
