#ifndef MainView_H
#define MainView_H


#include <dcopobject.h>

#include "mainview.h"
#include "containerview.h"

#include "../common/xchat.h"
#include "../common/userlist.h"

class KListView;
class QPushButton;
class QSplitter;
class QVBoxLayout;
class QHBoxLayout;

class MainView;
class TopicEdit;
class InputLineEdit;
class TextView;


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


class MainView: public ContainerView, virtual public MainViewIface {
  Q_OBJECT public:
     MainView(QWidget * s, session * s);
    ~MainView();

MainWindow *window();
    void setWindow(MainWindow *w);
	void closeView();
	
    void clearUserlist();
    void showUserlist();
    void hideUserlist();
    void insertUser(server * s, User * u, int index, int sep);
    bool removeUser(User * u);
	void rehashUser(User *u);
	

    void setTopic(QString t);

    TextView *getTextView() {
	return m_textView;
    }

    void setSession(session * s) {
	m_session = s;
    }

    session *getSession() {
	return m_session;
    }

    public slots:void topicEnter();
    void entryEnter();
    void entryTab();
    void entryDown();
    void entryUp();
    void completionMatches(const QStringList &);

	private slots:
	    void setActiveView();

    
  protected:

	session *m_session;

    KListView *m_userList;
   int m_oldTopicParaPos;
       int m_oldTopicIndexPos;

TopicEdit* m_topicEdit;
    QPushButton* channelButton;
    QSplitter* m_splitter;
    TextView* m_textView;
    InputLineEdit* m_inputEdit;
    QPushButton* textButton;
    QWidget* m_userlistPanel;
        QVBoxLayout* Form1Layout;
    QHBoxLayout* layout1;
    QVBoxLayout* layout5;
    QHBoxLayout* layout3;
};



#endif	
