#include <qvariant.h>
#include <qpushbutton.h>
#include <qwidgetstack.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qheader.h>
#include <klistview.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <klocale.h>
#include <qstyle.h>
#include <qpainter.h>
#include <kfontdialog.h>
#include <kglobalsettings.h>
#include <kdebug.h>
#include <knotifydialog.h>
#include <kaboutdata.h>

#include "configure.h"

#define PLUGIN_C

#include "../common/xchat.h"


#define PLUGIN_C
typedef struct session xchat_context;
#include "../common/xchat-plugin.h"
#include "../common/plugin.h"

#include "../common/xchatc.h"



#include "../common/cfgfiles.h"
#include "../common/outbound.h"
#include "../common/text.h"
#include "../common/util.h"
#include "extraview.h"


#include "app.h"

extern struct text_event te[];
extern char *pntevts_text[];
extern GSList *plugin_list;


ConfigureDialog::ConfigureDialog(QWidget * parent)
:  KDialogBase(IconList, i18n("Configure"), Help | Default | Ok | Apply | Cancel, Ok, parent, 0, false)
{
    QStringList tree;
    QFrame *frame;

    tree << i18n("Interface") << i18n("General");
    frame = addPage(tree, i18n("General"));
    createInterfaceGeneralPage(frame);
    tree.clear();

    tree << i18n("Interface") << i18n("Text");
    frame = addPage(tree, i18n("Text"));
    createInterfaceTextPage(frame);
    tree.clear();

    tree << i18n("Interface") << i18n("Color");
    frame = addPage(tree, i18n("Color"));
    createColorPage(frame);
    tree.clear();

    tree << i18n("Interface") << i18n("Tabs");
    frame = addPage(tree, i18n("Tabs"));
    createTabsPage(frame);
    tree.clear();

    tree << i18n("Interface") << i18n("Userlist");
    frame = addPage(tree, i18n("Userlist"));
    createUserlistPage(frame);
    tree.clear();

    tree << i18n("Chatting") << i18n("General");
    frame = addPage(tree, i18n("General"));
    createChattingGeneralPage(frame);
    tree.clear();

    tree << i18n("Chatting") << i18n("User Info");
    frame = addPage(tree, i18n("User Info"));
    createUserPage(frame);
    tree.clear();

    tree << i18n("Chatting") << i18n("Logging");
    frame = addPage(tree, i18n("Logging"));
    createLoggingPage(frame);
    tree.clear();

    tree << i18n("Events") << i18n("Text");
    frame = addPage(tree, i18n("Text"));
    createEventsTextPage(frame);
    tree.clear();

    tree << i18n("Events") << i18n("Notifications");
    frame = addPage(tree, i18n("Notifications"));
    createNotificationsPage(frame);
    tree.clear();

    tree << i18n("Events") << i18n("Plugins");
    frame = addPage(tree, i18n("Plugins"));
    createPluginsPage(frame);
    tree.clear();
    
        tree << i18n("Network") << i18n("General");
    frame = addPage(tree, i18n("General"));
    createNetworkGeneralPage(frame);
    tree.clear();
    
            tree << i18n("Network") << i18n("File Transfers");
    frame = addPage(tree, i18n("File Transfers"));
    createXferPage(frame);
    tree.clear();

    languageChange();
    textLabel1->setBuddy(m_tabCombo);
    textLabel1_2_4->setBuddy(m_shortenBox);
    textLabel1_2_2->setBuddy(m_dialogCombo);
    textLabel1_2->setBuddy(m_channelCombo);
    textLabel1_2_3->setBuddy(m_utilitiesCombo);
    textLabel1_3->setBuddy(m_foreColorButton);
    textLabel1_3_2_2->setBuddy(m_selBackColorButton);
    textLabel1_3_2_2_2->setBuddy(m_dataColorButton);
    textLabel1_3_2_2_2_2->setBuddy(m_msgColorButton);
    textLabel1_3_3->setBuddy(m_selForeColorButton);
    textLabel1_3_2_2_2_2_2->setBuddy(m_highlightColorButton);
    textLabel1_3_2->setBuddy(m_backColorButton);
    textLabel2->setBuddy(m_fontEdit);
    textLabel2_2_2_2->setBuddy(m_historyCombo);
    textLabel2_2_2->setBuddy(m_stampCombo);
    textLabel2_2_3->setBuddy(m_bgCombo);
    textLabel2_2->setBuddy(m_imageEdit);
    textLabel1_4_2->setBuddy(m_dblclickCombo);
    textLabel1_4->setBuddy(m_userlistCombo);
    textLabel2_3_3_2_2->setBuddy(m_reconnectCombo);
    textLabel2_3_3_2->setBuddy(m_highlightMsgEdit);
    textLabel2_3_3->setBuddy(m_awayMsgEdit);
    textLabel2_3_2->setBuddy(m_partMsgEdit);
    textLabel2_3->setBuddy(m_quitMsgEdit);
    textLabel4->setBuddy(m_logEdit);
    textLabel4_2->setBuddy(m_stampLogCheck);

    connect(m_fontChooseButton, SIGNAL(clicked()), this, SLOT(slotChooseFont()));

    //general
    //
    if(!prefs.slist_skip)
	m_serverlistCheck->setChecked(true);

    if(prefs.multiple_instances)
	m_instanceCheck->setChecked(true);

    if(prefs.systray_dock)
	m_dockCheck->setChecked(true);
    if(prefs.nickcompletion) {
	m_nickCompCheck->setChecked(true);
	slotNickCompButtonToggled(true);
    } else {
	slotNickCompButtonToggled(false);
    }

    m_nickCompEdit->setText(prefs.nick_suffix);

//text
    QString n = prefs.font_normal;
    int i = n.findRev(' ');

    if(n.left(i) == "monospace") {
	QFont font = KGlobalSettings::fixedFont();

	n = QString("%1%2%3 %4").arg(font.family()).arg(font.bold()? " Bold" : QString::null).arg(font.italic()? " Italic" : QString::null).arg(font.pointSize());
    }
    m_fontEdit->setText(n);
    m_historyCombo->setValue(prefs.max_lines);

    m_tintColorButton->setColor(QColor(prefs.tint_red, prefs.tint_green, prefs.tint_blue));

    if(prefs.timestamp)
	m_stampCheck->setChecked(true);
    m_stampCombo->setEditText(prefs.stamp_format);
    if(prefs.indent_nicks)
	m_indentCheck->setChecked(true);
    if(prefs.colorednicks)
	m_colorCheck->setChecked(true);
    if(prefs.stripcolor)
	m_stripCheck->setChecked(true);

    m_userlistCombo->setEditText(prefs.doubleclickuser);

//colors
    //
    Palette pal = xchatapp->palette();

#define setButtonColor(x) m_ircColorButton[x]->setColor(*(pal[x]))
     // for (i = 0; i< 16; i++)
     //    setButtonColor((i));
    //m_ircColor0Button->setColor(*(pal[0])); 
    m_ircColorButton0->setColor(*(pal[0]));
    m_ircColorButton1->setColor(*(pal[1]));
    m_ircColorButton2->setColor(*(pal[2]));
    m_ircColorButton3->setColor(*(pal[3]));
    m_ircColorButton4->setColor(*(pal[4]));
    m_ircColorButton5->setColor(*(pal[5]));
    m_ircColorButton6->setColor(*(pal[6]));
    m_ircColorButton7->setColor(*(pal[7]));
    m_ircColorButton8->setColor(*(pal[8]));
    m_ircColorButton9->setColor(*(pal[9]));
    m_ircColorButton10->setColor(*(pal[10]));
    m_ircColorButton11->setColor(*(pal[11]));
    m_ircColorButton12->setColor(*(pal[12]));
    m_ircColorButton13->setColor(*(pal[13]));
    m_ircColorButton14->setColor(*(pal[14]));
    m_ircColorButton15->setColor(*(pal[15]));

    m_foreColorButton->setColor(*(pal[18]));
    m_backColorButton->setColor(*(pal[19]));
    m_selForeColorButton->setColor(*(pal[16]));
    m_selBackColorButton->setColor(*(pal[17]));
    m_dataColorButton->setColor(*(pal[20]));
    m_msgColorButton->setColor(*(pal[21]));
    m_highlightColorButton->setColor(*(pal[22]));

// tabs
//

/*m_utilitiesCombo
m_channelCombo
m_dialogCombo
m_tabCombo*/
m_shortenBox->setValue(prefs.truncchans);
/*m_labelCheckbox
m_noticesCheckbox
m_messagesCheckbox
*/
if(prefs.newtabstofront)
m_popCheckbox->setChecked(true);
//m_groupCheckbox



//userlist

    m_userlistCombo->setCurrentItem(prefs.userlist_sort);
    m_dblclickCombo->setCurrentText(prefs.doubleclickuser);

    if(prefs.userlistbuttons) {
	m_userlistButtonCheck->setChecked(true);
	slotUserlistButtonToggled(true);
    } else {
	slotUserlistButtonToggled(false);
    }

// chatting
    //
    m_quitMsgEdit->setText(prefs.quitreason);
    m_partMsgEdit->setText(prefs.partreason);
    m_awayMsgEdit->setText(prefs.awayreason);
    m_highlightMsgEdit->setText(prefs.bluestring);
    m_reconnectCombo->setValue(prefs.recon_delay);

    if(prefs.show_away_once)
	m_awayCheck->setChecked(true);

    if(prefs.auto_unmark_away)
	m_autoAwayCheck->setChecked(true);

    if(prefs.show_away_message)
	m_announceCheck->setChecked(true);

//logging
//
//

// TODO xxx->setEnabled somewhere else..
//

    if(prefs.logging) {
	m_logCheck->setChecked(true);
	slotLogCheckToggled(true);
    } else {
	slotLogCheckToggled(false);
    }

    if(prefs.timestamp_logs) {
	m_stampLogCheck->setChecked(true);
	slotStampLogCheckToggled(true);
    } else {
	slotStampLogCheckToggled(false);
    }

    m_logEdit->setText(prefs.logmask);
    m_stampLogEdit->setText(prefs.timestamp_log_format);

// text events
    //

    QString helpText = QString::null;

    //"Special format codes for \"<b>%1</b>\":<br><br>";
    int j;

    for(i = 0; i < NUM_XP; i++) {
	helpText = QString("Special format codes for \"<b>%1</b>\":<br><br>").arg(te[i].name);
	j = 0;
	while(j < te[i].num_args) {
	    helpText.append(QString("$%1 %2<br>").arg(j + 1).arg(te[i].help[j]));
	    j++;
	}
	new EventlistItem(m_eventListView, te[i].name, pntevts_text[i], helpText);
    }

    // plugins
    //
    xchat_plugin *pl;
    GSList *list = plugin_list;

    while(list) {
	pl = (xchat_plugin *) list->data;
	if(pl->version[0] != 0)
	    new KListViewItem(m_pluginListView, pl->name, file_part(pl->filename), pl->version, pl->desc);

	list = list->next;
    }

    setRootIsDecorated(false);
    setTreeListAutoResize(false);
    //unfoldTreeList(true);

    resize(minimumSize());

    connect(m_nickCompCheck, SIGNAL(toggled(bool)), this, SLOT(slotNickCompButtonToggled(bool)));
    connect(m_userlistButtonCheck, SIGNAL(toggled(bool)), this, SLOT(slotUserlistButtonToggled(bool)));
    connect(m_logCheck, SIGNAL(toggled(bool)), this, SLOT(slotLogCheckToggled(bool)));
    connect(m_stampLogCheck, SIGNAL(toggled(bool)), this, SLOT(slotStampLogCheckToggled(bool)));

    connect(m_eventListView, SIGNAL(currentChanged(QListViewItem *)), this, SLOT(slotEventListViewCurrentChanged(QListViewItem *)));

 connect( m_proxyCheck, SIGNAL( clicked() ), this, SLOT( slotProxyCheckClicked() ) );

connect( m_uploadLimitCheck, SIGNAL( clicked() ), this, SLOT( slotUploadLimitCheckClicked() ) );

}

void ConfigureDialog::languageChange()
{

// TODO move to createXXX pages
    m_utilitiesCombo->clear();
    m_utilitiesCombo->insertItem(tr("Tabs"));
    m_utilitiesCombo->insertItem(tr("Windows"));
    textLabel1->setText(tr("Show &tabs at:"));
    textLabel1_2_4->setText(tr("&Shorten tabs to:"));
    textLabel1_2_2->setText(tr("Open &dialogs in:"));
    m_channelCombo->clear();
    m_channelCombo->insertItem(tr("Tabs"));
    m_channelCombo->insertItem(tr("Windows"));
    m_dialogCombo->clear();
    m_dialogCombo->insertItem(tr("Tabs"));
    m_dialogCombo->insertItem(tr("Windows"));
    m_tabCombo->clear();
    m_tabCombo->insertItem(tr("Bottom"));
    m_tabCombo->insertItem(tr("Top"));
    m_tabCombo->insertItem(tr("Left"));
    m_tabCombo->insertItem(tr("Right"));
    m_tabCombo->insertItem(tr("Hidden"));
    m_shortenBox->setSuffix(tr(" letters"));
    textLabel1_2->setText(tr("Open &channels in:"));
    textLabel1_2_3->setText(tr("Open &utilities in:"));
    m_labelCheckbox->setText(tr("Label tabs with server"));
    m_noticesCheckbox->setText(tr("Open tab for server notices"));
    m_messagesCheckbox->setText(tr("Open tab for server messages"));
    m_popCheckbox->setText(tr("Pop new tabs to front"));
    m_groupCheckbox->setText(tr("Group tabs by server"));
    m_colorsGroupBox->setTitle(tr("Text &colors:"));
    m_ircColorButton13->setText(tr("13"));
    m_ircColorButton10->setText(tr("10"));
    m_ircColorButton9->setText(tr("9"));
    m_ircColorButton14->setText(tr("14"));
    m_ircColorButton12->setText(tr("12"));
    m_ircColorButton11->setText(tr("11"));
    m_ircColorButton15->setText(tr("15"));
    m_ircColorButton8->setText(tr("8"));
    m_ircColorButton4->setText(tr("4"));
    m_ircColorButton6->setText(tr("6"));
    m_ircColorButton3->setText(tr("3"));
    m_ircColorButton5->setText(tr("5"));
    m_ircColorButton2->setText(tr("2"));
    m_ircColorButton7->setText(tr("7"));
    m_ircColorButton1->setText(tr("1"));
    m_ircColorButton0->setText(tr("0"));
    textLabel1_3->setText(tr("&Foreground:"));
    textLabel1_3_2_2->setText(tr("S&election Background:"));
    textLabel1_3_2_2_2->setText(tr("New &data:"));
    textLabel1_3_2_2_2_2->setText(tr("New &message:"));
    textLabel1_3_3->setText(tr("&Selection Foreground:"));
    textLabel1_3_2_2_2_2_2->setText(tr("&Highlight:"));
    textLabel1_3_2->setText(tr("&Background:"));
    textLabel2->setText(tr("&Font:"));
    m_fontChooseButton->setText(tr("Ch&oose..."));
    m_fontChooseButton->setAccel(QKeySequence(tr("Alt+O")));
    textLabel2_2_2_2->setText(tr("&History:"));
    m_historyCombo->setSuffix(tr(" lines"));
    groupBox4->setTitle(tr("Time &stamp"));
    m_stampCheck->setText(tr("Sho&w time stamp"));
    m_stampCheck->setAccel(QKeySequence(tr("Alt+W")));
    textLabel2_2_2->setText(tr("Fo&rmat:"));
    groupBox3->setTitle(tr("&Background"));
    textLabel2_2_3->setText(tr("T&ype:"));
    m_bgCombo->clear();
    m_bgCombo->insertItem(tr("Image"));
    m_bgCombo->insertItem(tr("Transparent"));
    m_tintCheck->setText(tr("&Tint transparency with color:"));
    m_tintCheck->setAccel(QKeySequence(tr("Alt+T")));
    textLabel2_2->setText(tr("&Image:"));
    m_imageChooseButton->setText(tr("Choos&e..."));
    m_imageChooseButton->setAccel(QKeySequence(tr("Alt+E")));
    m_indentCheck->setText(tr("Indent &nicks"));
    m_indentCheck->setAccel(QKeySequence(tr("Alt+N")));
    m_colorCheck->setText(tr("&Color nicks"));
    m_colorCheck->setAccel(QKeySequence(tr("Alt+C")));
    m_stripCheck->setText(tr("Strip &mIRC color"));
    m_stripCheck->setAccel(QKeySequence(tr("Alt+M")));
    m_dblclickCombo->clear();
    m_dblclickCombo->insertItem(tr("QUOTE WHOIS %s"));
    textLabel1_4_2->setText(tr("&Double click command:"));
    textLabel1_4->setText(tr("&Sort userlist by:"));
    m_userlistFontCheck->setText(tr("&Use IRC text font and colors"));
    m_userlistFontCheck->setAccel(QKeySequence(tr("Alt+U")));
    m_userlistCombo->clear();
    m_userlistCombo->insertItem(tr("Alphabetical, Ops first"));
    m_userlistCombo->insertItem(tr("Alphabetical"));
    m_userlistCombo->insertItem(tr("Reverse Alphabetical, Ops first"));
    m_userlistCombo->insertItem(tr("Reverse Alphabetical"));
    m_userlistCombo->insertItem(tr("Unsorted"));
    m_hostnameCheck->setText(tr("Show &hostnames in userlist"));
    m_hostnameCheck->setAccel(QKeySequence(tr("Alt+H")));
    groupBox8->setTitle(tr("Userlist &buttons"));
    m_userlistButtonCheck->setText(tr("&Enable userlist buttons"));
    m_userlistButtonCheck->setAccel(QKeySequence(tr("Alt+E")));
    // m_userlistView->header()->setLabel(0, tr("Name"));
    // m_userlistView->header()->setLabel(1, tr("Command"));
    /* m_newUserlistButton->setText(tr("&New"));
       m_newUserlistButton->setAccel(QKeySequence(tr("Alt+N")));
       m_deleteUserlistButton->setText(tr("De&lete"));
       m_deleteUserlistButton->setAccel(QKeySequence(tr("Alt+L")));
       m_helpUserlistButton->setText(tr("&Help"));
       m_helpUserlistButton->setAccel(QKeySequence(tr("Alt+H"))); */
    m_reconnectCombo->setSuffix(tr(" seconds"));
    textLabel2_3_3_2_2->setText(tr("Auto &reconnect delay:"));
    textLabel2_3_3_2->setText(tr("Extra words to &highlight on:"));
    textLabel3_2->setText(tr("(Seperate multiple words with commas)"));
    textLabel2_3_3->setText(tr("Default &away message:"));
    textLabel2_3_2->setText(tr("Default &part message:"));
    textLabel3->setText(tr("(Can be a text file relative to"));
    textLabel2_3->setText(tr("&Default quit message:"));
    m_awayCheck->setText(tr("&Show away once"));
    m_awayCheck->setAccel(QKeySequence(tr("Alt+S")));
    m_autoAwayCheck->setText(tr("Automatically &unmark away"));
    m_autoAwayCheck->setAccel(QKeySequence(tr("Alt+U")));
    m_announceCheck->setText(tr("Announce away &messages"));
    m_announceCheck->setAccel(QKeySequence(tr("Alt+M")));
    m_stampLogCheck->setText(tr("&Insert timestamps in logs"));
    m_stampLogCheck->setAccel(QKeySequence(tr("Alt+I")));
    textLabel5->setText(tr("(%s=Server %c=Channel %n=Network)"));
    textLabel4->setText(tr("Log &filename format:"));
    m_logCheck->setText(tr("&Enable logging of conversations"));
    m_logCheck->setAccel(QKeySequence(tr("Alt+E")));
    textLabel5_2->setText(tr("(see help for details)"));
    textLabel4_2->setText(tr("Log &timestamp format:"));
}

void ConfigureDialog::createInterfaceGeneralPage(QFrame * frame)
{
    QVBoxLayout *Form1Layout = new QVBoxLayout(frame, 12, 6, "lay");

    m_serverlistCheck = new QCheckBox(frame, "serverlistCheck");
    Form1Layout->addWidget(m_serverlistCheck);

    m_instanceCheck = new QCheckBox(frame, "instanceCheck");
    Form1Layout->addWidget(m_instanceCheck);

    m_dockCheck = new QCheckBox(frame, "dockCheck");
    Form1Layout->addWidget(m_dockCheck);

    m_nickCompCheck = new QCheckBox(frame, "nickCompCheck");
    Form1Layout->addWidget(m_nickCompCheck);

    QHBoxLayout *layout2 = new QHBoxLayout(0, 0, 6, "layout2");

    m_nickCompLabel = new QLabel(frame, "nickCompLabel");
    layout2->addWidget(m_nickCompLabel);

    m_nickCompEdit = new QLineEdit(frame, "nickCompEdit");
    m_nickCompEdit->setMaxLength(4);
    layout2->addWidget(m_nickCompEdit);
    //QSpacerItem *spacer = new QSpacerItem(61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    //layout2->addItem(spacer);
    Form1Layout->addLayout(layout2);
    QSpacerItem *spacer_2 = new QSpacerItem(21, 290, QSizePolicy::Minimum, QSizePolicy::Expanding);

    Form1Layout->addItem(spacer_2);

    // buddies
    m_nickCompLabel->setBuddy(m_nickCompEdit);

    m_serverlistCheck->setText(tr("&Show server list on startup"));
    m_instanceCheck->setText(tr("Allow &multiple instances"));
    m_dockCheck->setText(tr("&Dock in system tray"));
    m_nickCompCheck->setText(tr("Enable &nick completion"));
    m_nickCompLabel->setText(tr("Nick &completion suffix:"));

}

void ConfigureDialog::slotChooseFont()
{
    QFont font;
    QString name = m_fontEdit->text();
    int i = name.findRev(' ');
    int s = 10;

    font.setFamily(name.left(i));
    if(i != -1)
	s = name.mid(i + 1).toInt();
    font.setPointSize(s);

    if(KFontDialog::getFont(font, false, this) == QDialog::Rejected)
	return;

    name = QString("%1%2%3 %4").arg(font.family()).arg(font.bold()? " Bold" : QString::null).arg(font.italic()? " Italic" : QString::null).arg(font.pointSize());
//      name=QString("%1 %2").arg(font.family()).arg(font.pointSize());
    m_fontEdit->setText(name);

}

void ConfigureDialog::createInterfaceTextPage(QFrame * frame)
{
    WStackPageLayout_3 = new QVBoxLayout(frame, 12, 6, "WStackPageLayout_3");
    layout30 = new QHBoxLayout(0, 0, 6, "layout30");
    textLabel2 = new QLabel(frame, "textLabel2");
    layout30->addWidget(textLabel2);

    m_fontEdit = new QLineEdit(frame, "m_fontEdit");
    layout30->addWidget(m_fontEdit);

    m_fontChooseButton = new QPushButton(frame, "m_fontChooseButton");
    layout30->addWidget(m_fontChooseButton);
    WStackPageLayout_3->addLayout(layout30);

    layout31 = new QHBoxLayout(0, 0, 6, "layout31");

    textLabel2_2_2_2 = new QLabel(frame, "textLabel2_2_2_2");
    layout31->addWidget(textLabel2_2_2_2);

    m_historyCombo = new QSpinBox(frame, "m_historyCombo");
    m_historyCombo->setMaxValue(30000);
    layout31->addWidget(m_historyCombo);
    QSpacerItem *spacer_5 = new QSpacerItem(361, 20, QSizePolicy::Expanding,
	QSizePolicy::Minimum);

    layout31->addItem(spacer_5);
    WStackPageLayout_3->addLayout(layout31);

    groupBox4 = new QGroupBox(frame, "groupBox4");
    groupBox4->setColumnLayout(0, Qt::Vertical);
    groupBox4->layout()->setSpacing(6);
    groupBox4->layout()->setMargin(11);
    groupBox4Layout = new QVBoxLayout(groupBox4->layout());
    groupBox4Layout->setAlignment(Qt::AlignTop);

    m_stampCheck = new QCheckBox(groupBox4, "m_stampCheck");
    groupBox4Layout->addWidget(m_stampCheck);

    layout18 = new QHBoxLayout(0, 0, 6, "layout18");

    textLabel2_2_2 = new QLabel(groupBox4, "textLabel2_2_2");
    layout18->addWidget(textLabel2_2_2);

    m_stampCombo = new QComboBox(FALSE, groupBox4, "m_stampCombo");
    m_stampCombo->setEditable(TRUE);
    layout18->addWidget(m_stampCombo);
    QSpacerItem *spacer_6 = new QSpacerItem(181, 21, QSizePolicy::Expanding,
	QSizePolicy::Minimum);

    layout18->addItem(spacer_6);
    groupBox4Layout->addLayout(layout18);
    WStackPageLayout_3->addWidget(groupBox4);

    groupBox3 = new QGroupBox(frame, "groupBox3");
    groupBox3->setColumnLayout(0, Qt::Vertical);
    groupBox3->layout()->setSpacing(6);
    groupBox3->layout()->setMargin(11);
    groupBox3Layout = new QVBoxLayout(groupBox3->layout());
    groupBox3Layout->setAlignment(Qt::AlignTop);

    layout11 = new QHBoxLayout(0, 0, 6, "layout11");

    textLabel2_2_3 = new QLabel(groupBox3, "textLabel2_2_3");
    layout11->addWidget(textLabel2_2_3);

    m_bgCombo = new QComboBox(FALSE, groupBox3, "m_bgCombo");
    layout11->addWidget(m_bgCombo);
    QSpacerItem *spacer_7 = new QSpacerItem(101, 20, QSizePolicy::Expanding,
	QSizePolicy::Minimum);

    layout11->addItem(spacer_7);
    groupBox3Layout->addLayout(layout11);

    layout12 = new QHBoxLayout(0, 0, 6, "layout12");

    m_tintCheck = new QCheckBox(groupBox3, "m_tintCheck");
    layout12->addWidget(m_tintCheck);

    m_tintColorButton = new ColorButton(groupBox3, "m_tintColorButton");
    m_tintColorButton->setMaximumSize(QSize(50, 25));
    layout12->addWidget(m_tintColorButton);
    QSpacerItem *spacer_8 = new QSpacerItem(101, 20, QSizePolicy::Expanding,
	QSizePolicy::Minimum);

    layout12->addItem(spacer_8);
    groupBox3Layout->addLayout(layout12);

    layout13 = new QHBoxLayout(0, 0, 6, "layout13");

    textLabel2_2 = new QLabel(groupBox3, "textLabel2_2");
    layout13->addWidget(textLabel2_2);

    m_imageEdit = new QLineEdit(groupBox3, "m_imageEdit");
    layout13->addWidget(m_imageEdit);

    m_imageChooseButton = new QPushButton(groupBox3, "m_imageChooseButton");
    layout13->addWidget(m_imageChooseButton);
    groupBox3Layout->addLayout(layout13);
    WStackPageLayout_3->addWidget(groupBox3);

    m_indentCheck = new QCheckBox(frame, "m_indentCheck");
    WStackPageLayout_3->addWidget(m_indentCheck);

    m_colorCheck = new QCheckBox(frame, "m_colorCheck");
    WStackPageLayout_3->addWidget(m_colorCheck);

    m_stripCheck = new QCheckBox(frame, "m_stripCheck");
    WStackPageLayout_3->addWidget(m_stripCheck);
    QSpacerItem *spacer_9 = new QSpacerItem(41, 170, QSizePolicy::Minimum,
	QSizePolicy::Expanding);

    WStackPageLayout_3->addItem(spacer_9);

}

void ConfigureDialog::createColorPage(QFrame * frame)
{
    WStackPageLayout_2 = new QVBoxLayout(frame, 11, 6, "WStackPageLayout_2");

    layout5 = new QHBoxLayout(0, 0, 6, "layout5");

    m_colorsGroupBox = new QGroupBox(frame, "m_colorsGroupBox");
    m_colorsGroupBox->setColumnLayout(0, Qt::Vertical);
    m_colorsGroupBox->layout()->setSpacing(6);
    m_colorsGroupBox->layout()->setMargin(11);
    m_colorsGroupBoxLayout = new QGridLayout(m_colorsGroupBox->layout());
    m_colorsGroupBoxLayout->setAlignment(Qt::AlignTop);

    m_ircColorButton13 = new ColorButton(m_colorsGroupBox, "m_ircColor13Button");
    m_ircColorButton13->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton13, 1, 5);

    m_ircColorButton10 = new ColorButton(m_colorsGroupBox, "m_ircColor10Button");
    m_ircColorButton10->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton10, 1, 2);

    m_ircColorButton9 = new ColorButton(m_colorsGroupBox, "m_ircColor9Button");
    m_ircColorButton9->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton9, 1, 1);

    m_ircColorButton14 = new ColorButton(m_colorsGroupBox, "m_ircColor14Button");
    m_ircColorButton14->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton14, 1, 6);

    m_ircColorButton12 = new ColorButton(m_colorsGroupBox, "m_ircColor12Button");
    m_ircColorButton12->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton12, 1, 4);

    m_ircColorButton11 = new ColorButton(m_colorsGroupBox, "m_ircColor11Button");
    m_ircColorButton11->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton11, 1, 3);

    m_ircColorButton15 = new ColorButton(m_colorsGroupBox, "m_ircColor15Button");
    m_ircColorButton15->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton15, 1, 7);

    m_ircColorButton8 = new ColorButton(m_colorsGroupBox, "m_ircColor8Button");
    m_ircColorButton8->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton8, 1, 0);

    m_ircColorButton4 = new ColorButton(m_colorsGroupBox, "m_ircColor4Button");
    m_ircColorButton4->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton4, 0, 4);

    m_ircColorButton6 = new ColorButton(m_colorsGroupBox, "m_ircColor6Button");
    m_ircColorButton6->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton6, 0, 6);

    m_ircColorButton3 = new ColorButton(m_colorsGroupBox, "m_ircColor3Button");
    m_ircColorButton3->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton3, 0, 3);

    m_ircColorButton5 = new ColorButton(m_colorsGroupBox, "m_ircColor5Button");
    m_ircColorButton5->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton5, 0, 5);

    m_ircColorButton2 = new ColorButton(m_colorsGroupBox, "m_ircColor2Button");
    m_ircColorButton2->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton2, 0, 2);

    m_ircColorButton7 = new ColorButton(m_colorsGroupBox, "m_ircColor7Button");
    m_ircColorButton7->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton7, 0, 7);

    m_ircColorButton1 = new ColorButton(m_colorsGroupBox, "m_ircColor1Button");
    m_ircColorButton1->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton1, 0, 1);

    m_ircColorButton0 = new ColorButton(m_colorsGroupBox, "m_ircColor0Button");
    m_ircColorButton0->setMaximumSize(QSize(50, 25));

    m_colorsGroupBoxLayout->addWidget(m_ircColorButton0, 0, 0);
    layout5->addWidget(m_colorsGroupBox);
    QSpacerItem *spacer_2 = new QSpacerItem(321, 20, QSizePolicy::Expanding,
	QSizePolicy::Minimum);

    layout5->addItem(spacer_2);
    WStackPageLayout_2->addLayout(layout5);

    layout6 = new QHBoxLayout(0, 0, 6, "layout6");

    layout4 = new QGridLayout(0, 1, 1, 0, 6, "layout4");

    m_msgColorButton = new ColorButton(frame, "m_msgColorButton");
    m_msgColorButton->setMaximumSize(QSize(50, 25));

    layout4->addWidget(m_msgColorButton, 5, 1);

    m_highlightColorButton = new ColorButton(frame, "m_highlightColorButton");
    m_highlightColorButton->setMaximumSize(QSize(50, 25));

    layout4->addWidget(m_highlightColorButton, 6, 1);

    textLabel1_3 = new QLabel(frame, "textLabel1_3");

    layout4->addWidget(textLabel1_3, 0, 0);

    textLabel1_3_2_2 = new QLabel(frame, "textLabel1_3_2_2");

    layout4->addWidget(textLabel1_3_2_2, 3, 0);

    textLabel1_3_2_2_2 = new QLabel(frame, "textLabel1_3_2_2_2");

    layout4->addWidget(textLabel1_3_2_2_2, 4, 0);

    m_selForeColorButton = new ColorButton(frame, "m_selForeColorButton");
    m_selForeColorButton->setMaximumSize(QSize(50, 25));

    layout4->addWidget(m_selForeColorButton, 2, 1);

    m_backColorButton = new ColorButton(frame, "m_backColorButton");
    m_backColorButton->setMaximumSize(QSize(50, 25));

    layout4->addWidget(m_backColorButton, 1, 1);

    m_dataColorButton = new ColorButton(frame, "m_dataColorButton");
    m_dataColorButton->setMaximumSize(QSize(50, 25));

    layout4->addWidget(m_dataColorButton, 4, 1);

    m_selBackColorButton = new ColorButton(frame, "m_selBackColorButton");
    m_selBackColorButton->setMaximumSize(QSize(50, 25));

    layout4->addWidget(m_selBackColorButton, 3, 1);

    textLabel1_3_2_2_2_2 = new QLabel(frame, "textLabel1_3_2_2_2_2");

    layout4->addWidget(textLabel1_3_2_2_2_2, 5, 0);

    m_foreColorButton = new ColorButton(frame, "m_foreColorButton");
    m_foreColorButton->setMaximumSize(QSize(50, 25));

    layout4->addWidget(m_foreColorButton, 0, 1);

    textLabel1_3_3 = new QLabel(frame, "textLabel1_3_3");

    layout4->addWidget(textLabel1_3_3, 2, 0);

    textLabel1_3_2_2_2_2_2 = new QLabel(frame, "textLabel1_3_2_2_2_2_2");

    layout4->addWidget(textLabel1_3_2_2_2_2_2, 6, 0);

    textLabel1_3_2 = new QLabel(frame, "textLabel1_3_2");

    layout4->addWidget(textLabel1_3_2, 1, 0);
    layout6->addLayout(layout4);
    QSpacerItem *spacer_3 = new QSpacerItem(511, 20, QSizePolicy::Expanding,
	QSizePolicy::Minimum);

    layout6->addItem(spacer_3);
    WStackPageLayout_2->addLayout(layout6);

    QSpacerItem *spacer_4 = new QSpacerItem(31, 141, QSizePolicy::Minimum,
	QSizePolicy::Expanding);

    WStackPageLayout_2->addItem(spacer_4);
}

void ConfigureDialog::createTabsPage(QFrame * frame)
{
    WStackPageLayout = new QVBoxLayout(frame, 11, 6, "WStackPageLayout");
    layout8 = new QGridLayout(0, 1, 1, 0, 6, "layout8");

    m_utilitiesCombo = new QComboBox(FALSE, frame, "m_utilitiesCombo");
    layout8->addWidget(m_utilitiesCombo, 3, 1);

    textLabel1 = new QLabel(frame, "textLabel1");
    layout8->addWidget(textLabel1, 0, 0);

    textLabel1_2_4 = new QLabel(frame, "textLabel1_2_4");
    layout8->addWidget(textLabel1_2_4, 4, 0);

    textLabel1_2_2 = new QLabel(frame, "textLabel1_2_2");
    layout8->addWidget(textLabel1_2_2, 2, 0);

    m_channelCombo = new QComboBox(FALSE, frame, "m_channelCombo");
    layout8->addWidget(m_channelCombo, 1, 1);

    m_dialogCombo = new QComboBox(FALSE, frame, "m_dialogCombo");
    layout8->addWidget(m_dialogCombo, 2, 1);

    m_tabCombo = new QComboBox(FALSE, frame, "m_tabCombo");
    layout8->addWidget(m_tabCombo, 0, 1);

    m_shortenBox = new QSpinBox(frame, "m_shortenBox");
    m_shortenBox->setValue(20);

    layout8->addWidget(m_shortenBox, 4, 1);
    textLabel1_2 = new QLabel(frame, "textLabel1_2");

    layout8->addWidget(textLabel1_2, 1, 0);
    textLabel1_2_3 = new QLabel(frame, "textLabel1_2_3");

    layout8->addWidget(textLabel1_2_3, 3, 0);
    WStackPageLayout->addLayout(layout8);

    layout19 = new QGridLayout(0, 1, 1, 0, 6, "layout19");
    m_labelCheckbox = new QCheckBox(frame, "m_labelCheckbox");

    layout19->addWidget(m_labelCheckbox, 1, 1);
    m_noticesCheckbox = new QCheckBox(frame, "m_noticesCheckbox");

    layout19->addWidget(m_noticesCheckbox, 1, 0);
    m_messagesCheckbox = new QCheckBox(frame, "m_messagesCheckbox");

    layout19->addWidget(m_messagesCheckbox, 0, 0);
    m_popCheckbox = new QCheckBox(frame, "m_popCheckbox");

    layout19->addWidget(m_popCheckbox, 0, 1);
    m_groupCheckbox = new QCheckBox(frame, "m_groupCheckbox");

    layout19->addWidget(m_groupCheckbox, 2, 0);
    WStackPageLayout->addLayout(layout19);
    QSpacerItem *spacer = new QSpacerItem(41, 121, QSizePolicy::Minimum,
	QSizePolicy::Expanding);

    WStackPageLayout->addItem(spacer);

}

void ConfigureDialog::createUserlistPage(QFrame * frame)
{
    WStackPageLayout_4 = new QVBoxLayout(frame, 11, 6, "WStackPageLayout_4");

    layout27 = new QGridLayout(0, 1, 1, 0, 6, "layout27");

    m_dblclickCombo = new QComboBox(FALSE, frame, "m_dblclickCombo");
    m_dblclickCombo->setEditable(TRUE);

    layout27->addMultiCellWidget(m_dblclickCombo, 1, 1, 1, 2);

    textLabel1_4_2 = new QLabel(frame, "textLabel1_4_2");
    layout27->addWidget(textLabel1_4_2, 1, 0);

    textLabel1_4 = new QLabel(frame, "textLabel1_4");
    layout27->addWidget(textLabel1_4, 0, 0);

    m_userlistFontCheck = new QCheckBox(frame, "m_userlistFontCheck");
    layout27->addWidget(m_userlistFontCheck, 2, 2);

    m_userlistCombo = new QComboBox(FALSE, frame, "m_userlistCombo");
    layout27->addMultiCellWidget(m_userlistCombo, 0, 0, 1, 2);

    m_hostnameCheck = new QCheckBox(frame, "m_hostnameCheck");
    layout27->addMultiCellWidget(m_hostnameCheck, 2, 2, 0, 1);
    WStackPageLayout_4->addLayout(layout27);

    groupBox8 = new QGroupBox(frame, "groupBox8");
    groupBox8->setColumnLayout(0, Qt::Vertical);
    groupBox8->layout()->setSpacing(6);
    groupBox8->layout()->setMargin(11);
    groupBox8Layout = new QVBoxLayout(groupBox8->layout());
    groupBox8Layout->setAlignment(Qt::AlignTop);

    m_userlistButtonCheck = new QCheckBox(groupBox8, "m_userlistButtonCheck");
    groupBox8Layout->addWidget(m_userlistButtonCheck);

    m_userlistView = new EditListView(groupBox8, "buttons.conf", button_list);
    groupBox8Layout->addWidget(m_userlistView);
    /* layout30_2 = new QHBoxLayout(0, 0, 6, "layout30_2");

       m_userlistView = new QListView(groupBox8, "m_userlistView");
       m_userlistView->addColumn(tr("Name"));
       m_userlistView->addColumn(tr("Command"));
       layout30_2->addWidget(m_userlistView);

       layout29 = new QVBoxLayout(0, 0, 6, "layout29");

       m_newUserlistButton = new QPushButton(groupBox8, "m_newUserlistButton");
       layout29->addWidget(m_newUserlistButton);

       m_deleteUserlistButton = new QPushButton(groupBox8, "m_deleteUserlistButton");
       layout29->addWidget(m_deleteUserlistButton);

       m_helpUserlistButton = new QPushButton(groupBox8, "m_helpUserlistButton");
       layout29->addWidget(m_helpUserlistButton);
       QSpacerItem *spacer_10 = new QSpacerItem(31, 81, QSizePolicy::Minimum,
       QSizePolicy::Expanding);

       layout29->addItem(spacer_10);
       layout30_2->addLayout(layout29); */

    // groupBox8Layout->addLayout(layout30_2);
    WStackPageLayout_4->addWidget(groupBox8);
    QSpacerItem *spacer_11 = new QSpacerItem(20, 20, QSizePolicy::Minimum,
	QSizePolicy::Expanding);

    WStackPageLayout_4->addItem(spacer_11);

}

void ConfigureDialog::createChattingGeneralPage(QFrame * frame)
{
    WStackPageLayout_5 = new QVBoxLayout(frame, 11, 6, "WStackPageLayout_5");

    layout63 = new QGridLayout(0, 1, 1, 0, 6, "layout63");
    m_reconnectCombo = new QSpinBox(frame, "m_reconnectCombo");

    layout63->addWidget(m_reconnectCombo, 6, 1);
    textLabel2_3_3_2_2 = new QLabel(frame, "textLabel2_3_3_2_2");

    layout63->addWidget(textLabel2_3_3_2_2, 6, 0);
    textLabel2_3_3_2 = new QLabel(frame, "textLabel2_3_3_2");

    layout63->addWidget(textLabel2_3_3_2, 4, 0);
    m_awayMsgEdit = new QLineEdit(frame, "m_awayMsgEdit");

    layout63->addMultiCellWidget(m_awayMsgEdit, 2, 2, 1, 2);

    textLabel3_2 = new QLabel(frame, "textLabel3_2");
    textLabel3_2->setIndent(10);

    layout63->addMultiCellWidget(textLabel3_2, 5, 5, 1, 2);

    textLabel2_3_3 = new QLabel(frame, "textLabel2_3_3");
    layout63->addWidget(textLabel2_3_3, 2, 0);

    textLabel2_3_2 = new QLabel(frame, "textLabel2_3_2");

    layout63->addWidget(textLabel2_3_2, 1, 0);

    textLabel3 = new QLabel(frame, "textLabel3");
    textLabel3->setIndent(10);

    layout63->addMultiCellWidget(textLabel3, 3, 3, 1, 2);
    m_quitMsgEdit = new QLineEdit(frame, "m_quitMsgEdit");

    layout63->addMultiCellWidget(m_quitMsgEdit, 0, 0, 1, 2);
    m_highlightMsgEdit = new QLineEdit(frame, "m_highlightMsgEdit");

    layout63->addMultiCellWidget(m_highlightMsgEdit, 4, 4, 1, 2);
    m_partMsgEdit = new QLineEdit(frame, "m_partMsgEdit");

    layout63->addMultiCellWidget(m_partMsgEdit, 1, 1, 1, 2);
    QSpacerItem *spacer_12 = new QSpacerItem(185, 21, QSizePolicy::Expanding,
	QSizePolicy::Minimum);

    layout63->addItem(spacer_12, 6, 2);

    textLabel2_3 = new QLabel(frame, "textLabel2_3");

    layout63->addWidget(textLabel2_3, 0, 0);
    WStackPageLayout_5->addLayout(layout63);

    m_awayCheck = new QCheckBox(frame, "m_awayCheck");
    WStackPageLayout_5->addWidget(m_awayCheck);

    m_autoAwayCheck = new QCheckBox(frame, "m_autoAwayCheck");
    WStackPageLayout_5->addWidget(m_autoAwayCheck);

    m_announceCheck = new QCheckBox(frame, "m_announceCheck");
    WStackPageLayout_5->addWidget(m_announceCheck);
    QSpacerItem *spacer_13 = new QSpacerItem(31, 51, QSizePolicy::Minimum,
	QSizePolicy::Expanding);

    WStackPageLayout_5->addItem(spacer_13);

}

void ConfigureDialog::createUserPage(QFrame * frame)
{
    QVBoxLayout *vb = new QVBoxLayout(frame, 1, 1, "WStackPageLayout_6");
    UserOptionsView *ov = new UserOptionsView(frame);

    vb->addWidget(ov);
}

void ConfigureDialog::createLoggingPage(QFrame * frame)
{
    WStackPageLayout_6 = new QVBoxLayout(frame, 11, 6, "WStackPageLayout_6");

    layout79 = new QGridLayout(0, 1, 1, 0, 6, "layout79");
    m_stampLogEdit = new QLineEdit(frame, "m_stampLogEdit");

    layout79->addWidget(m_stampLogEdit, 4, 1);

    m_stampLogCheck = new QCheckBox(frame, "m_stampLogCheck");

    layout79->addMultiCellWidget(m_stampLogCheck, 3, 3, 0, 1);

    textLabel5 = new QLabel(frame, "textLabel5");
    textLabel5->setIndent(10);

    layout79->addWidget(textLabel5, 2, 1);

    m_logEdit = new QLineEdit(frame, "m_logEdit");

    layout79->addWidget(m_logEdit, 1, 1);

    textLabel4 = new QLabel(frame, "textLabel4");

    layout79->addWidget(textLabel4, 1, 0);

    m_logCheck = new QCheckBox(frame, "m_logCheck");

    layout79->addMultiCellWidget(m_logCheck, 0, 0, 0, 1);

    textLabel5_2 = new QLabel(frame, "textLabel5_2");
    textLabel5_2->setIndent(10);

    layout79->addWidget(textLabel5_2, 5, 1);

    textLabel4_2 = new QLabel(frame, "textLabel4_2");

    layout79->addWidget(textLabel4_2, 4, 0);
    WStackPageLayout_6->addLayout(layout79);
    QSpacerItem *spacer_14 = new QSpacerItem(21, 231, QSizePolicy::Minimum,
	QSizePolicy::Expanding);

    WStackPageLayout_6->addItem(spacer_14);
}

void ConfigureDialog::createEventsTextPage(QFrame * frame)
{
    QVBoxLayout *lay = new QVBoxLayout(frame, 12, 6, "lay");

    m_eventListView = new KListView(frame, "m_eventListView");
    m_eventListView->addColumn(tr("Event"));
    m_eventListView->addColumn(tr("Text"));

    m_eventListView->setFullWidth(true);
    m_eventListView->setItemsMovable(true);
    m_eventListView->setItemsRenameable(true);
    m_eventListView->setAllColumnsShowFocus(true);
    m_eventListView->setRenameable(0, false);
    m_eventListView->setRenameable(1, true);

    lay->addWidget(m_eventListView);

    m_eventListViewHelp = new QLabel(frame, "m_eventListViewHelp");
    m_eventListViewHelp->setText(tr("Special format codes for \"<b>Found IP</b>\":<br>\n" "<br>\n" "$1 - IP"));
    lay->addWidget(m_eventListViewHelp);

}

void ConfigureDialog::createNotificationsPage(QFrame * frame)
{
    QVBoxLayout *lay = new QVBoxLayout(frame, 12, 6, "lay");

    m_notifyWidget = new KNotify::KNotifyWidget(frame, "knotify widget");
//m_notifyWidget->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum, false));
    KNotify::Application * app = m_notifyWidget->addApplicationEvents(QString::fromUtf8(xchatapp->aboutData()->appName()) + QString::fromLatin1("/eventsrc"));

    if(app) {
	m_notifyWidget->addVisibleApp(app);
	m_notifyWidget->sort();
    }

    lay->addWidget(m_notifyWidget);
  //  QSpacerItem *spacer = new QSpacerItem(21, 231, QSizePolicy::Minimum,
//	QSizePolicy::Expanding);

  //  lay->addItem(spacer);

}

void ConfigureDialog::createPluginsPage(QFrame * frame)
{
    QGridLayout *lay = new QGridLayout(frame, 1, 1, 12, 6, "Form1Layout");

    m_pluginListView = new KListView(frame, "m_pluginListView");
    m_pluginListView->addColumn(tr("Name"));
    m_pluginListView->addColumn(tr("File"));
    m_pluginListView->addColumn(tr("Version"));
    m_pluginListView->addColumn(tr("Description"));
    m_pluginListView->setFullWidth(true);
       m_pluginListView->setAllColumnsShowFocus(true);
       
    lay->addMultiCellWidget(m_pluginListView, 0, 2, 0, 0);

    m_pluginLoadButton = new QPushButton(frame, "m_pluginLoadButton");

    lay->addWidget(m_pluginLoadButton, 0, 1);

    m_pluginUnloadButton = new QPushButton(frame, "m_pluginUnloadButton");

    lay->addWidget(m_pluginUnloadButton, 1, 1);
    QSpacerItem *spacer = new QSpacerItem(61, 291, QSizePolicy::Minimum, QSizePolicy::Expanding);

    lay->addItem(spacer, 2, 1);
    m_pluginLoadButton->setText(tr("Load..."));
    m_pluginUnloadButton->setText(tr("Unload"));

}

void ConfigureDialog::createNetworkGeneralPage(QFrame * frame)
{
QVBoxLayout *lay = new QVBoxLayout( frame, 12, 6, "Form8Layout"); 

   QHBoxLayout* m_bindLayout = new QHBoxLayout( 0, 0, 6, "m_bindLayout"); 

    m_bindLabel = new QLabel( frame, "m_bindLabel" );
    m_bindLayout->addWidget( m_bindLabel );

    m_bindEdit = new QLineEdit( frame, "m_bindEdit" );
    m_bindLayout->addWidget( m_bindEdit );
    lay->addLayout( m_bindLayout );

    m_proxyGroup = new QGroupBox( frame, "m_proxyGroup" );
    m_proxyGroup->setColumnLayout(0, Qt::Vertical );
    m_proxyGroup->layout()->setSpacing( 6 );
    m_proxyGroup->layout()->setMargin( 12 );
   QGridLayout* m_proxyGroupLayout = new QGridLayout( m_proxyGroup->layout() );
    m_proxyGroupLayout->setAlignment( Qt::AlignTop );

    m_serverLabel = new QLabel( m_proxyGroup, "m_serverLabel" );
    m_proxyGroupLayout->addWidget( m_serverLabel, 2, 0 );

    m_portLabel = new QLabel( m_proxyGroup, "m_portLabel" );
    m_proxyGroupLayout->addWidget( m_portLabel, 3, 0 );

    m_typeLabel = new QLabel( m_proxyGroup, "m_typeLabel" );
    m_proxyGroupLayout->addWidget( m_typeLabel, 1, 0 );

    m_proxyCheck = new QCheckBox( m_proxyGroup, "m_proxyCheck" );
    m_proxyGroupLayout->addMultiCellWidget( m_proxyCheck, 0, 0, 0, 2 );

    m_proxyServerEdit = new QLineEdit( m_proxyGroup, "m_proxyServerEdit" );
    m_proxyGroupLayout->addWidget( m_proxyServerEdit, 2, 1 );

    m_proxyTypeCombo = new QComboBox( FALSE, m_proxyGroup, "m_proxyTypeCombo" );
    m_proxyGroupLayout->addWidget( m_proxyTypeCombo, 1, 1 );

    m_proxyPortBox = new QSpinBox( m_proxyGroup, "m_proxyPortBox" );

    m_proxyGroupLayout->addWidget( m_proxyPortBox, 3, 1 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    m_proxyGroupLayout->addItem( spacer, 1, 2 );
    lay->addWidget( m_proxyGroup );
    QSpacerItem* spacer_2 = new QSpacerItem( 21, 201, QSizePolicy::Minimum, QSizePolicy::Expanding );
    lay->addItem( spacer_2 );
    
        m_bindLabel->setText( tr( "Local address to bind:" ) );
    m_proxyGroup->setTitle( tr( "Proxy:" ) );
    m_serverLabel->setText( tr( "Server:" ) );
    m_portLabel->setText( tr( "Port:" ) );
    m_typeLabel->setText( tr( "Type:" ) );
    m_proxyCheck->setText( tr( "Use proxy server" ) );
    m_proxyTypeCombo->clear();
    m_proxyTypeCombo->insertItem( tr( "HTTP" ) );
    m_proxyTypeCombo->insertItem( tr( "Socks4" ) );
    m_proxyTypeCombo->insertItem( tr( "Socks5" ) );
    m_proxyTypeCombo->insertItem( tr( "Wingate" ) );

 }

void ConfigureDialog::createXferPage(QFrame * frame)
{
QGridLayout  *lay = new QGridLayout( frame, 1, 1, 12, 6, "lay"); 

    m_downloadLabel = new QLabel( frame, "m_downloadLabel" );
    lay->addWidget( m_downloadLabel, 0, 0 );

    m_completedChooseButton = new QPushButton( frame, "m_completedChooseButton" );
    lay->addWidget( m_completedChooseButton, 1, 3 );

    m_downloadChooseButton = new QPushButton( frame, "m_downloadChooseButton" );
    lay->addWidget( m_downloadChooseButton, 0, 3 );

   QHBoxLayout* m_limitLayout = new QHBoxLayout( 0, 0, 6, "m_limitLayout"); 

    m_uploadLimitCheck = new QCheckBox( frame, "m_uploadLimitCheck" );
    m_limitLayout->addWidget( m_uploadLimitCheck );

    m_minUploadPortBox = new QSpinBox( frame, "m_minUploadPortBox" );
    m_limitLayout->addWidget( m_minUploadPortBox );

    m_toLabel = new QLabel( frame, "m_toLabel" );
    m_limitLayout->addWidget( m_toLabel );

    m_maxUploadPortBox = new QSpinBox( frame, "m_maxUploadPortBox" );
    m_limitLayout->addWidget( m_maxUploadPortBox );
    QSpacerItem* spacer = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    m_limitLayout->addItem( spacer );

    lay->addMultiCellLayout( m_limitLayout, 3, 3, 0, 3 );

    m_completedLabel = new QLabel( frame, "m_completedLabel" );
    lay->addWidget( m_completedLabel, 1, 0 );

    m_completedEdit = new QLineEdit( frame, "m_completedEdit" );
    lay->addMultiCellWidget( m_completedEdit, 1, 1, 1, 2 );

    m_downloadEdit = new QLineEdit( frame, "m_downloadEdit" );
    lay->addMultiCellWidget( m_downloadEdit, 0, 0, 1, 2 );

    m_uploadLabel = new QLabel( frame, "m_uploadLabel" );
    lay->addWidget( m_uploadLabel, 2, 0 );

    m_addressEdit = new QLineEdit( frame, "m_addressEdit" );
    lay->addMultiCellWidget( m_addressEdit, 2, 2, 1, 3 );

    m_autoOpenXferCheck = new QCheckBox( frame, "m_autoOpenXferCheck" );
    lay->addMultiCellWidget( m_autoOpenXferCheck, 4, 4, 0, 1 );

    m_cvtSpacesCheck = new QCheckBox( frame, "m_cvtSpacesCheck" );
    lay->addMultiCellWidget( m_cvtSpacesCheck, 4, 4, 2, 3 );

    m_saveNickCheck = new QCheckBox( frame, "m_saveNickCheck" );
    lay->addMultiCellWidget( m_saveNickCheck, 5, 5, 0, 1 );

    m_getAddressCheck = new QCheckBox( frame, "m_getAddressCheck" );

    lay->addMultiCellWidget( m_getAddressCheck, 5, 5, 2, 3 );


    m_speedGroupBox = new QGroupBox( frame, "m_speedGroupBox" );
    m_speedGroupBox->setColumnLayout(0, Qt::Vertical );
    m_speedGroupBox->layout()->setSpacing( 6 );
    m_speedGroupBox->layout()->setMargin( 12 );
    QGridLayout* m_speedGroupBoxLayout = new QGridLayout( m_speedGroupBox->layout() );
    m_speedGroupBoxLayout->setAlignment( Qt::AlignTop );

    m_ulSpeedFileLabel = new QLabel( m_speedGroupBox, "m_ulSpeedFileLabel" );
    m_speedGroupBoxLayout->addWidget( m_ulSpeedFileLabel, 1, 0 );

    m_dlSpeedTotalLabel = new QLabel( m_speedGroupBox, "m_dlSpeedTotalLabel" );
    m_speedGroupBoxLayout->addWidget( m_dlSpeedTotalLabel, 2, 0 );

    m_ulSpeedTotalLabel = new QLabel( m_speedGroupBox, "m_ulSpeedTotalLabel" );
    m_speedGroupBoxLayout->addWidget( m_ulSpeedTotalLabel, 3, 0 );

    m_dlSpeedFileLabel = new QLabel( m_speedGroupBox, "m_dlSpeedFileLabel" );
    m_speedGroupBoxLayout->addWidget( m_dlSpeedFileLabel, 0, 0 );

    m_dlSpeedFileBox = new QSpinBox( m_speedGroupBox, "m_dlSpeedFileBox" );
    m_speedGroupBoxLayout->addWidget( m_dlSpeedFileBox, 0, 1 );

    m_ulSpeedFileBox = new QSpinBox( m_speedGroupBox, "m_ulSpeedFileBox" );
    m_speedGroupBoxLayout->addWidget( m_ulSpeedFileBox, 1, 1 );

    m_dlSpeedTotalBox = new QSpinBox( m_speedGroupBox, "m_dlSpeedTotalBox" );
    m_speedGroupBoxLayout->addWidget( m_dlSpeedTotalBox, 2, 1 );

    m_ulSpeedTotalBox = new QSpinBox( m_speedGroupBox, "m_ulSpeedTotalBox" );

    m_speedGroupBoxLayout->addWidget( m_ulSpeedTotalBox, 3, 1 );
    QSpacerItem* spacer_2 = new QSpacerItem( 240, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    m_speedGroupBoxLayout->addItem( spacer_2, 1, 2 );

    lay->addMultiCellWidget( m_speedGroupBox, 6, 6, 0, 3 );

    QSpacerItem* spacer_3 = new QSpacerItem( 31, 71, QSizePolicy::Minimum, QSizePolicy::Expanding );
    lay->addItem( spacer_3, 7, 0 );
m_downloadLabel->setText( tr( "Download files to:" ) );
    m_completedChooseButton->setText( tr( "Choose..." ) );
    m_downloadChooseButton->setText( tr( "Choose..." ) );
    m_uploadLimitCheck->setText( tr( "Limit upload ports from:" ) );
    m_toLabel->setText( tr( "to:" ) );
    m_completedLabel->setText( tr( "Move completed files to:" ) );
    m_uploadLabel->setText( tr( "Upload IP address:" ) );
    m_speedGroupBox->setTitle( tr( "Speed Limiting:" ) );
    m_ulSpeedFileLabel->setText( tr( "Maximum upload speed per file:" ) );
    m_dlSpeedTotalLabel->setText( tr( "Maximum total download speed:" ) );
    m_ulSpeedTotalLabel->setText( tr( "Maximum total upload speed:" ) );
    m_dlSpeedFileLabel->setText( tr( "Maximum download speed per file:" ) );
    m_autoOpenXferCheck->setText( tr( "Automatically open file transfer list" ) );
    m_cvtSpacesCheck->setText( tr( "Convert spaces to underscores" ) );
    m_saveNickCheck->setText( tr( "Save nickname in filenames" ) );
    m_getAddressCheck->setText( tr( "Get local address from server" ) );
}


ConfigureDialog::~ConfigureDialog()
{
}

void ConfigureDialog::slotNickCompButtonToggled(bool on)
{
    if(on) {
	m_nickCompLabel->setEnabled(true);
	m_nickCompEdit->setEnabled(true);
    } else {
	m_nickCompLabel->setEnabled(false);
	m_nickCompEdit->setEnabled(false);
    }

}

void ConfigureDialog::slotUserlistButtonToggled(bool on)
{
    if(on) {
	m_userlistView->setEnabled(true);
    } else {
	m_userlistView->setEnabled(false);
    }

}

void ConfigureDialog::slotLogCheckToggled(bool on)
{
    if(on) {
	textLabel4->setEnabled(true);
	m_logEdit->setEnabled(true);
	textLabel5->setEnabled(true);
    } else {
	textLabel4->setEnabled(false);
	m_logEdit->setEnabled(false);
	textLabel5->setEnabled(false);
    }

}

void ConfigureDialog::slotStampLogCheckToggled(bool on)
{
    if(on) {
	m_stampLogEdit->setEnabled(true);
	textLabel4_2->setEnabled(true);
	textLabel5_2->setEnabled(true);

    } else {
	m_stampLogEdit->setEnabled(false);
	textLabel4_2->setEnabled(false);
	textLabel5_2->setEnabled(false);

    }
}

void ConfigureDialog::slotProxyCheckClicked()
{
    qWarning( "Form8::slotProxyCheckClicked(): Not implemented yet" );
}

void ConfigureDialog::slotUploadLimitCheckClicked()
{
    qWarning( "Form9::slotUploadLimitCheckClicked(): Not implemented yet" );
}

void ConfigureDialog::slotEventListViewCurrentChanged(QListViewItem * it)
{
    EventlistItem *i = (EventlistItem *) it;

    m_eventListViewHelp->setText(i->helpText);
}



ColorButton::ColorButton(QWidget * parent, const char *name)
:KColorButton(parent, name)
{
}

ColorButton::~ColorButton()
{
}

//taken from Web qstyle in kdelibs
static QColor contrastingForeground(const QColor & fg, const QColor & bg)
{
    int h, s, vbg, vfg;

    bg.getHsv(&h, &s, &vbg);
    fg.getHsv(&h, &s, &vfg);

    int diff(vbg - vfg);

    if((diff > -72) && (diff < 72)) {
	return (vbg < 128) ? Qt::white : Qt::black;
    } else {
	return fg;
    }
}

void ColorButton::drawButtonLabel(QPainter * painter)
{
    int x, y, w, h;
    QRect r = style().subRect(QStyle::SR_PushButtonContents, this);

    r.rect(&x, &y, &w, &h);

 //   int margin = 2;		//style().pixelMetric(QStyle::PM_ButtonMargin, this)-2;

int margin=h-fontMetrics().height()+3;

//x += //margin-2;
    //y += //margin-2;
    w -= margin;
    h -= margin;


//fontMetrics().height();


    if(isOn() || isDown()) {
	x += style().pixelMetric(QStyle::PM_ButtonShiftHorizontal, this);
	y += style().pixelMetric(QStyle::PM_ButtonShiftVertical, this);
    }

    QColor fillCol = isEnabled()? color() : backgroundColor();

    if(fillCol.isValid())
    {
    
	painter->fillRect(x + margin/2, y + margin/2, w, h+1, fillCol);
    }
    painter->setPen(contrastingForeground(colorGroup().foreground(), color()));
    painter->drawText(r, Qt::AlignCenter, text());
}

EventlistItem::EventlistItem(QListView * list, QString name, QString text, QString _helpText):
KListViewItem(list, name, text)
{
    helpText = _helpText;
}

EventlistItem::~EventlistItem()
{
}
