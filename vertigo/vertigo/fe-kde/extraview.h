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

class MainWindow;

class ContainerView: public QWidget
{
    Q_OBJECT

    public:
        ContainerView (QWidget* parent = 0, const char* name = 0,  MainWindow *w=0);
        virtual void setWindow(MainWindow *w);
        virtual MainWindow* window();
	virtual void showSelf();
    private:
        MainWindow *m_window;
};


class ChanlistView : public ContainerView
{
    Q_OBJECT

public:
    ChanlistView( QWidget* parent = 0, MainWindow * w=0, server * s=0);
    ~ChanlistView();

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

 void appendChannel(QString chan,QString users,QString topic);
 bool isMatch(QString chan,QString users,QString topic);

public slots:
    virtual void slotApplyButtonClicked();
    virtual void slotJoinButtonClicked();
    virtual void slotRefreshButtonClicked();
    virtual void slotSaveButtonClicked();
protected:

    server *m_server;
    QGridLayout* ChanlistViewLayout;
    QGridLayout* m_limitGroupBoxLayout;
};

class ChanlistItem:public KListViewItem {
  public:
    ChanlistItem(QListView * list, QString chan, QString users, QString topic);
    ~ChanlistItem();
int compare ( QListViewItem * i, int col, bool ascending ) const;

};



class RawlogView :  public ContainerView
{
    Q_OBJECT

public:
    RawlogView( QWidget* parent = 0, MainWindow * w=0, server * s=0);
    ~RawlogView();

    void appendText(QString s);

    
    TextView *m_rawlogView;
    QPushButton* m_clearButton;
    QPushButton* m_saveButton;

//MainWindow *window(){return m_window;}
//void setWindow(MainWindow *w){m_window=w;}

server *getServer(){return m_server;}
void setServer(server *s){m_server=s;}

public slots:
    void slotClearButtonClicked();
    void slotSaveButtonClicked();

protected:
    QGridLayout* RawlogViewLayout;

 server *m_server;
 

};

class BanListView :  public ContainerView
{
    Q_OBJECT

public:
    BanListView( QWidget* parent = 0, MainWindow * w=0, server * s=0);
    ~BanListView();

    QPushButton* m_clearButton;
    KListView* m_banlistView;
    QPushButton* m_unbanButton;
    QPushButton* m_refreshButton;
    QPushButton* m_cropButton;


public slots:
     void slotUnbanButtonClicked();
     void slotCropButtonClicked();
     void slotClearButtonClicked();
     void slotRefreshButtonClicked();

protected:

    QGridLayout* BanlistViewLayout;


};


class ChatListView :  public ContainerView
{
    Q_OBJECT

public:
    ChatListView( QWidget* parent = 0, MainWindow * w=0, server * s=0);
    ~ChatListView();

    QPushButton* m_cancelButton;
    KListView* m_chatListView;
    QPushButton* m_acceptButton;

    
protected:
    QGridLayout* ChatListViewLayout;


};



class NotifyListView :  public ContainerView
{
    Q_OBJECT

public:
    NotifyListView( QWidget* parent = 0, MainWindow * w=0, server * s=0);
    ~NotifyListView();

    KListView* m_notifyView;
    QPushButton* m_addButton;
    QPushButton* m_removeButton;

public slots:
    void slotAddButtonClicked();
    void slotRemoveButtonClicked();

protected:

    QGridLayout* NotifyViewLayout;


};

class XferView :  public ContainerView
{
    Q_OBJECT

public:
    XferView( QWidget* parent = 0, MainWindow * w=0, server * s=0);
    ~XferView();

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


public slots:
    void slotStartButtonClicked();
    void slotDlCancelButtonClicked();
    void slotResumeButtonClicked();
    void slotDlInfoButtonClicked();
    void slotUlCancelButtonClicked();
    void slotUlInfoButtonClicked();

protected:
    QVBoxLayout* FileXferViewLayout;
    QGridLayout* m_downloadLayout;
    QGridLayout* m_uploadLayout;


};

class URLGrabberView :  public ContainerView
{
    Q_OBJECT

public:
    URLGrabberView( QWidget* parent = 0, MainWindow * w=0, server * s=0);
    ~URLGrabberView();

    QPushButton* m_saveButton;
    QPushButton* m_clearButton;
    KListView* m_urlView;

public slots:
    void slotSaveButtonClicked();
    void slotClearButtonClicked();

protected:
    QGridLayout* URLViewLayout;


};




class EditListView : public QWidget
{
    Q_OBJECT

public:
    EditListView( QWidget* parent , const char *filename, GSList *entries, const char* name = 0);
    ~EditListView();
private: 
    GSList *m_entries;
};


class UserOptionsView : public QWidget
{
    Q_OBJECT

public:
    UserOptionsView( QWidget* parent = 0, const char* name = 0);
    ~UserOptionsView();

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
    QGridLayout* UserOptionsViewLayout;

};

#endif
