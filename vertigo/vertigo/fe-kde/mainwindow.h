#ifndef XChatMainWin_H
#define XChatMainWin_H

#include <kmainwindow.h>
#include "mainview.h"

#include "../common/xchat.h"

class XChatTabWidget;
class XChatStatusBarWidget;

class XChatMainWindow:public KMainWindow {
  Q_OBJECT public:
     XChatMainWindow(QWidget * parent = 0);
    ~XChatMainWindow();

XChatMainView *getMainView(session * sess);
    XChatTabWidget *tabWidget2;


    
    void showProgressBar();
    void hideProgressBar();


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
	return tabWidget2;
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

    //TODO move these to the tabwidget directly
void slotNextTab();
void slotPreviousTab();

    
  protected:
    QVBoxLayout * XChatMainWindowLayout;
    QPtrList < XChatMainView > *m_mainViewList;
    XChatStatusBarWidget *m_statusBarWidget;

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