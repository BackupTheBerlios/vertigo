#ifndef XChatMainView_H
#define XChatMainView_H

#include <qvariant.h>
#include <klistbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qwidgetstack.h>
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
class KListBox;
class QListBoxItem;


class XChatMainView;
class XChatUserlistView;
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
    QTextEdit *topicEdit;
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



class XChatUserlistView:public KListBox {
  Q_OBJECT public:
     XChatUserlistView(QWidget * parent);
    ~XChatUserlistView();
    bool event(QEvent * e);
    const QColor & alternateBackground() const;
QSize minimumSizeHint() const;
	
  private:
     QColor m_alternateBackground;
    //void resizeEvent(QResizeEvent * r);
};


class XChatUserlistItem:public QListBoxItem {
  public:
    XChatUserlistItem(XChatUserlistView * list, QPixmap * pix, User * u);
    ~XChatUserlistItem();

    bool isAlternate();

    User *getUser() {
	return m_user;
    }
    void paint(QPainter * painter);
    int height(const QListBox * lb) const;
    int width(const QListBox * lb) const;

    bool m_known;
    bool m_odd;


  private:
    User * m_user;
    QPixmap *m_pixmap;
};


#endif				// XChatMainView_H
