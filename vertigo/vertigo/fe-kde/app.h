#ifndef APP_H
#define APP_H

#include <kapplication.h>
#include <kcharselect.h>
#include <kprogress.h>

#include <qptrlist.h>
#include <qtimer.h>
#include <qsocketnotifier.h>

#include <qpushbutton.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qlineedit.h>
#include <qdialog.h>
#include <qcolor.h>
#include <qintdict.h>

#include "../common/xchat.h"
#include "../common/userlist.h"

typedef QIntDict<QColor> Palette;
#define xchatapp (static_cast<App*>(kapp))

class Serverlist;
class URLGrabberView;
class ChatListView;
class XferView;
class NotifyListView;
class IgnoreListView;
class BanListView;
class MainWindow;
class MainView;


typedef int (*TimerFunc) (void *data);
typedef int (*SocketFunc) (void *source, int con, void *data);
typedef void (*StringInputFunc) (int cancel, char *val, void *data);
typedef void (*IntInputFunc) (int cancel, int val, void *data);

typedef struct TimerData {
    TimerFunc func;
    void *args;
    QTimer *timer;
} TimerData;


typedef struct SocketData {
    SocketFunc func;
    int id;
    int con;
    void *args;
    QSocketNotifier *readn;
    QSocketNotifier *writen;
    QSocketNotifier *excen;
} SocketData;


class App:public KApplication {
  Q_OBJECT public:
     App();

    Serverlist *serverList();
	KCharSelect *charChart();
	URLGrabberView *urlGrabber();
	ChatListView *chatList();
	XferView *transfers();
	NotifyListView *notifyList();
	IgnoreListView *ignoreList();
	BanListView *banList();

	void loadDefaultPalette();
	void loadPalette();

	Palette palette();
	
	int getUserLevel(QPixmap *p);

    int addTimeoutFunction(int interval, void *fn, void *args);
    void removeTimeoutFunction(int timerId);
    void addIdleFunction(void *fn, void *args);
    int addInputFunction(int fd, int flags, void *func, void *data);
    void removeInputFunction(int id);

    void getInt(char *prompt, int def, void *callback, void *ud);
    void getString(char *prompt, char *def, void *callback, void *ud);

    void loadUserIcons();
    QPixmap *getUserIcon(server * serv, User * user);

    MainWindow *createNewWindow();
    MainWindow *getMainWindow(bool force);
    void removeMainWindow();
    void removeServerlistWindow();

    int windowCount();

	void setUrlGrabber(URLGrabberView* v);
	void setChatList(ChatListView*v);
	void setTransfers(XferView*v);
	void setNotifyList(NotifyListView* nl);
	void setBanList(BanListView* v);
    
	public slots:void execTimeoutFunction();
    void execIdleFunction();
    void socketReady(int);
    void dialogInputHandled();

	private:
    QPtrList < TimerData > *m_idleList;
    QPtrList < TimerData > *m_timeoutList;
    QPtrList < SocketData > *m_socketList;
    int m_mainWindowCount;
    int m_otherWindowCount;

	int m_nextSocketID;

    QPixmap *m_opPix;
    QPixmap *m_hopPix;
    QPixmap *m_voicePix;
    QPixmap *m_redPix;
    QPixmap *m_purplePix;

    Serverlist *m_serverList;
	Palette m_palette;
	KCharSelect *m_charChart;
    URLGrabberView *m_urlGrabber;
    ChatListView *m_chatList;
    XferView *m_transfers;
    NotifyListView *m_notifyList;
    IgnoreListView *m_ignoreList;
    BanListView *m_banList;

};


class CleanupHandler:public QObject {
  Q_OBJECT public:
     CleanupHandler();
    KProgress *m_progress;
    QTimer *m_timer;
    void cleanup();

    public slots:void startCleanupLoop();
    void forwardLoop();
};

class InputDialog:public QDialog {
  Q_OBJECT public:
    enum DialogType { IntDialog = 0, StringDialog };

     InputDialog(QObject * parent, char *prompt, int def,
		      void *callback, void *ud);
     InputDialog(QObject * parent, char *prompt, char *def,
		      void *callback, void *ud);

    ~InputDialog();

    IntInputFunc getIntFunc() {
	return m_intFunc;
    } StringInputFunc getStringFunc() {
	return m_stringFunc;
    }
    DialogType getType() {
	return m_type;
    }
    QString getString() {
	return lineEdit1->text();
    }
    void *getUserData() {
	return m_userData;
    }
    int getInt() {
	return spinBox1->value();
    }


    int cancelled() {
	if (m_cancelled) {
	    return 1;
	}
	return 0;
    }

  protected:


    void setup(QString prompt, QString def);

    QVBoxLayout *Form2Layout;
    QHBoxLayout *layout3;
    QHBoxLayout *layout1;

    QLabel *textLabel1;
    QLineEdit *lineEdit1;
    QSpinBox *spinBox1;
    QPushButton *pushButton1;
    QPushButton *pushButton2;


    bool m_cancelled;
    DialogType m_type;
    void *m_userData;

    IntInputFunc m_intFunc;
    StringInputFunc m_stringFunc;

    public slots:void okClicked();
    void cancelledClicked();

  signals:
    void dialogDone();

};

#endif

