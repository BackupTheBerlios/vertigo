#ifndef XChatMainView_H
#define XChatMainView_H

#include <qvariant.h>
#include <klistview.h>
#include <qlabel.h>
#include <qwidgetstack.h>
#include <qlineedit.h>
#include <qtextedit.h>

#include <ksqueezedtextlabel.h>


#include <qdialog.h>
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QWidgetStack;
class QTabWidget;
class QWidget;
class QLabel;
class QPixmap;
class QPushButton;
class QSplitter;
class QGrid;
class QLineEdit;
class QTextEdit;
class KListBox;
class QListBoxItem;


class XChatMainView;
class XChatUserlistView;
class XChatTopicEdit;
class XChatInputLineEdit;
#include <dcopobject.h>

#include "extraview.h"
#include "textwidget.h"

#include "../common/xchat.h"
#include "../common/userlist.h"


 class XChatMainViewIface : virtual public DCOPObject
 {
     K_DCOP

protected:
    XChatMainViewIface( QCString objId ) : DCOPObject( objId ) {}

k_dcop:

        virtual void clearUserlist() = 0;
        virtual void showUserlist() =0;
        virtual void hideUserlist() =0;
};


class XChatMainView:public QWidget, public XChatContainerView, virtual public XChatMainViewIface {
  Q_OBJECT public:
     XChatMainView(XChatMainWindow * s, session * s);
    ~XChatMainView();

XChatMainWindow *window();
    void setWindow(XChatMainWindow *w);

    void clearUserlist();
    void showUserlist();
    void hideUserlist();
    void insertUser(server * s, User * u, int index, int sep);
    bool removeUser(User * u);
	void rehashUser(User *u);
	

    void setTopic(QString t);

    Vertigo::TextView *getTextView() {
	return textEdit1;
    }
    void setSession(session * s) {
	m_session = s;
    }

    session *getSession() {
	return m_session;
    }
    void giveInputFocus();

    public slots:void topicEnter();
    void entryEnter();
    void entryTab();
    void entryDown();
    void entryUp();
    void completionMatches(const QStringList &);

  protected:
    QVBoxLayout * Form1Layout;
    QVBoxLayout *tabLayout;
    QHBoxLayout *layout1;
    QVBoxLayout *layout5;
    QHBoxLayout *layout3;

    QVBoxLayout *layout4;

    // bool m_used;
    QGridLayout *gridLayout;
    QPushButton *pushButton5;
    QSplitter *splitter1;
    Vertigo::TextView *textEdit1;
    XChatInputLineEdit *m_inputLine;
    QPushButton *pushButton1;
    XChatUserlistView *m_userList;
    QPushButton *pushButton10_2_2;
    QWidget *m_textPanel;
    QWidget *m_userlistPanel;
    XChatTopicEdit *topicEdit;
    session *m_session;
};

/*
class XChatWidgetStack:public QWidgetStack {
  Q_OBJECT public:
     XChatWidgetStack(QWidget * parent);
    ~XChatWidgetStack();
QSize sizeHint () const;
};*/

class XChatTopicEdit:public QTextEdit {
  Q_OBJECT public:
     XChatTopicEdit(QWidget * parent);
    ~XChatTopicEdit();
	//QSize sizeHint() const;
	void setTopicText(QString s);
	void focusInEvent ( QFocusEvent * );
	void focusOutEvent ( QFocusEvent * );
QSize sizeHint() const;
QSize minimumSizeHint() const;
	private:
	QString m_topicText;
};

class XChatInputLineEdit:public QLineEdit {
  Q_OBJECT public:
     XChatInputLineEdit(QWidget * parent);
    ~XChatInputLineEdit();
    bool eventFilter(QObject * o, QEvent * e);
    
     signals:void tabPressed();
    void upPressed();
    void downPressed();

};



class XChatUserlistView:public KListView {
  Q_OBJECT public:
     XChatUserlistView(QWidget * parent);
    ~XChatUserlistView();
//QSize minimumSizeHint() const;
	
  //private:
    // QColor m_alternateBackground;
    //void resizeEvent(QResizeEvent * r);
};


class XChatUserlistItem:public KListViewItem {
  public:
    XChatUserlistItem(XChatUserlistView * list, QPixmap * pix, User * u);
    ~XChatUserlistItem();

 //   bool isAlternate();
QPixmap *getPixmap();

    User *getUser() {
	return m_user;
    }

	void setUser(User *user);

int compare ( QListViewItem * i, int col, bool ascending ) const;
void paintCell ( QPainter * p, const QColorGroup & cg, int col, int width, int align );

   // void paint(QPainter * painter);
    //int height(const QListBox * lb) const;
    //int width(const QListBox * lb) const;

    //bool m_known;
    //bool m_odd;


  private:
    User * m_user;
	QPixmap *m_pixmap;
};


#endif				// XChatMainView_H
