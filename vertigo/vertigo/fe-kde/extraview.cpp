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

#include "../common/xchat.h"
#include "../common/xchatc.h"
#include "../common/outbound.h"
#include "../common/util.h"

#include "tabwidget.h"

using namespace Vertigo;


XChatContainerView::XChatContainerView (XChatMainWindow *w)
{
    setWindow(w);
}

void XChatContainerView::setWindow(XChatMainWindow *w)
{
    kdDebug() << "container setwindow"<<endl;
    m_window=w;
}

XChatMainWindow* XChatContainerView::window()
{
    return m_window;
}

XChatChanlistView::XChatChanlistView(QWidget * parent, XChatMainWindow * w, server * s, const char *name, WFlags fl)
    : QWidget( parent, name, fl ), XChatContainerView(w)
{
	m_window=w;
	m_server=s;
    if ( !name )
	setName( "XChatChanlistView" );
    XChatChanlistViewLayout = new QGridLayout( this, 1, 1, 11, 6, "XChatChanlistViewLayout"); 

    m_chanlistView = new KListView( this, "m_chanlistView" );
    m_chanlistView->addColumn( tr( "Channel" ) , m_chanlistView->fontMetrics().width("#halohalohalo"));
    m_chanlistView->addColumn( tr( "Users" ) );
    m_chanlistView->addColumn( tr( "Topic" ) );

m_chanlistView->setColumnWidthMode(0, QListView::Manual);
m_chanlistView->setFullWidth(true);
    
    XChatChanlistViewLayout->addMultiCellWidget( m_chanlistView, 1, 4, 0, 0 );

    m_refreshButton = new QPushButton( this, "m_refreshButton" );

    XChatChanlistViewLayout->addWidget( m_refreshButton, 2, 1 );

    m_saveButton = new QPushButton( this, "m_saveButton" );

    XChatChanlistViewLayout->addWidget( m_saveButton, 3, 1 );

    m_joinButton = new QPushButton( this, "m_joinButton" );

    XChatChanlistViewLayout->addWidget( m_joinButton, 1, 1 );
    QSpacerItem* spacer = new QSpacerItem( 31, 151, QSizePolicy::Minimum, QSizePolicy::Expanding );
    XChatChanlistViewLayout->addItem( spacer, 4, 1 );

    m_limitGroupBox = new QGroupBox( this, "m_limitGroupBox" );
    m_limitGroupBox->setColumnLayout(0, Qt::Vertical );
    m_limitGroupBox->layout()->setSpacing( 6 );
    m_limitGroupBox->layout()->setMargin( 11 );
    m_limitGroupBoxLayout = new QGridLayout( m_limitGroupBox->layout() );
    m_limitGroupBoxLayout->setAlignment( Qt::AlignTop );

    m_maximumLabel = new QLabel( m_limitGroupBox, "m_maximumLabel" );

    m_limitGroupBoxLayout->addWidget( m_maximumLabel, 1, 0 );

    m_minSpin = new QSpinBox( m_limitGroupBox, "m_minSpin" );
    m_minSpin->setMinimumSize( QSize( 30, 0 ) );

    m_limitGroupBoxLayout->addWidget( m_minSpin, 0, 1 );

    m_maxSpin = new QSpinBox( m_limitGroupBox, "m_maxSpin" );
    m_maxSpin->setMinimumSize( QSize( 30, 0 ) );

    m_limitGroupBoxLayout->addWidget( m_maxSpin, 1, 1 );

    m_minimumLabel = new QLabel( m_limitGroupBox, "m_minimumLabel" );

    m_limitGroupBoxLayout->addWidget( m_minimumLabel, 0, 0 );

    m_matchLabel = new QLabel( m_limitGroupBox, "m_matchLabel" );

    m_limitGroupBoxLayout->addWidget( m_matchLabel, 1, 2 );

    m_channelCheck = new QCheckBox( m_limitGroupBox, "m_channelCheck" );

    m_limitGroupBoxLayout->addWidget( m_channelCheck, 1, 3 );

    m_textLabel = new QLabel( m_limitGroupBox, "m_textLabel" );

    m_limitGroupBoxLayout->addWidget( m_textLabel, 0, 2 );

    m_topicCheck = new QCheckBox( m_limitGroupBox, "m_topicCheck" );

    m_limitGroupBoxLayout->addWidget( m_topicCheck, 1, 4 );

    m_textEdit = new QLineEdit( m_limitGroupBox, "m_textEdit" );

    m_limitGroupBoxLayout->addMultiCellWidget( m_textEdit, 0, 0, 3, 5 );

    m_applyButton = new QPushButton( m_limitGroupBox, "m_applyButton" );

    m_limitGroupBoxLayout->addWidget( m_applyButton, 1, 5 );

    XChatChanlistViewLayout->addMultiCellWidget( m_limitGroupBox, 0, 0, 0, 1 );
    resize( QSize(570, 456).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( m_applyButton, SIGNAL( clicked() ), this, SLOT( slotApplyButtonClicked() ) );
    connect( m_joinButton, SIGNAL( clicked() ), this, SLOT( slotJoinButtonClicked() ) );
    connect( m_refreshButton, SIGNAL( clicked() ), this, SLOT( slotRefreshButtonClicked() ) );
    connect( m_saveButton, SIGNAL( clicked() ), this, SLOT( slotSaveButtonClicked() ) );

    // buddies
    m_maximumLabel->setBuddy( m_maxSpin );
    m_minimumLabel->setBuddy( m_minSpin );
    m_matchLabel->setBuddy( m_channelCheck );
    m_textLabel->setBuddy( m_textEdit );
    m_chanlistView->header()->setLabel( 0, tr( "Channel" ) );
    m_chanlistView->header()->setLabel( 1, tr( "Users" ) );
    m_chanlistView->header()->setLabel( 2, tr( "Topic" ) );
    m_refreshButton->setText( tr( "&Refresh" ) );
    m_refreshButton->setAccel( QKeySequence( tr( "Alt+R" ) ) );
    m_saveButton->setText( tr( "&Save..." ) );
    m_saveButton->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    m_joinButton->setText( tr( "&Join" ) );
    m_joinButton->setAccel( QKeySequence( tr( "Alt+J" ) ) );
    m_limitGroupBox->setTitle( tr( "Show channels that have:" ) );
    m_maximumLabel->setText( tr( "Ma&ximum:" ) );
    m_minSpin->setPrefix( QString::null );
    m_minSpin->setSuffix( tr( " users" ) );
    m_maxSpin->setPrefix( QString::null );
    m_maxSpin->setSuffix( tr( " users" ) );
    m_minimumLabel->setText( tr( "&Minimum:" ) );
    m_matchLabel->setText( tr( "&Match:" ) );
    m_channelCheck->setText( tr( "&Channel" ) );
    m_channelCheck->setAccel( QKeySequence( tr( "Alt+C" ) ) );
    m_textLabel->setText( tr( "&The text:" ) );
    m_topicCheck->setText( tr( "&Topic" ) );
    m_topicCheck->setAccel( QKeySequence( tr( "Alt+T" ) ) );
    m_applyButton->setText( tr( "&Apply" ) );
    m_applyButton->setAccel( QKeySequence( tr( "Alt+A" ) ) );

m_chanlistView->setFullWidth(true);
		    m_chanlistView->setAllColumnsShowFocus(true);


 //m_chanlistView->setVScrollBarMode(QScrollView::AlwaysOn);
}

/*
 *  Destroys the object and frees any allocated resources
 */
XChatChanlistView::~XChatChanlistView()
{
    // no need to delete child widgets, Qt does it all for us
}

void XChatChanlistView::enableItems(bool yes){
	if (yes)
	{
		setEnabled(true);
		m_chanlistView->scrollBy(0,1);
		//verticalScrollBar ()
	}
	else{
	setEnabled(false);
	}
	
}

void XChatChanlistView::showSelf()
{
	m_window->tabWidget()->showPage(this);
}

void XChatChanlistView::slotApplyButtonClicked()
{
}

 void XChatChanlistView::slotJoinButtonClicked(){

}

 void XChatChanlistView::slotRefreshButtonClicked(){
	if (m_server->connected)
        {
		m_chanlistView->clear();
		handle_command (m_server->server_session, "list", FALSE);
		enableItems(false);
	}
	else
                KMessageBox::error  	(   this, "Not connected to a server",
		"error");
}


 void XChatChanlistView::appendChannel(QString chan,QString users,QString topic)
{
	m_chanlistView->setUpdatesEnabled(false);
	XChatChanlistItem *i=new XChatChanlistItem(m_chanlistView, chan, users, topic);
	
	if (isMatch(chan, users, topic))
	{
		//m_chanlistView->insertItem(i);
		m_chanlistView->setUpdatesEnabled(true);
	//	m_chanlistView->update();
			
	}	
	else{
		m_chanlistView->takeItem(i);
		m_chanlistView->setUpdatesEnabled(true);
	}

}

bool XChatChanlistView::isMatch(QString chan,QString users,QString topic)
{
	return true;
}



 void XChatChanlistView::slotSaveButtonClicked()
{

}




XChatChanlistItem::XChatChanlistItem(QListView * list,  QString chan, QString users, QString topic):
KListViewItem(list, chan, users, topic)
{

}

XChatChanlistItem::~XChatChanlistItem()
{
}

int XChatChanlistItem::compare ( QListViewItem * i, int col, bool ascending ) const
{
	if (col!=1)
	{
		return KListViewItem::compare(i,col,ascending);
		
	}
	{
		int a=key( col, ascending ).toInt();
		int b=i->key( col, ascending ).toInt();
		if (a==b)
			return 0;
		if (a<b)
			return -1;
		return 1;
	}
}






XChatBanListView::XChatBanListView(QWidget * parent,XChatMainWindow * w, const char *name, WFlags fl)
:QWidget(parent, name, fl), XChatContainerView(w)
{
	m_window=w;
    if(!name)
	setName("XChatBanlistView");
    XChatBanlistViewLayout = new QGridLayout(this, 1, 1, 12, 6, "XChatBanlistViewLayout");

    m_clearButton = new QPushButton(this, "m_clearButton");

    XChatBanlistViewLayout->addWidget(m_clearButton, 2, 1);
    QSpacerItem *spacer = new QSpacerItem(41, 280, QSizePolicy::Minimum, QSizePolicy::Expanding);

    XChatBanlistViewLayout->addItem(spacer, 4, 1);

    m_banlistView = new KListView(this, "m_banlistView");
    m_banlistView->addColumn(tr("Mask"));
    m_banlistView->addColumn(tr("From"));
    m_banlistView->addColumn(tr("Date"));
    XChatBanlistViewLayout->addMultiCellWidget(m_banlistView, 0, 4, 0, 0);

    m_unbanButton = new QPushButton(this, "m_unbanButton");
    XChatBanlistViewLayout->addWidget(m_unbanButton, 0, 1);

    m_refreshButton = new QPushButton(this, "m_refreshButton");
    XChatBanlistViewLayout->addWidget(m_refreshButton, 3, 1);

    m_cropButton = new QPushButton(this, "m_cropButton");
    XChatBanlistViewLayout->addWidget(m_cropButton, 1, 1);

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

XChatBanListView::~XChatBanListView()
{
}

void XChatBanListView::showSelf()
{
	        m_window->tabWidget()->showPage(this);
}

void XChatBanListView::slotUnbanButtonClicked()
{

}

void XChatBanListView::slotCropButtonClicked()
{

}

void XChatBanListView::slotClearButtonClicked()
{

}

void XChatBanListView::slotRefreshButtonClicked()
{

}

XChatChatListView::XChatChatListView(QWidget * parent, XChatMainWindow * w,const char *name, WFlags fl)
:QWidget(parent, name, fl), XChatContainerView(w)
{
	m_window=w;
    if(!name)
	setName("XChatChatListView");
    XChatChatListViewLayout = new QGridLayout(this, 1, 1, 12, 6, "XChatChatListViewLayout");

    m_cancelButton = new QPushButton(this, "m_cancelButton");
    XChatChatListViewLayout->addWidget(m_cancelButton, 1, 1);

    m_chatListView = new KListView(this, "m_chatListView");
    m_chatListView->addColumn(tr("Status"));
    m_chatListView->addColumn(tr("User"));
    m_chatListView->addColumn(tr("Time"));

    XChatChatListViewLayout->addMultiCellWidget(m_chatListView, 0, 2, 0, 0);
    QSpacerItem *spacer = new QSpacerItem(31, 221, QSizePolicy::Minimum, QSizePolicy::Expanding);

    XChatChatListViewLayout->addItem(spacer, 2, 1);

    m_acceptButton = new QPushButton(this, "m_acceptButton");
    XChatChatListViewLayout->addWidget(m_acceptButton, 0, 1);

    setCaption(tr("Chats"));
    m_cancelButton->setText(tr("&Cancel"));
    m_cancelButton->setAccel(QKeySequence(tr("Alt+C")));
    m_chatListView->header()->setLabel(0, tr("Status"));
    m_chatListView->header()->setLabel(1, tr("User"));
    m_chatListView->header()->setLabel(2, tr("Time"));
    m_acceptButton->setText(tr("&Accept"));
    m_acceptButton->setAccel(QKeySequence(tr("Alt+A")));
}

XChatChatListView::~XChatChatListView()
{
}

void XChatChatListView::showSelf()
{
	        m_window->tabWidget()->showPage(this);
}

XChatNotifyListView::XChatNotifyListView(QWidget * parent,XChatMainWindow * w, const char *name, WFlags fl)
:QWidget(parent, name, fl), XChatContainerView(w)
{
	m_window=w;
    if(!name)
	setName("XChatNotifyView");
    XChatNotifyViewLayout = new QGridLayout(this, 1, 1, 12, 6, "XChatNotifyViewLayout");

    m_notifyView = new KListView(this, "m_notifyView");
    m_notifyView->addColumn(tr("User"));
    m_notifyView->addColumn(tr("Status"));
    m_notifyView->addColumn(tr("Server"));
    m_notifyView->addColumn(tr("Last Seen"));
    XChatNotifyViewLayout->addMultiCellWidget(m_notifyView, 0, 2, 0, 0);

    m_addButton = new QPushButton(this, "m_addButton");
    XChatNotifyViewLayout->addWidget(m_addButton, 0, 1);

    m_removeButton = new QPushButton(this, "m_removeButton");
    XChatNotifyViewLayout->addWidget(m_removeButton, 1, 1);
    QSpacerItem *spacer = new QSpacerItem(30, 120, QSizePolicy::Minimum, QSizePolicy::Expanding);

    XChatNotifyViewLayout->addItem(spacer, 2, 1);
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

XChatNotifyListView::~XChatNotifyListView()
{
}

void XChatNotifyListView::showSelf()
{
	        m_window->tabWidget()->showPage(this);
}

void XChatNotifyListView::slotAddButtonClicked()
{
    qWarning("XChatNotifyView::slotAddButtonClicked(): Not implemented yet");
}

void XChatNotifyListView::slotRemoveButtonClicked()
{
    qWarning("XChatNotifyView::slotRemoveButtonClicked(): Not implemented yet");
}

XChatRawlogView::XChatRawlogView(QWidget * parent, XChatMainWindow * w, server * s, const char *name, WFlags fl)
:QWidget(parent, name, fl), XChatContainerView(w)
{
    if(!name)
	setName("XChatRawlogView");

    //setWindow(w);
    setServer(s);
    XChatRawlogViewLayout = new QGridLayout(this, 1, 1, 12, 6, "XChatRawlogViewLayout");

    m_clearButton = new QPushButton(this, "m_clearButton");
    XChatRawlogViewLayout->addWidget(m_clearButton, 1, 1);

    m_rawlogView = new TextView(this, "m_rawlogView");
    m_rawlogView->setSizePolicy(QSizePolicy((QSizePolicy::SizeType) 7, (QSizePolicy::SizeType) 7, 0, 0, m_rawlogView->sizePolicy().hasHeightForWidth()));
    XChatRawlogViewLayout->addMultiCellWidget(m_rawlogView, 0, 2, 0, 0);

    m_saveButton = new QPushButton(this, "m_saveButton");
    XChatRawlogViewLayout->addWidget(m_saveButton, 0, 1);
    QSpacerItem *spacer = new QSpacerItem(50, 254, QSizePolicy::Minimum, QSizePolicy::Expanding);

    XChatRawlogViewLayout->addItem(spacer, 2, 1);

    setCaption(tr("Raw Log"));
    m_clearButton->setText(tr("&Clear"));
    m_clearButton->setAccel(QKeySequence(tr("Alt+C")));
    m_saveButton->setText(tr("&Save..."));
    m_saveButton->setAccel(QKeySequence(tr("Alt+S")));

    connect(m_clearButton, SIGNAL(clicked()), this, SLOT(slotClearButtonClicked()));
    connect(m_saveButton, SIGNAL(clicked()), this, SLOT(slotSaveButtonClicked()));
}

XChatRawlogView::~XChatRawlogView()
{
}

void XChatRawlogView::slotClearButtonClicked()
{
    qWarning("XChatRawlogView::slotClearButtonClicked(): Not implemented yet");
}

void XChatRawlogView::slotSaveButtonClicked()
{
    qWarning("XChatRawlogView::slotSaveButtonClicked(): Not implemented yet");
}

void XChatRawlogView::appendText(QString s)
{
    m_rawlogView->appendText(s, true);
}

XChatURLGrabberView::XChatURLGrabberView(QWidget * parent, XChatMainWindow * w,const char *name, WFlags fl)
:QWidget(parent, name, fl), XChatContainerView(w)
{
	m_window=w;
    if(!name)
	setName("XChatURLView");
    XChatURLViewLayout = new QGridLayout(this, 1, 1, 12, 6, "XChatURLViewLayout");
    m_saveButton = new QPushButton(this, "m_saveButton");
    XChatURLViewLayout->addWidget(m_saveButton, 0, 1);

    m_clearButton = new QPushButton(this, "m_clearButton");
    XChatURLViewLayout->addWidget(m_clearButton, 1, 1);
    QSpacerItem *spacer = new QSpacerItem(20, 220, QSizePolicy::Minimum, QSizePolicy::Expanding);

    XChatURLViewLayout->addItem(spacer, 2, 1);

    m_urlView = new KListView(this, "m_urlView");
    m_urlView->addColumn(tr("URL"));

    XChatURLViewLayout->addMultiCellWidget(m_urlView, 0, 2, 0, 0);
    setCaption(tr("URL Grabber"));
    m_saveButton->setText(tr("&Save..."));
    m_saveButton->setAccel(QKeySequence(tr("Alt+S")));
    m_clearButton->setText(tr("&Clear"));
    m_clearButton->setAccel(QKeySequence(tr("Alt+C")));
    m_urlView->header()->setLabel(0, tr("URL"));

    connect(m_saveButton, SIGNAL(clicked()), this, SLOT(slotSaveButtonClicked()));
    connect(m_clearButton, SIGNAL(clicked()), this, SLOT(slotClearButtonClicked()));
}

XChatURLGrabberView::~XChatURLGrabberView()
{
}

void XChatURLGrabberView::showSelf()
{
	        m_window->tabWidget()->showPage(this);
}

void XChatURLGrabberView::slotSaveButtonClicked()
{
    qWarning("XChatURLView::slotSaveButtonClicked(): Not implemented yet");
}

void XChatURLGrabberView::slotClearButtonClicked()
{
    qWarning("XChatURLView::slotClearButtonClicked(): Not implemented yet");
}

XChatXferView::XChatXferView(QWidget * parent,XChatMainWindow * w, const char *name, WFlags fl)
:QWidget(parent, name, fl), XChatContainerView(w)
{
	m_window=w;
    if(!name)
	setName("XChatFileXferView");
    XChatFileXferViewLayout = new QVBoxLayout(this, 1, 1, "XChatFileXferViewLayout");

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
    XChatFileXferViewLayout->addWidget(m_splitter);
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

XChatXferView::~XChatXferView()
{
}

void XChatXferView::showSelf()
{
	        m_window->tabWidget()->showPage(this);
}

void XChatXferView::slotStartButtonClicked()
{

}

void XChatXferView::slotDlCancelButtonClicked()
{

}

void XChatXferView::slotResumeButtonClicked()
{

}

void XChatXferView::slotDlInfoButtonClicked()
{

}

void XChatXferView::slotUlCancelButtonClicked()
{

}

void XChatXferView::slotUlInfoButtonClicked()
{

}

XChatEditListView::XChatEditListView(QWidget * parent, const char *filename, GSList * entries, const char *name)
:QWidget(parent, name)
{
    if(!name)
	setName("XChatRawlogView");
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

XChatEditListView::~XChatEditListView()
{
}

XChatUserOptionsView::XChatUserOptionsView(QWidget * parent, const char *name)
:QWidget(parent, name)
{
    if(!name)
	setName("XChatUserOptionsView");
    XChatUserOptionsViewLayout = new QGridLayout(this, 1, 1, 12, 6, "XChatUserOptionsViewLayout");

    nickEdit = new QLineEdit(this, "nickEdit");
    XChatUserOptionsViewLayout->addWidget(nickEdit, 0, 1);

    altLabel = new QLabel(this, "altLabel");
    XChatUserOptionsViewLayout->addWidget(altLabel, 1, 0);

    nickLabel = new QLabel(this, "nickLabel");
    XChatUserOptionsViewLayout->addWidget(nickLabel, 0, 0);

    secEdit = new QLineEdit(this, "secEdit");
    XChatUserOptionsViewLayout->addWidget(secEdit, 2, 1);

    secLabel = new QLabel(this, "secLabel");
    XChatUserOptionsViewLayout->addWidget(secLabel, 2, 0);

    altEdit = new QLineEdit(this, "altEdit");
    XChatUserOptionsViewLayout->addWidget(altEdit, 1, 1);

    realLabel = new QLabel(this, "realLabel");
    XChatUserOptionsViewLayout->addWidget(realLabel, 4, 0);

    userEdit = new QLineEdit(this, "userEdit");
    XChatUserOptionsViewLayout->addWidget(userEdit, 3, 1);

    userLabel = new QLabel(this, "userLabel");
    XChatUserOptionsViewLayout->addWidget(userLabel, 3, 0);

    realEdit = new QLineEdit(this, "realEdit");
    XChatUserOptionsViewLayout->addWidget(realEdit, 4, 1);

    QSpacerItem *spacer = new QSpacerItem(51, 133, QSizePolicy::Minimum, QSizePolicy::Expanding);

    XChatUserOptionsViewLayout->addItem(spacer, 5, 1);

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

XChatUserOptionsView::~XChatUserOptionsView()
{

}