#ifndef Serverlist_H
#define Serverlist_H

#include <qvariant.h>
#include <kmainwindow.h>
#include <klistview.h>
#include <kdialogbase.h>

#include "extraview.h"

#include "../common/xchat.h"
#include "../common/servlist.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSplitter;
class QFrame;
class QLabel;
class QToolButton;
class KListView;
class QListViewItem;
class KListViewItem;
class QGroupBox;
class QLineEdit;
class QComboBox;
class QCheckBox;
class QPushButton;


class UserOptionsDialog;

class Serverlist:public KMainWindow {
  Q_OBJECT public:
     Serverlist(QWidget * parent = 0, const char *name = 0);
    ~Serverlist();

    void closeEvent(QCloseEvent * e);


    void setSession(session * sess);

UserOptionsDialog *m_optionsDialog;
    
    QSplitter *m_mainSplitter;
    QFrame *m_networkFrame;
    QLabel *textLabel1;
    QToolButton *m_networkButton;
    KListView *m_networkListView;
    QGroupBox *m_settingsGroupBox;
    QLabel *textLabel2_2_2;
    QLineEdit *m_userLineEdit;
    QLineEdit *m_chansLineEdit;
    QLineEdit *m_realLineEdit;
    QLabel *textLabel2;
    QLineEdit *m_pwdLineEdit;
    QLabel *textLabel2_4;
    QLabel *textLabel2_3;
    QLabel *textLabel2_2;
    QLabel *textLabel2_3_2_3;
    QComboBox *m_charComboBox;
    QLineEdit *m_cmdLineEdit;
    QLabel *textLabel2_3_2;
    QLineEdit *m_nickLineEdit;
    QFrame *m_serverFrame;
    QLabel *textLabel1_2;
    QToolButton *m_serverAddButton;
    KListView *m_serverListView;
    QCheckBox *m_autoCheckbox;
    QCheckBox *m_proxyCheckbox;
    QCheckBox *m_globalCheckbox;
    QCheckBox *m_cycleCheckbox;
    QCheckBox *m_secureCheckbox;
    QCheckBox *m_invalidCheckbox;
    QCheckBox *m_editCheckbox;
    QCheckBox *m_newTabCheckbox;
    QPushButton *m_connectButton;
    QPushButton *m_userOptionsButton;
    QPushButton *m_closeButton;

    public slots:void slotNetworkButtonClicked();
    void slotServerAddButtonClicked();
    void slotNickLineEditTextChanged(const QString &);
    void slotUserLineEdittextChanged(const QString &);
    void slotRealLineEditTextChanged(const QString &);
    void slotPwdLineEditTextChanged(const QString &);
    void slotChansLineEditTextChanged(const QString &);
    void slotCmdLineEditTextChanged(const QString &);
    void slotCharComboBoxActivated(const QString &);
    void slotCloseButtonClicked();
    void slotUserOptionsButtonClicked();
    void slotConnectButtonClicked();
    void slotNewTabCheckboxToggled(bool);
    void slotEditCheckboxToggled(bool);
    void slotNetworkListViewContextMenuRequested(QListViewItem *,
						 const QPoint &, int);
    void slotServerListViewContextMenuRequested(QListViewItem *,
						const QPoint &, int);
    void slotInvalidCheckboxToggled(bool);
    void slotSecureCheckboxToggled(bool);
    void slotProxyCheckboxToggled(bool);
    void slotGlobalCheckboxToggled(bool);
    void slotAutoCheckboxToggled(bool);
    void slotCycleBoxToggled(bool);
    void slotNetworkListViewCurrentChanged(QListViewItem *);

 void slotDelayedDestruct();
    
  protected:
     QVBoxLayout * ServerlistLayout;
    QVBoxLayout *m_networkFrameLayout;
    QHBoxLayout *networkLayout;
    QVBoxLayout *m_settingsGroupBoxLayout;
    QHBoxLayout *layout23;
    QGridLayout *layout22;
    QVBoxLayout *m_serverFrameLayout;
    QHBoxLayout *layout5_2;
    QGridLayout *layout9;
    QHBoxLayout *layout24;
    session *m_session;

    protected slots:void languageChange();

};

class ServerlistNetworkItem:public KListViewItem {
  public:
    ServerlistNetworkItem(QListView * list,
			       ServerlistNetworkItem * after,
			       ircnet * net);
    ~ServerlistNetworkItem();

    ircnet *getNetwork();
  private:
     ircnet * m_network;
};

class UserOptionsDialog:public KDialogBase {
  Q_OBJECT public:
    UserOptionsDialog(QWidget * parent);
    ~UserOptionsDialog();
    private:
    UserOptionsView *m_userOptionsView;
};

#endif				// Serverlist_H
