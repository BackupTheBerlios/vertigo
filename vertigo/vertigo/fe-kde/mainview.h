#ifndef MainView_H
#define MainView_H

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

#include <dcopobject.h>

#include "extraview.h"
#include "textwidget.h"

#include "../common/xchat.h"
#include "../common/userlist.h"

class MainView;
class UserlistView;
class TopicEdit;
class InputLineEdit;


 class MainViewIface : virtual public DCOPObject
 {
     K_DCOP

protected:
    MainViewIface( QCString objId ) : DCOPObject( objId ) {}

k_dcop:

        virtual void clearUserlist() = 0;
        virtual void showUserlist() =0;
        virtual void hideUserlist() =0;
};


class MainView:public QWidget, public ContainerView, virtual public MainViewIface {
  Q_OBJECT public:
     MainView(MainWindow * s, session * s);
    ~MainView();

MainWindow *window();
    void setWindow(MainWindow *w);

    void clearUserlist();
    void showUserlist();
    void hideUserlist();
    void insertUser(server * s, User * u, int index, int sep);
    bool removeUser(User * u);
	void rehashUser(User *u);
	

    void setTopic(QString t);

    TextView *getTextView() {
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
    TextView *textEdit1;
    InputLineEdit *m_inputLine;
    QPushButton *pushButton1;
    UserlistView *m_userList;
    QPushButton *pushButton10_2_2;
    QWidget *m_textPanel;
    QWidget *m_userlistPanel;
    TopicEdit *topicEdit;
    session *m_session;
   int m_oldTopicParaPos;
       int m_oldTopicIndexPos;
	   

};

/*
class WidgetStack:public QWidgetStack {
  Q_OBJECT public:
     WidgetStack(QWidget * parent);
    ~WidgetStack();
QSize sizeHint () const;
};*/

class TopicEdit:public QTextEdit {
  Q_OBJECT public:
     TopicEdit(QWidget * parent);
    ~TopicEdit();
	//QSize sizeHint() const;
	void setTopicText(QString s);
	void focusInEvent ( QFocusEvent * );
	void focusOutEvent ( QFocusEvent * );
QSize sizeHint() const;
QSize minimumSizeHint() const;
	void keyPressEvent( QKeyEvent *e );
	
	private:
	QString m_topicText;
};

class InputLineEdit:public QLineEdit {
  Q_OBJECT public:
     InputLineEdit(QWidget * parent);
    ~InputLineEdit();
    bool eventFilter(QObject * o, QEvent * e);
    
     signals:void tabPressed();
    void upPressed();
    void downPressed();

};



class UserlistView:public KListView {
  Q_OBJECT public:
     UserlistView(QWidget * parent);
    ~UserlistView();
//QSize minimumSizeHint() const;
	
  //private:
    // QColor m_alternateBackground;
    //void resizeEvent(QResizeEvent * r);
};


class UserlistItem:public KListViewItem {
  public:
    UserlistItem(UserlistView * list, QPixmap * pix, User * u);
    ~UserlistItem();

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


#endif				// MainView_H
