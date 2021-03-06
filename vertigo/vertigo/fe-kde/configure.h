#ifndef ConfigureDialog_H
#define ConfigureDialog_H

#include <qvariant.h>
#include <qwidget.h>
#include <kdialogbase.h>
#include <kcolorbutton.h>
#include <knotifydialog.h>
#include <klistview.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QWidgetStack;
class QComboBox;
class QLabel;
class QSpinBox;
class QCheckBox;
class QGroupBox;
class QPushButton;
class QLineEdit;
class KListView;
class KListViewItem;

class EditListView;
class ColorButton;

class ConfigureDialog:public KDialogBase {
  Q_OBJECT public:
     ConfigureDialog(QWidget * parent);
    ~ConfigureDialog();

    void createInterfaceGeneralPage(QFrame * frame);
    void createInterfaceTextPage(QFrame * frame);
    void createColorPage(QFrame * frame);
    void createTabsPage(QFrame * frame);
    void createUserlistPage(QFrame * frame);
    void createChattingGeneralPage(QFrame * frame);
    void createUserPage(QFrame * frame);
    void createLoggingPage(QFrame * frame);
    void createEventsTextPage(QFrame * frame);
    void createNotificationsPage(QFrame * frame);
    void createPluginsPage(QFrame * frame);
    void createNetworkGeneralPage(QFrame * frame);
    void createXferPage(QFrame * frame);
    
//gen interface
    QCheckBox *m_serverlistCheck;
    QCheckBox *m_instanceCheck;
    QCheckBox *m_dockCheck;
    QCheckBox *m_nickCompCheck;
    QLabel *m_nickCompLabel;
    QLineEdit *m_nickCompEdit;

    //Events
    KListView *m_eventListView;
    QLabel *m_eventListViewHelp;

    //Notifications
     KNotify::KNotifyWidget * m_notifyWidget;
     
     //plugins
      KListView* m_pluginListView;
    QPushButton* m_pluginLoadButton;
    QPushButton* m_pluginUnloadButton;
     
     // network
     QLabel* m_bindLabel;
    QLineEdit* m_bindEdit;
    QGroupBox* m_proxyGroup;
    QLabel* m_serverLabel;
    QLabel* m_portLabel;
    QLabel* m_typeLabel;
    QCheckBox* m_proxyCheck;
    QLineEdit* m_proxyServerEdit;
    QComboBox* m_proxyTypeCombo;
    QSpinBox* m_proxyPortBox;
     
     //xfer
       QLabel* m_downloadLabel;
    QPushButton* m_completedChooseButton;
    QPushButton* m_downloadChooseButton;
    QCheckBox* m_uploadLimitCheck;
    QSpinBox* m_minUploadPortBox;
    QLabel* m_toLabel;
    QSpinBox* m_maxUploadPortBox;
    QLabel* m_completedLabel;
    QLineEdit* m_completedEdit;
    QLineEdit* m_downloadEdit;
    QLabel* m_uploadLabel;
    QLineEdit* m_addressEdit;
    QGroupBox* m_speedGroupBox;
    QLabel* m_ulSpeedFileLabel;
    QLabel* m_dlSpeedTotalLabel;
    QLabel* m_ulSpeedTotalLabel;
    QLabel* m_dlSpeedFileLabel;
    QSpinBox* m_dlSpeedFileBox;
    QSpinBox* m_ulSpeedFileBox;
    QSpinBox* m_dlSpeedTotalBox;
    QSpinBox* m_ulSpeedTotalBox;
    QCheckBox* m_autoOpenXferCheck;
    QCheckBox* m_cvtSpacesCheck;
    QCheckBox* m_saveNickCheck;
    QCheckBox* m_getAddressCheck;

     

    QComboBox *m_utilitiesCombo;
    QLabel *textLabel1;
    QLabel *textLabel1_2_4;
    QLabel *textLabel1_2_2;
    QComboBox *m_channelCombo;
    QComboBox *m_dialogCombo;
    QComboBox *m_tabCombo;
    QSpinBox *m_shortenBox;
    QLabel *textLabel1_2;
    QLabel *textLabel1_2_3;
    QCheckBox *m_labelCheckbox;
    QCheckBox *m_noticesCheckbox;
    QCheckBox *m_messagesCheckbox;
    QCheckBox *m_popCheckbox;
    QCheckBox *m_groupCheckbox;
    QGroupBox *m_colorsGroupBox;
    ColorButton *m_ircColorButton13;
    ColorButton *m_ircColorButton10;
    ColorButton *m_ircColorButton9;
    ColorButton *m_ircColorButton14;
    ColorButton *m_ircColorButton12;
    ColorButton *m_ircColorButton11;
    ColorButton *m_ircColorButton15;
    ColorButton *m_ircColorButton8;
    ColorButton *m_ircColorButton4;
    ColorButton *m_ircColorButton6;
    ColorButton *m_ircColorButton3;
    ColorButton *m_ircColorButton5;
    ColorButton *m_ircColorButton2;
    ColorButton *m_ircColorButton7;
    ColorButton *m_ircColorButton1;
    ColorButton *m_ircColorButton0;
    ColorButton *m_msgColorButton;
    ColorButton *m_highlightColorButton;
    QLabel *textLabel1_3;
    QLabel *textLabel1_3_2_2;
    QLabel *textLabel1_3_2_2_2;
    ColorButton *m_selForeColorButton;
    ColorButton *m_backColorButton;
    ColorButton *m_dataColorButton;
    ColorButton *m_selBackColorButton;
    QLabel *textLabel1_3_2_2_2_2;
    ColorButton *m_foreColorButton;
    QLabel *textLabel1_3_3;
    QLabel *textLabel1_3_2_2_2_2_2;
    QLabel *textLabel1_3_2;
    QLabel *textLabel2;
    QLineEdit *m_fontEdit;
    QPushButton *m_fontChooseButton;
    QLabel *textLabel2_2_2_2;
    QSpinBox *m_historyCombo;
    QGroupBox *groupBox4;
    QCheckBox *m_stampCheck;
    QLabel *textLabel2_2_2;
    QComboBox *m_stampCombo;
    QGroupBox *groupBox3;
    QLabel *textLabel2_2_3;
    QComboBox *m_bgCombo;
    QCheckBox *m_tintCheck;
    ColorButton *m_tintColorButton;
    QLabel *textLabel2_2;
    QLineEdit *m_imageEdit;
    QPushButton *m_imageChooseButton;
    QCheckBox *m_indentCheck;
    QCheckBox *m_colorCheck;
    QCheckBox *m_stripCheck;
    QComboBox *m_dblclickCombo;
    QLabel *textLabel1_4_2;
    QLabel *textLabel1_4;
    QCheckBox *m_userlistFontCheck;
    QComboBox *m_userlistCombo;
    QCheckBox *m_hostnameCheck;
    QGroupBox *groupBox8;
    QCheckBox *m_userlistButtonCheck;
    EditListView *m_userlistView;
    // QListView *m_userlistView;
    //  QPushButton *m_newUserlistButton;
    //  QPushButton *m_deleteUserlistButton;
    //   QPushButton *m_helpUserlistButton;
    QSpinBox *m_reconnectCombo;
    QLabel *textLabel2_3_3_2_2;
    QLabel *textLabel2_3_3_2;
    QLineEdit *m_awayMsgEdit;
    QLabel *textLabel3_2;
    QLabel *textLabel2_3_3;
    QLabel *textLabel2_3_2;
    QLabel *textLabel3;
    QLineEdit *m_quitMsgEdit;
    QLineEdit *m_highlightMsgEdit;
    QLineEdit *m_partMsgEdit;
    QLabel *textLabel2_3;
    QCheckBox *m_awayCheck;
    QCheckBox *m_autoAwayCheck;
    QCheckBox *m_announceCheck;
    QLineEdit *m_stampLogEdit;
    QCheckBox *m_stampLogCheck;
    QLabel *textLabel5;
    QLineEdit *m_logEdit;
    QLabel *textLabel4;
    QCheckBox *m_logCheck;
    QLabel *textLabel5_2;
    QLabel *textLabel4_2;

    public slots: void slotChooseFont();
    void slotNickCompButtonToggled(bool);
    void slotUserlistButtonToggled(bool);
    void slotLogCheckToggled(bool);
    void slotStampLogCheckToggled(bool);
void slotEventListViewCurrentChanged(QListViewItem * it);
	  void slotProxyCheckClicked();
void slotUploadLimitCheckClicked();
    
  protected:
     QVBoxLayout * ConfigureDialogLayout;
    QVBoxLayout *WStackPageLayout;
    QGridLayout *layout8;
    QGridLayout *layout19;
    QVBoxLayout *WStackPageLayout_2;
    QHBoxLayout *layout5;
    QGridLayout *m_colorsGroupBoxLayout;
    QHBoxLayout *layout6;
    QGridLayout *layout4;
    QVBoxLayout *WStackPageLayout_3;
    QHBoxLayout *layout30;
    QHBoxLayout *layout31;
    QVBoxLayout *groupBox4Layout;
    QHBoxLayout *layout18;
    QVBoxLayout *groupBox3Layout;
    QHBoxLayout *layout11;
    QHBoxLayout *layout12;
    QHBoxLayout *layout13;
    QVBoxLayout *WStackPageLayout_4;
    QGridLayout *layout27;
    QVBoxLayout *groupBox8Layout;
    //  QHBoxLayout *layout30_2;
    // QVBoxLayout *layout29;
    QVBoxLayout *WStackPageLayout_5;
    QGridLayout *layout63;
    QVBoxLayout *WStackPageLayout_6;
    QGridLayout *layout79;

    void languageChange();

};

class ColorButton:public KColorButton {
  Q_OBJECT public:

    ColorButton(QWidget * parent, const char *name = 0L);
    ~ColorButton();

  protected:
    void drawButtonLabel(QPainter * p);
};

class EventlistItem:public KListViewItem {
  public:
    EventlistItem(QListView * list, QString name, QString text, QString _helpText);
    ~EventlistItem();
    QString helpText;
};


#endif				// ConfigureDialog_H
