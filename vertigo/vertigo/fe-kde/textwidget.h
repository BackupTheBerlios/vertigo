#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H


#include <time.h>
#include <qwidget.h>
#include <qscrollview.h>
#include <qptrlist.h>
#include <qpainter.h>
#include <qtimer.h>
#include <qpixmap.h>
#include <qintdict.h>

#define ATTR_BOLD '\002'
#define ATTR_COLOR '\003'
#define ATTR_BEEP '\007'
#define ATTR_RESET '\017'
#define ATTR_REVERSE '\026'
#define ATTR_UNDERLINE '\037'


class QScrollView;


typedef QIntDict<QColor> Palette;

typedef struct TextEntry {
    TextEntry *next;
    QString timeStamp;
    QString rightText;
    QString leftText;
    uint leftWidth;
    uint leftX;
    uint leftY;
    uint rightX;
    uint rightWidth;
    uint linesTaken;
    int selStartOffset;
    int selEndOffset;

};

typedef struct TextRenderState {
    ushort bg;
    ushort fg;
    bool parsingCol;
    ushort numCol;
    bool parsingBG;
    bool bold;
    bool underline;
};

typedef struct TextBuffer {
    QFont *font;
    QFontMetrics *fm;
    uint sepValue;
    ulong textHeight;
    uint lineHeight;
    uint lineDescent;

    bool onlySep;
    bool sepActive;
    bool hasSelection;
    bool verticalSliderPressed;
    bool skiprender;

	bool layoutEnabled;
	int layoutTimerId;

    TextEntry *startEnt;
    TextEntry *endEnt;

    TextEntry *lastStartEnt;
    TextEntry *lastEndEnt;
    TextEntry *pageTopEnt;
    TextEntry *pageBottomEnt;
    uint lastStartOffset;
    uint lastEndOffset;

    uint selStartX;
    uint selStartY;
    uint selEndX;
    uint selEndY;

    uint numEnts;

    QPixmap *pixmap;
    bool paintDirty;

    TextRenderState *renderState;
};

#define isReverseLayout() QApplication::reverseLayout()

/*
class STextViewportWidget : public QWidget{
public:
	STextViewportWidget (QWidget * parent);
	~STextViewportWidget();
};
*/



class TextView:public QScrollView {
  Q_OBJECT public:

    enum TextScrollType { NoScroll, AutoScroll, AlwaysScroll };

     TextView(QWidget * parent = 0, const char *name = 0);

    void setFontName(const QString & name);
    void clearBuffer(bool resetFonts = true, bool resetRenderState = true);
    void appendLines(const QString & rawLines, bool scroll = true);
    void appendText(QString text, bool indent, bool scroll = true);
    void appendTextIndent(QString r, QString l, bool scroll = true);
    void newTextEntry(QString r, QString l = QString::null, bool scroll =
		      true);
    void clear();
  protected:
    void viewportResizeEvent(QResizeEvent * pe);
    void clearRenderState(bool attribs = false);
    int recalcEntryLayout(TextEntry * ent, bool resizeCols = false);
    void paintArea(QPainter * p);
    int paintEntry(TextEntry * ent,
		   QPainter * p, int y = -1,
		   int selStart = -1, int selEnd = -1, bool onlysel =
		   false);
    void paintTextChunk(QString text, int x, int y, QPainter * p,
			bool clearState = true, int selstart =
			-1, int selend = -1, bool dourls =
			true, bool onlysel = false);

    void scheduleLayout();
	void timerEvent ( QTimerEvent *e );

    bool isUrl(QString txt);
    void paintText(int x, int y, int w, int h, QString text, QPainter * p);

    QColor convColor(ushort col);
    void recalcLayout();

    void resizeView(int h);
    QString wrapText(QString & text, int wrapLen, bool clearState = true);
    void scrollDown();
    int textWidth(QString text);
    int charWidth(QChar c);
    TextEntry *findEntAt(int x, int y, bool exact = false);
    int findOffsetAt(TextEntry * ent, int x, int y);
    int findCharAtX(QString text, int x);
    void updateSelection();
    void drawSelection(TextEntry * startEnt, int startOffset,
		       TextEntry * endEnt, int endOffset);
    bool clearSelection();


    void contentsMousePressEvent(QMouseEvent * e);
    void contentsMouseMoveEvent(QMouseEvent * e);
    void contentsMouseReleaseEvent(QMouseEvent * e);
    void contentsKeyPressEvent(QKeyEvent * e);

    void drawContents(QPainter * p, int clipx, int clipy,
		      int clipw, int cliph);

  private:

     uint m_maxEntries;
    uint m_sepWidth;
    ushort m_marginWidth;
    uint m_maxSubLines;
    uint m_minWrapWidth;
    uint m_wrapBoundary;
    bool m_autoResizeColumns;
    TextScrollType m_scrollType;
    bool m_doubleBuffer;
    bool m_showTimeStamp;
    bool m_styledSep;
    QString m_timeStampFormat;

    TextBuffer *m_buffer;
    Palette m_palette;

    QCursor *m_cursorSplit;
    QCursor *m_cursorIbeam;

};

#endif
