#ifndef MainWin_H
#define MainWin_H

#include <kmainwindow.h>
#include "mainview.h"


#include "../common/xchat.h"

class KProgress;

class TabWidget;
class StatusBarWidget;

class MainWindow:public KMainWindow {
  Q_OBJECT public:
     MainWindow(QWidget * parent = 0);
    ~MainWindow();

MainView *getMainView(session * sess);
    TabWidget *m_tabWidget;


    
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

	void raiseView(MainView *v);

	
    TabWidget *tabWidget() {
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
void forwardLoop();
    
  protected:
    QVBoxLayout * MainWindowLayout;
    QPtrList < MainView > *m_mainViewList;
    StatusBarWidget *m_statusBarWidget;
	KProgress *m_progressBar;

    // bool m_detached;
    QLabel *lagLabel;
    QLabel *numbersLabel;
    QLabel *nameLabel;
    QTimer *m_timer;
};

/*
class StatusBarWidget : public QWidget
{
    Q_OBJECT

public:
    StatusBarWidget( QWidget* parent = 0, const char* name = 0);
    ~StatusBarWidget();

    QLabel* messageLabel;
    QLabel* lagLabel;
    QFrame* nameFrame;
    QLabel* nameLabel;

protected:
    QHBoxLayout* StatusBarWidgetLayout;
    QHBoxLayout* nameFrameLayout;


};

*/


#endif				// MainWin_H
