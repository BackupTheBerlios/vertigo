#ifndef APP_H
#define APP_H

#include <kapplication.h>
#include <qptrlist.h>
#include <qtimer.h>
#include <qsocketnotifier.h>

#include <qpushbutton.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtimer.h>
#include <kprogress.h>
#include <qlineedit.h>
#include <qdialog.h>
#include <qcolor.h>
#include <qintdict.h>
#include <kcharselect.h>

#define xchatapp (static_cast<XChatApp*>(kapp))



#include "../common/xchat.h"
#include "../common/userlist.h"

#include "mainview.h"
#include "mainwindow.h"
#include "serverlist.h"
#include "textwidget.h"
#include "extraview.h"


class XChatServerlist;
class XChatURLGrabberView;
class XChatChatListView;
class XChatXferView;
class XChatNotifyListView;
class XChatIgnoreListView;
class XChatBanListView;
class KCharSelect;




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


class XChatApp:public KApplication {
  Q_OBJECT public:
     XChatApp();

    XChatServerlist *serverList();
KCharSelect *charChart();
XChatURLGrabberView *urlGrabber();
	XChatChatListView *chatList();
	XChatXferView *transfers();
	XChatNotifyListView *notifyList();
	XChatIgnoreListView *ignoreList();
	XChatBanListView *banList();

void loadDefaultPalette();
void loadPalette();
   
XChatPalette palette();

    int addTimeoutFunction(int interval, void *fn, void *args);
    void removeTimeoutFunction(int timerId);
    void addIdleFunction(void *fn, void *args);
    int addInputFunction(int fd, int flags, void *func, void *data);
    void removeInputFunction(int id);

    void getInt(char *prompt, int def, void *callback, void *ud);
    void getString(char *prompt, char *def, void *callback, void *ud);

    void loadUserIcons();
    QPixmap *getUserIcon(server * serv, User * user);

    XChatMainWindow *createNewWindow();
    XChatMainWindow *getMainWindow(bool force);
    void removeMainWindow();
    void removeServerlistWindow();

    int windowCount();

void setUrlGrabber(XChatURLGrabberView* v);
void setChatList(XChatChatListView*v);
void setTransfers(XChatXferView*v);
void setNotifyList(XChatNotifyListView* nl);
void setBanList(XChatBanListView* v);






    
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

    QPixmap *m_opPix;
    QPixmap *m_hopPix;
    QPixmap *m_voicePix;
    QPixmap *m_redPix;
    QPixmap *m_purplePix;

    XChatServerlist *m_serverList;
XChatPalette m_palette;
KCharSelect *m_charChart;
        XChatURLGrabberView *m_urlGrabber;
        XChatChatListView *m_chatList;
        XChatXferView *m_transfers;
        XChatNotifyListView *m_notifyList;
        XChatIgnoreListView *m_ignoreList;
        XChatBanListView *m_banList;


};


class XChatCleanupHandler:public QObject {
  Q_OBJECT public:
     XChatCleanupHandler();
    KProgress *m_progress;
    QTimer *m_timer;
    void cleanup();

    public slots:void startCleanupLoop();
    void forwardLoop();


    
};

class XChatInputDialog:public QDialog {
  Q_OBJECT public:
    enum DialogType { IntDialog = 0, StringDialog };

     XChatInputDialog(QObject * parent, char *prompt, int def,
		      void *callback, void *ud);
     XChatInputDialog(QObject * parent, char *prompt, char *def,
		      void *callback, void *ud);

    ~XChatInputDialog();

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
