#ifndef XCHATEXTRAVIEW_H
#define XCHATEXTRAVIEW_H

#include <qvariant.h>
#include <qwidget.h>

#include <glib/gslist.h>

#include <klistview.h>

#include "textwidget.h"

#include "../common/xchat.h"

#include "rawlogbase.h"
#include "chanlistbase.h"

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
