#ifndef XChatMainWin_H
#define XChatMainWin_H

#include <kmainwindow.h>
#include "mainview.h"


#include "../common/xchat.h"

class KProgress;
class XChatTabWidget;
class XChatStatusBarWidget;

class XChatMainWindow:public KMainWindow {
  Q_OBJECT public:
     XChatMainWindow(QWidget * parent = 0);
    ~XChatMainWindow();

XChatMainView *getMainView(session * sess);
    XChatTabWidget *m_tabWidget;


    
    void showProgressBar();
    void hideProgressBar();

bool isCurrent(session *sess);


    void focusInEvent(QFocusEvent *);
    void resizeEvent(QResizeEvent * r);
    void closeEvent(QCloseEvent * e);
    void windowActivationChange(bool oldActive);

    int viewCount();
int lastSameServerTabIndex();

	void setLag(int lag);
    void setNumbers(int ops, int total);

	void raiseView(XChatMainView *v);

	
    XChatTabWidget *tabWidget() {
	return m_tabWidget;
    }
    public slots:void slotTabChanged(QWidget *);
    void slotNewServerWindow();
    void slotNewChannelWindow();
    void slotNewServerTab();
    void slotNewChannelTab();

    void slotServerList();

    void slotDetachTab();
    void slotCloseTab();
    void slotQuit();

    void slotChanlist();
    void slotURLGrabber();
    void slotChatList();
    void slotTransfers();
    void slotNotifyList();
    void slotIgnoreList();
    void slotBanlist();
    void slotCharChart();
    void slotRawlog();
    
    void slotConfigureToolbars();
    void slotMenubar();
    void slotStatusbar();
    void slotConfigureKeys();
	void slotPreferences();
    void slotToolbar();

void slotTab10();
void slotTab1();
void slotTab2();
void slotTab3();
void slotTab4();
void slotTab5();
void slotTab6();
void slotTab7();
void slotTab8();
void slotTab9();

    //TODO move these to the tabwidget directly
void slotNextTab();
void slotPreviousTab();

    
  protected:
    QVBoxLayout * XChatMainWindowLayout;
    QPtrList < XChatMainView > *m_mainViewList;
    XChatStatusBarWidget *m_statusBarWidget;
	KProgress *m_progressBar;

    // bool m_detached;
    QLabel *lagLabel;
    QLabel *numbersLabel;
    QLabel *nameLabel;

};

/*
class XChatStatusBarWidget : public QWidget
{
    Q_OBJECT

public:
    XChatStatusBarWidget( QWidget* parent = 0, const char* name = 0);
    ~XChatStatusBarWidget();

    QLabel* messageLabel;
    QLabel* lagLabel;
    QFrame* nameFrame;
    QLabel* nameLabel;

protected:
    QHBoxLayout* XChatStatusBarWidgetLayout;
    QHBoxLayout* nameFrameLayout;


};

*/




#endif				// XChatMainWin_H
