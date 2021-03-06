#include "mainwindow.h"
#include "extraview.h"

#include <qvariant.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qfontmetrics.h>
#include <qwhatsthis.h>
#include <kdebug.h>
#include <klistview.h>
#include <qheader.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qsplitter.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <kmessagebox.h>
#include <klistview.h>
#include <kpushbutton.h>

#include "../common/xchat.h"
#include "../common/xchatc.h"
#include "../common/outbound.h"
#include "../common/util.h"



#include "tabwidget.h"
#include "fe-kde.h"


BanListView::BanListView(QWidget * parent, MainWindow * w, server * s)
    :  ContainerView(parent, "BanList", w, s)
{
    BanlistViewLayout = new QGridLayout(this, 1, 1, 12, 6, "BanlistViewLayout");

    m_clearButton = new QPushButton(this, "m_clearButton");

    BanlistViewLayout->addWidget(m_clearButton, 2, 1);
    QSpacerItem *spacer = new QSpacerItem(41, 280, QSizePolicy::Minimum, QSizePolicy::Expanding);

    BanlistViewLayout->addItem(spacer, 4, 1);

    m_banlistView = new KListView(this, "m_banlistView");
    m_banlistView->addColumn(tr("Mask"));
    m_banlistView->addColumn(tr("From"));
    m_banlistView->addColumn(tr("Date"));
    BanlistViewLayout->addMultiCellWidget(m_banlistView, 0, 4, 0, 0);

    m_unbanButton = new QPushButton(this, "m_unbanButton");
    BanlistViewLayout->addWidget(m_unbanButton, 0, 1);

    m_refreshButton = new QPushButton(this, "m_refreshButton");
    BanlistViewLayout->addWidget(m_refreshButton, 3, 1);

    m_cropButton = new QPushButton(this, "m_cropButton");
    BanlistViewLayout->addWidget(m_cropButton, 1, 1);

    connect(m_unbanButton, SIGNAL(clicked()), this, SLOT(slotUnbanButtonClicked()));
    connect(m_cropButton, SIGNAL(clicked()), this, SLOT(slotCropButtonClicked()));
    connect(m_clearButton, SIGNAL(clicked()), this, SLOT(slotClearButtonClicked()));
    connect(m_refreshButton, SIGNAL(clicked()), this, SLOT(slotRefreshButtonClicked()));

    setCaption(tr("Ban List"));
    m_clearButton->setText(tr("C&lear"));
    m_clearButton->setAccel(QKeySequence(tr("Alt+L")));
    m_banlistView->header()->setLabel(0, tr("Mask"));
    m_banlistView->header()->setLabel(1, tr("From"));
    m_banlistView->header()->setLabel(2, tr("Date"));
    m_unbanButton->setText(tr("&Unban"));
    m_unbanButton->setAccel(QKeySequence(tr("Alt+U")));
    m_refreshButton->setText(tr("&Refresh"));
    m_refreshButton->setAccel(QKeySequence(tr("Alt+R")));
    m_cropButton->setText(tr("&Crop"));
    m_cropButton->setAccel(QKeySequence(tr("Alt+C")));
}

BanListView::~BanListView()
{
}


void BanListView::slotUnbanButtonClicked()
{

}

void BanListView::slotCropButtonClicked()
{

}

void BanListView::slotClearButtonClicked()
{

}

void BanListView::slotRefreshButtonClicked()
{

}

ChatListView::ChatListView(QWidget * parent, MainWindow * w, server * s)
    :  ContainerView(parent, "ChatList", w, s)
{
    ChatListViewLayout = new QGridLayout(this, 1, 1, 12, 6, "ChatListViewLayout");

    m_cancelButton = new QPushButton(this, "m_cancelButton");
    ChatListViewLayout->addWidget(m_cancelButton, 1, 1);

    m_chatListView = new KListView(this, "m_chatListView");
    m_chatListView->addColumn(tr("Status"));
    m_chatListView->addColumn(tr("User"));
    m_chatListView->addColumn(tr("Time"));

    ChatListViewLayout->addMultiCellWidget(m_chatListView, 0, 2, 0, 0);
    QSpacerItem *spacer = new QSpacerItem(31, 221, QSizePolicy::Minimum, QSizePolicy::Expanding);

    ChatListViewLayout->addItem(spacer, 2, 1);

    m_acceptButton = new QPushButton(this, "m_acceptButton");
    ChatListViewLayout->addWidget(m_acceptButton, 0, 1);

    setCaption(tr("Chats"));
    m_cancelButton->setText(tr("&Cancel"));
    m_cancelButton->setAccel(QKeySequence(tr("Alt+C")));
    m_chatListView->header()->setLabel(0, tr("Status"));
    m_chatListView->header()->setLabel(1, tr("User"));
    m_chatListView->header()->setLabel(2, tr("Time"));
    m_acceptButton->setText(tr("&Accept"));
    m_acceptButton->setAccel(QKeySequence(tr("Alt+A")));
}

ChatListView::~ChatListView()
{
}

NotifyListView::NotifyListView(QWidget * parent, MainWindow * w, server * s)
    :  ContainerView(parent, "NotifyList", w, s)
{

    NotifyViewLayout = new QGridLayout(this, 1, 1, 12, 6, "NotifyViewLayout");

    m_notifyView = new KListView(this, "m_notifyView");
    m_notifyView->addColumn(tr("User"));
    m_notifyView->addColumn(tr("Status"));
    m_notifyView->addColumn(tr("Server"));
    m_notifyView->addColumn(tr("Last Seen"));
    NotifyViewLayout->addMultiCellWidget(m_notifyView, 0, 2, 0, 0);

    m_addButton = new QPushButton(this, "m_addButton");
    NotifyViewLayout->addWidget(m_addButton, 0, 1);

    m_removeButton = new QPushButton(this, "m_removeButton");
    NotifyViewLayout->addWidget(m_removeButton, 1, 1);
    QSpacerItem *spacer = new QSpacerItem(30, 120, QSizePolicy::Minimum, QSizePolicy::Expanding);

    NotifyViewLayout->addItem(spacer, 2, 1);
    setCaption(tr("Notify"));
    m_notifyView->header()->setLabel(0, tr("User"));
    m_notifyView->header()->setLabel(1, tr("Status"));
    m_notifyView->header()->setLabel(2, tr("Server"));
    m_notifyView->header()->setLabel(3, tr("Last Seen"));
    m_addButton->setText(tr("Add"));
    m_removeButton->setText(tr("Remove"));

    connect(m_addButton, SIGNAL(clicked()), this, SLOT(slotAddButtonClicked()));
    connect(m_removeButton, SIGNAL(clicked()), this, SLOT(slotRemoveButtonClicked()));
}

NotifyListView::~NotifyListView()
{
}


void NotifyListView::slotAddButtonClicked()
{
    qWarning("NotifyView::slotAddButtonClicked(): Not implemented yet");
}

void NotifyListView::slotRemoveButtonClicked()
{
    qWarning("NotifyView::slotRemoveButtonClicked(): Not implemented yet");
}


URLGrabberView::URLGrabberView(QWidget * parent, MainWindow * w, server * s)
    :  ContainerView(parent, "URLGrabber", w, s)
{
    URLViewLayout = new QGridLayout(this, 1, 1, 12, 6, "URLViewLayout");
    m_saveButton = new QPushButton(this, "m_saveButton");
    URLViewLayout->addWidget(m_saveButton, 0, 1);

    m_clearButton = new QPushButton(this, "m_clearButton");
    URLViewLayout->addWidget(m_clearButton, 1, 1);
    QSpacerItem *spacer = new QSpacerItem(20, 220, QSizePolicy::Minimum, QSizePolicy::Expanding);

    URLViewLayout->addItem(spacer, 2, 1);

    m_urlView = new KListView(this, "m_urlView");
    m_urlView->addColumn(tr("URL"));

    URLViewLayout->addMultiCellWidget(m_urlView, 0, 2, 0, 0);
    setCaption(tr("URL Grabber"));
    m_saveButton->setText(tr("&Save..."));
    m_saveButton->setAccel(QKeySequence(tr("Alt+S")));
    m_clearButton->setText(tr("&Clear"));
    m_clearButton->setAccel(QKeySequence(tr("Alt+C")));
    m_urlView->header()->setLabel(0, tr("URL"));

    connect(m_saveButton, SIGNAL(clicked()), this, SLOT(slotSaveButtonClicked()));
    connect(m_clearButton, SIGNAL(clicked()), this, SLOT(slotClearButtonClicked()));
}

URLGrabberView::~URLGrabberView()
{
}

void URLGrabberView::slotSaveButtonClicked()
{
    qWarning("URLView::slotSaveButtonClicked(): Not implemented yet");
}

void URLGrabberView::slotClearButtonClicked()
{
    qWarning("URLView::slotClearButtonClicked(): Not implemented yet");
}



XferView::XferView(QWidget * parent, MainWindow * w, server * s)
    :  ContainerView(parent, "XferView", w, s)
{
    FileXferViewLayout = new QVBoxLayout(this, 1, 1, "FileXferViewLayout");

    m_splitter = new QSplitter(this, "m_splitter");
    m_splitter->setOrientation(QSplitter::Vertical);

    QWidget *privateLayoutWidget = new QWidget(m_splitter, "m_downloadLayout");

    m_downloadLayout = new QGridLayout(privateLayoutWidget, 1, 1, 12, 6, "m_downloadLayout");

    m_dlInfoButton = new QPushButton(privateLayoutWidget, "m_dlInfoButton");
    m_downloadLayout->addWidget(m_dlInfoButton, 4, 1);

    m_startButton = new QPushButton(privateLayoutWidget, "m_startButton");
    m_downloadLayout->addWidget(m_startButton, 1, 1);

    m_downloadsView = new KListView(privateLayoutWidget, "m_downloadsView");
    m_downloadsView->addColumn(tr("Status"));
    m_downloadsView->addColumn(tr("From"));
    m_downloadsView->addColumn(tr("File"));
    m_downloadsView->addColumn(tr("Size"));
    m_downloadsView->addColumn(tr("ETA"));
    m_downloadsView->addColumn(tr("Position"));

    m_downloadLayout->addMultiCellWidget(m_downloadsView, 1, 5, 0, 0);
    QSpacerItem *spacer = new QSpacerItem(20, 73, QSizePolicy::Minimum, QSizePolicy::Expanding);

    m_downloadLayout->addItem(spacer, 5, 1);

    m_dlCancelButton = new QPushButton(privateLayoutWidget, "m_dlCancelButton");
    m_downloadLayout->addWidget(m_dlCancelButton, 2, 1);

    m_downloadsLabel = new QLabel(privateLayoutWidget, "m_downloadsLabel");
    m_downloadLayout->addMultiCellWidget(m_downloadsLabel, 0, 0, 0, 1);

    m_resumeButton = new QPushButton(privateLayoutWidget, "m_resumeButton");
    m_downloadLayout->addWidget(m_resumeButton, 3, 1);

    QWidget *privateLayoutWidget_2 = new QWidget(m_splitter, "m_uploadLayout");

    m_uploadLayout = new QGridLayout(privateLayoutWidget_2, 1, 1, 12, 6, "m_uploadLayout");
    m_uploadsLabel = new QLabel(privateLayoutWidget_2, "m_uploadsLabel");

    m_uploadLayout->addMultiCellWidget(m_uploadsLabel, 0, 0, 0, 1);
    QSpacerItem *spacer_2 = new QSpacerItem(20, 110, QSizePolicy::Minimum, QSizePolicy::Expanding);

    m_uploadLayout->addItem(spacer_2, 3, 1);

    m_ulInfoButton = new QPushButton(privateLayoutWidget_2, "m_ulInfoButton");
    m_uploadLayout->addWidget(m_ulInfoButton, 2, 1);

    m_ulCancelButton = new QPushButton(privateLayoutWidget_2, "m_ulCancelButton");
    m_uploadLayout->addWidget(m_ulCancelButton, 1, 1);

    m_uploadsView = new KListView(privateLayoutWidget_2, "m_uploadsView");
    m_uploadsView->addColumn(tr("Status"));
    m_uploadsView->addColumn(tr("To"));
    m_uploadsView->addColumn(tr("File"));
    m_uploadsView->addColumn(tr("Size"));
    m_uploadsView->addColumn(tr("ETA"));
    m_uploadsView->addColumn(tr("Position"));

    m_uploadLayout->addMultiCellWidget(m_uploadsView, 1, 3, 0, 0);
    FileXferViewLayout->addWidget(m_splitter);
    setCaption(tr("File Transfers"));
    m_dlInfoButton->setText(tr("&Information"));
    m_dlInfoButton->setAccel(QKeySequence(tr("Alt+I")));
    m_startButton->setText(tr("&Start"));
    m_startButton->setAccel(QKeySequence(tr("Alt+S")));
    m_downloadsView->header()->setLabel(0, tr("Status"));
    m_downloadsView->header()->setLabel(1, tr("From"));
    m_downloadsView->header()->setLabel(2, tr("File"));
    m_downloadsView->header()->setLabel(3, tr("Size"));
    m_downloadsView->header()->setLabel(4, tr("ETA"));
    m_downloadsView->header()->setLabel(5, tr("Position"));
    m_downloadsView->clear();

    m_dlCancelButton->setText(tr("&Cancel"));
    m_dlCancelButton->setAccel(QKeySequence(tr("Alt+C")));
    m_downloadsLabel->setText(tr("&Downloads:"));
    m_resumeButton->setText(tr("&Resume"));
    m_resumeButton->setAccel(QKeySequence(tr("Alt+R")));
    m_uploadsLabel->setText(tr("&Uploads:"));
    m_ulInfoButton->setText(tr("I&nformation"));
    m_ulInfoButton->setAccel(QKeySequence(tr("Alt+N")));
    m_ulCancelButton->setText(tr("C&ancel"));
    m_ulCancelButton->setAccel(QKeySequence(tr("Alt+A")));
    m_uploadsView->header()->setLabel(0, tr("Status"));
    m_uploadsView->header()->setLabel(1, tr("To"));
    m_uploadsView->header()->setLabel(2, tr("File"));
    m_uploadsView->header()->setLabel(3, tr("Size"));
    m_uploadsView->header()->setLabel(4, tr("ETA"));
    m_uploadsView->header()->setLabel(5, tr("Position"));
    m_uploadsView->clear();

    connect(m_startButton, SIGNAL(clicked()), this, SLOT(slotStartButtonClicked()));
    connect(m_dlCancelButton, SIGNAL(clicked()), this, SLOT(slotDlCancelButtonClicked()));
    connect(m_resumeButton, SIGNAL(clicked()), this, SLOT(slotResumeButtonClicked()));
    connect(m_dlInfoButton, SIGNAL(clicked()), this, SLOT(slotDlInfoButtonClicked()));
    connect(m_ulCancelButton, SIGNAL(clicked()), this, SLOT(slotUlCancelButtonClicked()));
    connect(m_ulInfoButton, SIGNAL(clicked()), this, SLOT(slotUlInfoButtonClicked()));

    m_downloadsLabel->setBuddy(m_downloadsView);
    m_uploadsLabel->setBuddy(m_uploadsView);
}

XferView::~XferView()
{
}


void XferView::slotStartButtonClicked()
{

}

void XferView::slotDlCancelButtonClicked()
{

}

void XferView::slotResumeButtonClicked()
{

}

void XferView::slotDlInfoButtonClicked()
{

}

void XferView::slotUlCancelButtonClicked()
{

}

void XferView::slotUlInfoButtonClicked()
{

}

EditListView::EditListView(QWidget * parent, const char *filename, GSList * entries, const char *name)
:QWidget(parent, name)
{
    if(!name)
	setName("RawlogView");
    m_entries = entries;
    QHBoxLayout *layout30_2 = new QHBoxLayout(this, 0, 6, "layout30_2");

    KListView *m_userlistView = new KListView(this, "m_userlistView");

    m_userlistView->addColumn(tr("Name"));
    m_userlistView->addColumn(tr("Command"));

    m_userlistView->setFullWidth(true);
    m_userlistView->setItemsMovable(true);
    m_userlistView->setItemsRenameable(true);
    m_userlistView->setAllColumnsShowFocus(true);
    m_userlistView->setRenameable(0, true);
    m_userlistView->setRenameable(1, true);

    struct popup *pop;
    GSList *list = entries;

    while(list) {
	pop = (struct popup *)list->data;
	new KListViewItem(m_userlistView, pop->name, pop->cmd);

	list = list->next;
    }

    layout30_2->addWidget(m_userlistView);

    QVBoxLayout *layout29 = new QVBoxLayout(0, 0, 6, "layout29");
    QPushButton *m_newUserlistButton = new QPushButton(this, "m_newUserlistButton");

    layout29->addWidget(m_newUserlistButton);

    QPushButton *m_deleteUserlistButton = new QPushButton(this, "m_deleteUserlistButton");

    layout29->addWidget(m_deleteUserlistButton);

    QPushButton *m_helpUserlistButton = new QPushButton(this, "m_helpUserlistButton");

    layout29->addWidget(m_helpUserlistButton);
    QSpacerItem *spacer_10 = new QSpacerItem(31, 81, QSizePolicy::Minimum,
	QSizePolicy::Expanding);

    layout29->addItem(spacer_10);
    layout30_2->addLayout(layout29);

    m_newUserlistButton->setText(tr("&New"));
    m_deleteUserlistButton->setText(tr("De&lete"));
    m_helpUserlistButton->setText(tr("&Help"));
}

EditListView::~EditListView()
{
}

UserOptionsView::UserOptionsView(QWidget * parent, const char *name)
:QWidget(parent, name)
{
    if(!name)
	setName("UserOptionsView");
    UserOptionsViewLayout = new QGridLayout(this, 1, 1, 12, 6, "UserOptionsViewLayout");

    nickEdit = new QLineEdit(this, "nickEdit");
    UserOptionsViewLayout->addWidget(nickEdit, 0, 1);

    altLabel = new QLabel(this, "altLabel");
    UserOptionsViewLayout->addWidget(altLabel, 1, 0);

    nickLabel = new QLabel(this, "nickLabel");
    UserOptionsViewLayout->addWidget(nickLabel, 0, 0);

    secEdit = new QLineEdit(this, "secEdit");
    UserOptionsViewLayout->addWidget(secEdit, 2, 1);

    secLabel = new QLabel(this, "secLabel");
    UserOptionsViewLayout->addWidget(secLabel, 2, 0);

    altEdit = new QLineEdit(this, "altEdit");
    UserOptionsViewLayout->addWidget(altEdit, 1, 1);

    realLabel = new QLabel(this, "realLabel");
    UserOptionsViewLayout->addWidget(realLabel, 4, 0);

    userEdit = new QLineEdit(this, "userEdit");
    UserOptionsViewLayout->addWidget(userEdit, 3, 1);

    userLabel = new QLabel(this, "userLabel");
    UserOptionsViewLayout->addWidget(userLabel, 3, 0);

    realEdit = new QLineEdit(this, "realEdit");
    UserOptionsViewLayout->addWidget(realEdit, 4, 1);

    QSpacerItem *spacer = new QSpacerItem(51, 133, QSizePolicy::Minimum, QSizePolicy::Expanding);

    UserOptionsViewLayout->addItem(spacer, 5, 1);

    secLabel->setBuddy(secEdit);
    realLabel->setBuddy(realEdit);
    userLabel->setBuddy(userEdit);
    altLabel->setBuddy(altEdit);
    nickLabel->setBuddy(nickEdit);

    secLabel->setText(tr("&Secondary:"));
    realLabel->setText(tr("&Real Name:"));
    userLabel->setText(tr("&Username:"));
    altLabel->setText(tr("&Alternate:"));
    nickLabel->setText(tr("&Nickname:"));

    nickEdit->setText(prefs.nick1);
    altEdit->setText(prefs.nick2);
    secEdit->setText(prefs.nick3);
    realEdit->setText(prefs.realname);
    userEdit->setText(prefs.username);

}

UserOptionsView::~UserOptionsView()
{

}
