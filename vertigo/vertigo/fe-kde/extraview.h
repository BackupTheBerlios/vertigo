#ifndef XCHATEXTRAVIEW_H
#define XCHATEXTRAVIEW_H

#include <qvariant.h>
#include <qwidget.h>

#include <glib/gslist.h>

#include <klistview.h>

#include "textwidget.h"

#include "../common/xchat.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QFrame;
class QPushButton;
class QLineEdit;
class QLabel;
class KListView;
class QGroupBox;
class QSplitter;
class QCheckBox;
class QSpinBox;
class XChatMainWindow;

class XChatContainerView{
    public:
        XChatContainerView (XChatMainWindow *w);
        virtual void setWindow(XChatMainWindow *w);
        virtual XChatMainWindow* window();
    private:
        XChatMainWindow *m_window;
};


class XChatChanlistView : public QWidget, public XChatContainerView
{
    Q_OBJECT

public:
    XChatChanlistView( QWidget* parent = 0, XChatMainWindow * w=0, server * s=0, const char* name = 0, WFlags fl = 0 );
    ~XChatChanlistView();

    KListView* m_chanlistView;
    QPushButton* m_refreshButton;
    QPushButton* m_saveButton;
    QPushButton* m_joinButton;
    QGroupBox* m_limitGroupBox;
    QLabel* m_maximumLabel;
    QSpinBox* m_minSpin;
    QSpinBox* m_maxSpin;
    QLabel* m_minimumLabel;
    QLabel* m_matchLabel;
    QCheckBox* m_channelCheck;
    QLabel* m_textLabel;
    QCheckBox* m_topicCheck;
    QLineEdit* m_textEdit;
    QPushButton* m_applyButton;


    void enableItems(bool yes);
void showSelf();

 void appendChannel(QString chan,QString users,QString topic);
 bool isMatch(QString chan,QString users,QString topic);

public slots:
    virtual void slotApplyButtonClicked();
    virtual void slotJoinButtonClicked();
    virtual void slotRefreshButtonClicked();
    virtual void slotSaveButtonClicked();
protected:
    XChatMainWindow * m_window;
    server *m_server;
    QGridLayout* XChatChanlistViewLayout;
    QGridLayout* m_limitGroupBoxLayout;
};

class XChatChanlistItem:public KListViewItem {
  public:
    XChatChanlistItem(QListView * list, QString chan, QString users, QString topic);
    ~XChatChanlistItem();
int compare ( QListViewItem * i, int col, bool ascending ) const;

};



class XChatRawlogView : public QWidget, public XChatContainerView
{
    Q_OBJECT

public:
    XChatRawlogView( QWidget* parent ,  XChatMainWindow *w, server *s, const char* name = 0, WFlags fl = 0 );
    ~XChatRawlogView();

    void appendText(QString s);

    
    Vertigo::TextView *m_rawlogView;
    QPushButton* m_clearButton;
    QPushButton* m_saveButton;

//XChatMainWindow *window(){return m_window;}
//void setWindow(XChatMainWindow *w){m_window=w;}

server *getServer(){return m_server;}
void setServer(server *s){m_server=s;}

public slots:
    void slotClearButtonClicked();
    void slotSaveButtonClicked();

protected:
    QGridLayout* XChatRawlogViewLayout;
  //  XChatMainWindow *m_window;
 server *m_server;
 

};

class XChatBanListView : public QWidget, public XChatContainerView
{
    Q_OBJECT

public:
    XChatBanListView( QWidget* parent = 0,XChatMainWindow * w=0, const char* name = 0, WFlags fl = 0 );
    ~XChatBanListView();

    QPushButton* m_clearButton;
    KListView* m_banlistView;
    QPushButton* m_unbanButton;
    QPushButton* m_refreshButton;
    QPushButton* m_cropButton;

void showSelf();
    
public slots:
     void slotUnbanButtonClicked();
     void slotCropButtonClicked();
     void slotClearButtonClicked();
     void slotRefreshButtonClicked();

protected:
     XChatMainWindow * m_window;
    QGridLayout* XChatBanlistViewLayout;


};


class XChatChatListView : public QWidget, public XChatContainerView
{
    Q_OBJECT

public:
    XChatChatListView( QWidget* parent = 0, XChatMainWindow * w=0,const char* name = 0, WFlags fl = 0 );
    ~XChatChatListView();

    QPushButton* m_cancelButton;
    KListView* m_chatListView;
    QPushButton* m_acceptButton;

void showSelf();


    
protected:
    QGridLayout* XChatChatListViewLayout;
    XChatMainWindow * m_window;

};



class XChatNotifyListView : public QWidget, public XChatContainerView
{
    Q_OBJECT

public:
    XChatNotifyListView( QWidget* parent = 0, XChatMainWindow * w=0,const char* name = 0, WFlags fl = 0 );
    ~XChatNotifyListView();

    KListView* m_notifyView;
    QPushButton* m_addButton;
    QPushButton* m_removeButton;

void showSelf();


    
public slots:
    void slotAddButtonClicked();
    void slotRemoveButtonClicked();

protected:
    XChatMainWindow * m_window;
    QGridLayout* XChatNotifyViewLayout;


};

class XChatXferView : public QWidget, public XChatContainerView
{
    Q_OBJECT

public:
    XChatXferView( QWidget* parent = 0, XChatMainWindow * w=0,const char* name = 0, WFlags fl = 0 );
    ~XChatXferView();

    QSplitter* m_splitter;
    QPushButton* m_dlInfoButton;
    QPushButton* m_startButton;
    KListView* m_downloadsView;
    QPushButton* m_dlCancelButton;
    QLabel* m_downloadsLabel;
    QPushButton* m_resumeButton;
    QLabel* m_uploadsLabel;
    QPushButton* m_ulInfoButton;
    QPushButton* m_ulCancelButton;
    KListView* m_uploadsView;

void showSelf();



    
public slots:
    void slotStartButtonClicked();
    void slotDlCancelButtonClicked();
    void slotResumeButtonClicked();
    void slotDlInfoButtonClicked();
    void slotUlCancelButtonClicked();
    void slotUlInfoButtonClicked();

protected:
    QVBoxLayout* XChatFileXferViewLayout;
    QGridLayout* m_downloadLayout;
    QGridLayout* m_uploadLayout;
    XChatMainWindow * m_window;

};

class XChatURLGrabberView : public QWidget, public XChatContainerView
{
    Q_OBJECT

public:
    XChatURLGrabberView( QWidget* parent = 0, XChatMainWindow * w=0,const char* name = 0, WFlags fl = 0 );
    ~XChatURLGrabberView();

    QPushButton* m_saveButton;
    QPushButton* m_clearButton;
    KListView* m_urlView;
void showSelf();


public slots:
    void slotSaveButtonClicked();
    void slotClearButtonClicked();

protected:
    QGridLayout* XChatURLViewLayout;
    XChatMainWindow * m_window;

};




class XChatEditListView : public QWidget
{
    Q_OBJECT

public:
    XChatEditListView( QWidget* parent , const char *filename, GSList *entries, const char* name = 0);
    ~XChatEditListView();
private: 
    GSList *m_entries;
};


class XChatUserOptionsView : public QWidget
{
    Q_OBJECT

public:
    XChatUserOptionsView( QWidget* parent = 0, const char* name = 0);
    ~XChatUserOptionsView();

    QLineEdit* secEdit;
    QLabel* secLabel;
    QLineEdit* altEdit;
    QLabel* realLabel;
    QLineEdit* userEdit;
    QLabel* userLabel;
    QLineEdit* realEdit;
    QLineEdit* nickEdit;
    QLabel* altLabel;
    QLabel* nickLabel;

protected:
    QGridLayout* XChatUserOptionsViewLayout;

};

#endif
