#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H


#include <qscrollview.h>
#include <qptrlist.h>
#include <qstringlist.h>

class QString;
class QCursor;


#define ATTR_BOLD '\002'
#define ATTR_COLOR '\003'
#define ATTR_BEEP '\007'
#define ATTR_RESET '\017'
#define ATTR_REVERSE '\026'
#define ATTR_UNDERLINE '\037'

typedef struct TextRenderState {
    ushort bg;
    ushort fg;
    bool parsingCol;
    ushort numCol;
    bool parsingBG;
    bool bold;
    bool underline;

};

class TextEntry {
  public:
	TextEntry(QString leftText, QString rightText=QString::null);
	void setLeftRegion(const QRegion &region);
	QRegion leftRegion();
	void addRightRegion(const QRegion & region);
	QRegion rightRegion();
	void clearRightRegion();
	int linesTaken();
	void setLinesTaken(int a);
	void addRightWrap(const QString &string);
	void clearRightWrap();
	QStringList rightWrapList();
	QString text (const int col);

  private:
  	QString m_leftText;
	QString m_rightText;
	QStringList m_textWrap;
	QRegion m_leftRegion;
	QRegion m_rightRegion;
	int m_linesTaken;
};

typedef struct TextBuffer {
    QFont *font;
    QFontMetrics *fm;
    int sepValue;
    bool sepActive;
    ulong textHeight;
    int lineHeight;
    int lineDescent;

    bool layoutEnabled;
	int layoutTimerId;


    QPixmap *pixmap;
    QPainter *painter;

    QRegion sepRegion;
    TextRenderState *renderState;
};


class TextView:public QScrollView {
  Q_OBJECT
  public:

      enum TextScrollType { NoScroll, AutoScroll, AlwaysScroll };
  TextView(QWidget * parent = 0, const char *name = 0);
	~TextView();

void clear();
  void setFontName(const QString & name);

	int textWidth(const QString &text);
	int charWidth(const QChar &c);



void viewportPaintEvent ( QPaintEvent * pe );

   void appendLines(const QString & rawLines, bool scroll=false);
void appendText(const QString &text, bool indent, bool scroll=false);

  void appendTextIndent(const QString &leftText, const QString &rightText, bool scroll=false);

    void scrollDown();
protected:
void clearBuffer();
   void clearRenderState(bool attribs = false);
   QString wrapText(QString & text, int wrapLen, int &curLen, bool clearState = true);
  int recalcEntryLayout(TextEntry * ent, bool resizeCols=false);
void recalcLayout();
void scheduleLayout();
void timerEvent ( QTimerEvent *e );
void viewportResizeEvent(QResizeEvent * pe);
void resizeView(int h);

void paintText(int x, int y, int w, int h, QString text,
			  QPainter *p);
QColor convColor(ushort col);
void paintTextChunk(QString text, int x, int y, QPainter * p,
			       bool clearState=true);

void contentsMousePressEvent(QMouseEvent * e);
void contentsMouseMoveEvent(QMouseEvent * e);
void contentsMouseReleaseEvent(QMouseEvent * e);
  private:
  	QPtrList<TextEntry> m_entryList;
	TextBuffer *m_buffer;
	int m_sepWidth;
	int m_margin;
	int m_maxSubLines;
    	int m_minWrapWidth;
	int m_scrollType;
	bool m_showTimeStamp;
	int m_wrapBoundary;
	bool m_reversedLayout;
	bool m_doubleBuffer;
	bool m_autoResizeColumns;
	    QCursor *m_cursorSplit;
    QCursor *m_cursorIbeam;
	QCursor *m_currentCursor;
	bool m_relayoutBeforePaint;
};

#endif
