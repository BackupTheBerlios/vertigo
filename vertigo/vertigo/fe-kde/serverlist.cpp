#include "serverlist.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qframe.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <qheader.h>
#include <klistview.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qpopupmenu.h>
#include <kdialogbase.h>
#include <qimage.h>
#include <qpixmap.h>
#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kaction.h>
#include <kmenubar.h>

#include <glib/gslist.h>

#include "app.h"

#include "../common/xchat.h"
#include "../common/xchatc.h"
#include "../common/servlist.h"
#include "../common/cfgfiles.h"

XChatServerlist::XChatServerlist(QWidget * parent, const char *name)
:KMainWindow(parent, name)
{
    if(!name)
	setName("XChatServerlist");
    XChatServerlistLayout = new QVBoxLayout(this, 4, 3, "XChatServerlistLayout");

    m_mainSplitter = new QSplitter(this, "m_mainSplitter");
    m_mainSplitter->setOrientation(QSplitter::Horizontal);
    m_mainSplitter->setOpaqueResize(TRUE);

    m_networkFrame = new QFrame(m_mainSplitter, "m_networkFrame");
    m_networkFrame->setFrameShape(QFrame::StyledPanel);
    m_networkFrame->setFrameShadow(QFrame::Sunken);
    m_networkFrameLayout = new QVBoxLayout(m_networkFrame, 1, 1, "m_networkFrameLayout");

    networkLayout = new QHBoxLayout(0, 3, 4, "networkLayout");

    textLabel1 = new QLabel(m_networkFrame, "textLabel1");
    networkLayout->addWidget(textLabel1);
    QSpacerItem *spacer = new QSpacerItem(90, 20, QSizePolicy::Expanding,
	QSizePolicy::Minimum);

    networkLayout->addItem(spacer);

    m_networkButton = new QToolButton(m_networkFrame, "m_networkButton");
    networkLayout->addWidget(m_networkButton);
    m_networkFrameLayout->addLayout(networkLayout);

    m_networkButton->hide();

    m_networkListView = new KListView(m_networkFrame, "m_networkListView");
    m_networkListView->addColumn(tr("Column 1"));
    //m_networkListView->setFrameShape( KListView::NoFrame );

    m_networkListView->setItemsRenameable(true);
    m_networkListView->setRenameable(0, true);
    m_networkListView->setFullWidth(true);
    m_networkListView->setSorting(-1, true);
    m_networkListView->setDragEnabled(true);
    m_networkListView->setAcceptDrops(true);

    m_networkFrameLayout->addWidget(m_networkListView);

    m_settingsGroupBox = new QGroupBox(m_mainSplitter, "m_settingsGroupBox");
    m_settingsGroupBox->hide();
    m_settingsGroupBox->setMinimumSize(QSize(80, 1));
    m_settingsGroupBox->setColumnLayout(0, Qt::Vertical);
    m_settingsGroupBox->layout()->setSpacing(4);
    m_settingsGroupBox->layout()->setMargin(3);
    m_settingsGroupBoxLayout = new QVBoxLayout(m_settingsGroupBox->layout());
    m_settingsGroupBoxLayout->setAlignment(Qt::AlignTop);

    layout23 = new QHBoxLayout(0, 0, 6, "layout23");

    layout22 = new QGridLayout(0, 1, 1, 0, 6, "layout22");

    textLabel2_2_2 = new QLabel(m_settingsGroupBox, "textLabel2_2_2");
    textLabel2_2_2->setMinimumSize(QSize(20, 0));
    textLabel2_2_2->setAlignment(int (QLabel::AlignVCenter | QLabel::AlignRight));

    layout22->addWidget(textLabel2_2_2, 4, 0);

    m_userLineEdit = new QLineEdit(m_settingsGroupBox, "m_userLineEdit");
    m_userLineEdit->setMinimumSize(QSize(20, 0));

    layout22->addWidget(m_userLineEdit, 1, 1);

    m_chansLineEdit = new QLineEdit(m_settingsGroupBox, "m_chansLineEdit");
    m_chansLineEdit->setMinimumSize(QSize(20, 0));

    layout22->addWidget(m_chansLineEdit, 4, 1);

    m_realLineEdit = new QLineEdit(m_settingsGroupBox, "m_realLineEdit");
    m_realLineEdit->setMinimumSize(QSize(20, 0));

    layout22->addWidget(m_realLineEdit, 2, 1);

    textLabel2 = new QLabel(m_settingsGroupBox, "textLabel2");
    textLabel2->setMinimumSize(QSize(20, 0));
    textLabel2->setAlignment(int (QLabel::AlignVCenter | QLabel::AlignRight));

    layout22->addWidget(textLabel2, 0, 0);
    QSpacerItem *spacer_2 = new QSpacerItem(51, 191, QSizePolicy::Minimum,
	QSizePolicy::Expanding);

    layout22->addItem(spacer_2, 7, 1);
    QSpacerItem *spacer_3 = new QSpacerItem(51, 191, QSizePolicy::Minimum,
	QSizePolicy::Expanding);

    layout22->addItem(spacer_3, 7, 0);

    m_pwdLineEdit = new QLineEdit(m_settingsGroupBox, "m_pwdLineEdit");
    m_pwdLineEdit->setMinimumSize(QSize(20, 0));

    layout22->addWidget(m_pwdLineEdit, 3, 1);

    textLabel2_4 = new QLabel(m_settingsGroupBox, "textLabel2_4");
    textLabel2_4->setMinimumSize(QSize(20, 0));
    textLabel2_4->setAlignment(int (QLabel::AlignVCenter | QLabel::AlignRight));

    layout22->addWidget(textLabel2_4, 3, 0);

    textLabel2_3 = new QLabel(m_settingsGroupBox, "textLabel2_3");
    textLabel2_3->setMinimumSize(QSize(20, 0));
    textLabel2_3->setAlignment(int (QLabel::AlignVCenter | QLabel::AlignRight));

    layout22->addWidget(textLabel2_3, 2, 0);

    textLabel2_2 = new QLabel(m_settingsGroupBox, "textLabel2_2");
    textLabel2_2->setMinimumSize(QSize(20, 0));
    textLabel2_2->setAlignment(int (QLabel::AlignVCenter | QLabel::AlignRight));

    layout22->addWidget(textLabel2_2, 1, 0);

    textLabel2_3_2_3 = new QLabel(m_settingsGroupBox, "textLabel2_3_2_3");
    textLabel2_3_2_3->setMinimumSize(QSize(20, 0));
    textLabel2_3_2_3->setAlignment(int (QLabel::AlignVCenter | QLabel::AlignRight));

    layout22->addWidget(textLabel2_3_2_3, 6, 0);

    m_charComboBox = new QComboBox(FALSE, m_settingsGroupBox, "m_charComboBox");
    m_charComboBox->setSizePolicy(QSizePolicy((QSizePolicy::SizeType) 7, (QSizePolicy::SizeType) 0, 0, 0, m_charComboBox->sizePolicy().hasHeightForWidth()));
    m_charComboBox->setMinimumSize(QSize(20, 0));
    m_charComboBox->setEditable(FALSE);

    layout22->addWidget(m_charComboBox, 6, 1);

    m_cmdLineEdit = new QLineEdit(m_settingsGroupBox, "m_cmdLineEdit");
    m_cmdLineEdit->setMinimumSize(QSize(20, 0));

    layout22->addWidget(m_cmdLineEdit, 5, 1);

    textLabel2_3_2 = new QLabel(m_settingsGroupBox, "textLabel2_3_2");
    textLabel2_3_2->setMinimumSize(QSize(20, 0));

    layout22->addWidget(textLabel2_3_2, 5, 0);

    m_nickLineEdit = new QLineEdit(m_settingsGroupBox, "m_nickLineEdit");
    m_nickLineEdit->setSizePolicy(QSizePolicy((QSizePolicy::SizeType) 3, (QSizePolicy::SizeType) 0, 0, 0, m_nickLineEdit->sizePolicy().hasHeightForWidth()));
    m_nickLineEdit->setMinimumSize(QSize(20, 0));

    layout22->addWidget(m_nickLineEdit, 0, 1);
    layout23->addLayout(layout22);

    m_serverFrame = new QFrame(m_settingsGroupBox, "m_serverFrame");
    m_serverFrame->setMinimumSize(QSize(20, 0));
    m_serverFrame->setFrameShape(QFrame::StyledPanel);
    m_serverFrame->setFrameShadow(QFrame::Sunken);
    m_serverFrameLayout = new QVBoxLayout(m_serverFrame, 1, 1, "m_serverFrameLayout");

    layout5_2 = new QHBoxLayout(0, 3, 4, "layout5_2");

    textLabel1_2 = new QLabel(m_serverFrame, "textLabel1_2");
    textLabel1_2->setSizePolicy(QSizePolicy((QSizePolicy::SizeType) 5, (QSizePolicy::SizeType) 5, 0, 0, textLabel1_2->sizePolicy().hasHeightForWidth()));
    textLabel1_2->setMinimumSize(QSize(0, 0));
    layout5_2->addWidget(textLabel1_2);
    QSpacerItem *spacer_4 = new QSpacerItem(90, 20, QSizePolicy::Expanding,
	QSizePolicy::Minimum);

    layout5_2->addItem(spacer_4);

    m_serverAddButton = new QToolButton(m_serverFrame, "m_serverAddButton");
    m_serverAddButton->setSizePolicy(QSizePolicy((QSizePolicy::SizeType) 1, (QSizePolicy::SizeType) 1, 0, 0, m_serverAddButton->sizePolicy().hasHeightForWidth()));
    m_serverAddButton->setMinimumSize(QSize(0, 0));
    layout5_2->addWidget(m_serverAddButton);
    m_serverFrameLayout->addLayout(layout5_2);

    m_serverListView = new KListView(m_serverFrame, "m_serverListView");
    m_serverListView->addColumn(tr("Column 1"));
    m_serverListView->setSizePolicy(QSizePolicy((QSizePolicy::SizeType) 7, (QSizePolicy::SizeType) 7, 0, 0, m_serverListView->sizePolicy().hasHeightForWidth()));
    m_serverListView->setMinimumSize(QSize(20, 0));
    // m_serverListView->setFrameShape( KListView::NoFrame );

    m_serverListView->setItemsRenameable(true);
    m_serverListView->setRenameable(0, true);
    m_serverListView->setFullWidth(true);
    m_serverListView->setSorting(-1, false);
    m_serverListView->setDragEnabled(true);
    m_serverListView->setAcceptDrops(true);

    m_serverFrameLayout->addWidget(m_serverListView);
    layout23->addWidget(m_serverFrame);
    m_settingsGroupBoxLayout->addLayout(layout23);

    layout9 = new QGridLayout(0, 1, 1, 3, 4, "layout9");

    m_autoCheckbox = new QCheckBox(m_settingsGroupBox, "m_autoCheckbox");
    m_autoCheckbox->setMinimumSize(QSize(20, 0));

    layout9->addWidget(m_autoCheckbox, 0, 0);

    m_proxyCheckbox = new QCheckBox(m_settingsGroupBox, "proxyCheckBox");
    m_proxyCheckbox->setMinimumSize(QSize(20, 0));

    layout9->addWidget(m_proxyCheckbox, 1, 1);

    m_globalCheckbox = new QCheckBox(m_settingsGroupBox, "m_globalCheckbox");
    m_globalCheckbox->setMinimumSize(QSize(20, 0));

    layout9->addWidget(m_globalCheckbox, 0, 1);

    m_cycleCheckbox = new QCheckBox(m_settingsGroupBox, "m_cycleCheckbox");
    m_cycleCheckbox->setMinimumSize(QSize(20, 0));

    layout9->addWidget(m_cycleCheckbox, 1, 0);

    m_secureCheckbox = new QCheckBox(m_settingsGroupBox, "m_secureCheckbox");
    m_secureCheckbox->setMinimumSize(QSize(20, 0));

    layout9->addWidget(m_secureCheckbox, 0, 2);

    m_invalidCheckbox = new QCheckBox(m_settingsGroupBox, "m_invalidCheckbox");
    m_invalidCheckbox->setMinimumSize(QSize(20, 0));

    layout9->addWidget(m_invalidCheckbox, 1, 2);
    m_settingsGroupBoxLayout->addLayout(layout9);
    XChatServerlistLayout->addWidget(m_mainSplitter);

    layout24 = new QHBoxLayout(0, 0, 6, "layout24");

    m_editCheckbox = new QCheckBox(this, "m_editCheckbox");
    m_editCheckbox->setMinimumSize(QSize(0, 0));
    layout24->addWidget(m_editCheckbox);
    QSpacerItem *spacer_5 = new QSpacerItem(250, 20, QSizePolicy::Expanding,
	QSizePolicy::Minimum);

    layout24->addItem(spacer_5);

    m_newTabCheckbox = new QCheckBox(this, "m_newTabCheckBox");
    m_newTabCheckbox->setMinimumSize(QSize(0, 0));
    layout24->addWidget(m_newTabCheckbox);

    KIconLoader *loader = KGlobal::iconLoader();

    m_connectButton = new QPushButton(loader->loadIconSet("connect_creating", KIcon::Small), tr("&Connect"), this, "m_connectButton");
    m_connectButton->setDefault(true);
    layout24->addWidget(m_connectButton);
    m_userOptionsButton = new QPushButton(loader->loadIconSet("configure", KIcon::Small), tr("Use&r Info..."), this, "m_userOptionsButton");
    m_userOptionsButton->setAutoDefault(FALSE);
    layout24->addWidget(m_userOptionsButton);
    m_closeButton = new QPushButton(loader->loadIconSet("cancel", KIcon::Small), tr("C&lose"), this, "m_closeButton");
    layout24->addWidget(m_closeButton);
    XChatServerlistLayout->addLayout(layout24);
    languageChange();
    resize(QSize(704, 434).expandedTo(minimumSizeHint()));
    clearWState(WState_Polished);
    // signals and slots connections
    connect(m_networkButton, SIGNAL(clicked()), this, SLOT(slotNetworkButtonClicked()));
    connect(m_serverAddButton, SIGNAL(clicked()), this, SLOT(slotServerAddButtonClicked()));
    connect(m_nickLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(slotNickLineEditTextChanged(const QString &)));
    connect(m_userLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(slotUserLineEdittextChanged(const QString &)));
    connect(m_realLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(slotRealLineEditTextChanged(const QString &)));
    connect(m_pwdLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(slotPwdLineEditTextChanged(const QString &)));
    connect(m_chansLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(slotChansLineEditTextChanged(const QString &)));
    connect(m_cmdLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(slotCmdLineEditTextChanged(const QString &)));
    connect(m_charComboBox, SIGNAL(activated(const QString &)), this, SLOT(slotCharComboBoxActivated(const QString &)));

    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(slotCloseButtonClicked()));
    connect(m_userOptionsButton, SIGNAL(clicked()), this, SLOT(slotUserOptionsButtonClicked()));
    connect(m_connectButton, SIGNAL(clicked()), this, SLOT(slotConnectButtonClicked()));
    connect(m_newTabCheckbox, SIGNAL(toggled(bool)), this, SLOT(slotNewTabCheckboxToggled(bool)));
    connect(m_editCheckbox, SIGNAL(toggled(bool)), this, SLOT(slotEditCheckboxToggled(bool)));
    connect(m_networkListView, SIGNAL(contextMenuRequested(QListViewItem *, const QPoint &, int)), this, SLOT(slotNetworkListViewContextMenuRequested(QListViewItem *, const QPoint &, int)));
    connect(m_serverListView, SIGNAL(contextMenuRequested(QListViewItem *, const QPoint &, int)), this, SLOT(slotServerListViewContextMenuRequested(QListViewItem *, const QPoint &, int)));

    connect(m_invalidCheckbox, SIGNAL(toggled(bool)), this, SLOT(slotInvalidCheckboxToggled(bool)));
    connect(m_secureCheckbox, SIGNAL(toggled(bool)), this, SLOT(slotSecureCheckboxToggled(bool)));
    connect(m_proxyCheckbox, SIGNAL(toggled(bool)), this, SLOT(slotProxyCheckboxToggled(bool)));
    connect(m_globalCheckbox, SIGNAL(toggled(bool)), this, SLOT(slotGlobalCheckboxToggled(bool)));
    connect(m_autoCheckbox, SIGNAL(toggled(bool)), this, SLOT(slotAutoCheckboxToggled(bool)));
    connect(m_cycleCheckbox, SIGNAL(toggled(bool)), this, SLOT(slotCycleBoxToggled(bool)));
    connect(m_networkListView, SIGNAL(currentChanged(QListViewItem *)), this, SLOT(slotNetworkListViewCurrentChanged(QListViewItem *)));
    // buddies
    textLabel2_2_2->setBuddy(m_chansLineEdit);
    textLabel2->setBuddy(m_nickLineEdit);
    textLabel2_4->setBuddy(m_pwdLineEdit);
    textLabel2_3->setBuddy(m_realLineEdit);
    textLabel2_2->setBuddy(m_userLineEdit);
    textLabel2_3_2_3->setBuddy(m_charComboBox);
    textLabel2_3_2->setBuddy(m_connectButton);

    m_optionsDialog = 0;

    new KAction(i18n("Add New Server..."), loader->loadIconSet("filenew", KIcon::Small), "" /*, CTRL + Key_N */ , this, SLOT(slotServerList()), actionCollection(), "add_network");
    new KAction(i18n("Rename Network..."), loader->loadIconSet("fill", KIcon::Small), "" /*, CTRL + Key_R */ , this, SLOT(slotServerList()), actionCollection(), "rename_network");
    new KAction(i18n("Move Network Up"), loader->loadIconSet("1uparrow", KIcon::Small), "" /*, CTRL + Key_U */ , this, SLOT(slotServerList()), actionCollection(), "moveup_network");
    new KAction(i18n("Move Network Down"), loader->loadIconSet("1downarrow", KIcon::Small), "" /*,CTRL + Key_D */ , this, SLOT(slotServerList()), actionCollection(), "movedown_network");
    new KAction(i18n("Remove Network"), loader->loadIconSet("remove", KIcon::Small), "" /*,CTRL + Key_X */ , this, SLOT(slotServerList()), actionCollection(), "remove_network");

    createGUI("serverlist.rc");

    menuBar()->hide();

    m_mainSplitter->setCollapsible(m_networkFrame, false);
    m_mainSplitter->setCollapsible(m_settingsGroupBox, true);

    GSList *netlist = network_list;
    int i;
    ircnet *net;

    if(!netlist) {
	net = servlist_net_add("New Network", "");
	servlist_server_add(net, "newserver/6667");
	netlist = network_list;
    }

    i = 0;
    XChatServerlistNetworkItem *v;
    XChatServerlistNetworkItem *lastItem = 0;

    while(netlist) {
	net = (ircnet *) netlist->data;
	v = new XChatServerlistNetworkItem(m_networkListView, lastItem, net);
	lastItem = v;
	if(i == prefs.slist_select) {
	    m_networkListView->setCurrentItem(v);
	    m_networkListView->ensureItemVisible(v);
	}
	i++;
	netlist = netlist->next;
    }

    if(prefs.slist_edit)
	m_editCheckbox->setChecked(true);

    m_session = 0;
}

/*
 *  Destroys the object and frees any allocated resources
 */
XChatServerlist::~XChatServerlist()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void XChatServerlist::languageChange()
{
    setCaption(tr("Server List"));
    textLabel1->setText(tr("Network"));
    m_networkButton->setText(tr("&New"));
    m_networkButton->setFocusPolicy(StrongFocus);
    m_networkListView->header()->hide();
    m_networkListView->clear();
    m_settingsGroupBox->setTitle("Settings");
    textLabel2_2_2->setText(tr("&Join Channels:"));
    textLabel2->setText(tr("&Nickname:"));
    textLabel2_4->setText(tr("&Server Password:"));
    textLabel2_3->setText(tr("&Real Name:"));
    textLabel2_2->setText(tr("&Username:"));
    textLabel2_3_2_3->setText(tr("Character &Set:"));
    textLabel2_3_2->setText(tr("C&onnect Command:"));
    m_nickLineEdit->setText(QString::null);
    textLabel1_2->setText(tr("Servers"));
    m_serverAddButton->setText(tr("&New"));
    m_serverAddButton->setFocusPolicy(StrongFocus);
    m_serverListView->header()->hide();
    m_serverListView->clear();
    m_autoCheckbox->setText(tr("Au&to connect"));
    m_autoCheckbox->setAccel(QKeySequence(tr("Alt+T")));
    m_proxyCheckbox->setText(tr("Use &proxy server"));
    m_proxyCheckbox->setAccel(QKeySequence(tr("Alt+P")));
    m_globalCheckbox->setText(tr("Use &global user info"));
    m_globalCheckbox->setAccel(QKeySequence(tr("Alt+G")));
    m_cycleCheckbox->setText(tr("C&ycle until connected"));
    m_cycleCheckbox->setAccel(QKeySequence(tr("Alt+Y")));
    m_secureCheckbox->setText(tr("Use &secure connection"));
    m_secureCheckbox->setAccel(QKeySequence(tr("Alt+S")));
    m_invalidCheckbox->setText(tr("Accept &Invalid Certificate"));
    m_invalidCheckbox->setAccel(QKeySequence(tr("Alt+I")));
    m_editCheckbox->setText(tr("&Edit Mode"));
    m_editCheckbox->setAccel(QKeySequence(tr("Alt+E")));
    m_newTabCheckbox->setText(tr("Open In New &Tab"));
    m_newTabCheckbox->setAccel(QKeySequence(tr("Alt+T")));
    // m_connectButton->setText(tr("&Connect"));
    // m_connectButton->setAccel(QKeySequence(tr("Alt+C")));
    //m_userOptionsButton->setText(tr("Use&r Options"));
    //m_userOptionsButton->setAccel(QKeySequence(tr("Alt+R")));
    m_closeButton->setText(tr("C&lose"));
    //m_closeButton->setAccel(QKeySequence(tr("Alt+L")));
}

void XChatServerlist::closeEvent(QCloseEvent * e)
{
    servlist_save();
    m_closeButton->animateClick();
//              xchatapp->flushX();

    xchatapp->removeServerlistWindow();
    //hide();
    // xchatapp->flushX();
    m_closeButton->animateClick();
    QTimer::singleShot(200, this, SLOT(slotDelayedDestruct()));

    //   KMainWindow::closeEvent(e);
}

void XChatServerlist::slotDelayedDestruct()
{
    delete this;
}

void XChatServerlist::setSession(session * sess)
{
    m_session = sess;
}

void XChatServerlist::slotNetworkButtonClicked()
{
    ircnet *net = servlist_net_add(_("New Network"), "");
    XChatServerlistNetworkItem *v = new XChatServerlistNetworkItem(m_networkListView, (XChatServerlistNetworkItem *) m_networkListView->lastItem(),
	net);

    //m_serverListView->setSelected(m_serverListView->selectedItem (), false);
    //m_networkListView->clearSelection();
    m_networkListView->setSelected(v, true);
    m_networkListView->ensureItemVisible(v);
}

void XChatServerlist::slotServerAddButtonClicked()
{

}

void XChatServerlist::slotNickLineEditTextChanged(const QString &)
{

}

void XChatServerlist::slotUserLineEdittextChanged(const QString &)
{

}

void XChatServerlist::slotRealLineEditTextChanged(const QString &)
{

}

void XChatServerlist::slotPwdLineEditTextChanged(const QString &)
{

}

void XChatServerlist::slotChansLineEditTextChanged(const QString &)
{

}

void XChatServerlist::slotCmdLineEditTextChanged(const QString &)
{

}

void XChatServerlist::slotCharComboBoxActivated(const QString &)
{

}

void XChatServerlist::slotCloseButtonClicked()
{
    close();
}

void XChatServerlist::slotUserOptionsButtonClicked()
{
    if(!m_optionsDialog)
	m_optionsDialog = new XChatUserOptionsDialog(this);
    m_optionsDialog->show();
}

void XChatServerlist::slotConnectButtonClicked()
{
    XChatServerlistNetworkItem *ni = (XChatServerlistNetworkItem *) m_networkListView->currentItem();
    ircnet *i = ni->getNetwork();

    kdDebug() << "connecting to: " << i->name << endl;
    if(m_newTabCheckbox->isChecked())	//|| !is_session(m_session))
	servlist_connect(NULL, i);
    else {
	kdDebug() << "connecting in new window:" << i->name << endl;
	int old = prefs.tabchannels;

	prefs.tabchannels = 0;
	servlist_connect(m_session, i);
	prefs.tabchannels = old;
    }

    servlist_save();
    close();
}

void XChatServerlist::slotNewTabCheckboxToggled(bool)
{

}

void XChatServerlist::slotEditCheckboxToggled(bool tog)
{
    kdDebug() << "editBoxToggled:" << tog << endl;
    if(tog) {
	m_settingsGroupBox->show();
	m_networkButton->show();
	m_mainSplitter->setSizes(QValueList < int >() << m_networkFrame->minimumWidth() << width() - m_networkFrame->minimumWidth());

	m_networkListView->rename(m_networkListView->currentItem(), 0);
    } else {
/*m_mainSplitter->setSizes(QValueList <
                                 int >() << width()  << 1);
m_networkFrame->show();*/
	m_settingsGroupBox->hide();
	m_networkButton->hide();
    }
}

void XChatServerlist::slotNetworkListViewContextMenuRequested(QListViewItem *, const QPoint & p, int)
{
    QPopupMenu *pop = (QPopupMenu *) factory()->container("network_popup", this);

    pop->popup(p);

}

void XChatServerlist::slotServerListViewContextMenuRequested(QListViewItem *, const QPoint &, int)
{

}

void XChatServerlist::slotInvalidCheckboxToggled(bool sel)
{
    kdDebug() << "slotInvalidCheckboxToggled toggled " << sel << endl;

    XChatServerlistNetworkItem *ni = (XChatServerlistNetworkItem *) m_networkListView->currentItem();

    ircnet *net = ni->getNetwork();

    kdDebug() << "flags= " << net->flags << endl;
    if(sel)
	net->flags |= (1 << 5);
    else
	net->flags &= ~(1 << 5);
    kdDebug() << "flags end= " << net->flags << endl;
}

void XChatServerlist::slotSecureCheckboxToggled(bool sel)
{
    kdDebug() << "slotSecureCheckboxToggled toggled " << sel << endl;

    XChatServerlistNetworkItem *ni = (XChatServerlistNetworkItem *) m_networkListView->currentItem();

    ircnet *net = ni->getNetwork();

    kdDebug() << "flags= " << net->flags << endl;
    if(sel)
	net->flags |= (1 << 2);
    else
	net->flags &= ~(1 << 2);
    kdDebug() << "flags end= " << net->flags << endl;
}

void XChatServerlist::slotProxyCheckboxToggled(bool sel)
{
    kdDebug() << "slotProxyCheckboxToggled toggled " << sel << endl;

    XChatServerlistNetworkItem *ni = (XChatServerlistNetworkItem *) m_networkListView->currentItem();

    ircnet *net = ni->getNetwork();

    kdDebug() << "flags= " << net->flags << endl;
    if(sel)
	net->flags |= (1 << 4);
    else
	net->flags &= ~(1 << 4);
    kdDebug() << "flags end= " << net->flags << endl;
}

void XChatServerlist::slotGlobalCheckboxToggled(bool sel)
{
    kdDebug() << "slotGlobalCheckboxToggled toggled" << sel << endl;
    if(sel) {
	m_nickLineEdit->setEnabled(false);
	m_userLineEdit->setEnabled(false);
	m_realLineEdit->setEnabled(false);
	textLabel2->setEnabled(false);
	textLabel2_3->setEnabled(false);
	textLabel2_2->setEnabled(false);
    } else {
	m_nickLineEdit->setEnabled(true);
	m_userLineEdit->setEnabled(true);
	m_realLineEdit->setEnabled(true);
	textLabel2->setEnabled(true);
	textLabel2_3->setEnabled(true);
	textLabel2_2->setEnabled(true);
    }

    XChatServerlistNetworkItem *ni = (XChatServerlistNetworkItem *) m_networkListView->currentItem();

    ircnet *net = ni->getNetwork();

    kdDebug() << "flags= " << net->flags << endl;
    if(sel)
	net->flags |= (1 << 1);
    else
	net->flags &= ~(1 << 1);
    kdDebug() << "flags end= " << net->flags << endl;
}

void XChatServerlist::slotAutoCheckboxToggled(bool sel)
{
    kdDebug() << "slotAutoCheckboxToggled toggled " << sel << endl;

    XChatServerlistNetworkItem *ni = (XChatServerlistNetworkItem *) m_networkListView->currentItem();

    ircnet *net = ni->getNetwork();

    kdDebug() << "flags= " << net->flags << endl;
    if(sel)
	net->flags |= (1 << 3);
    else
	net->flags &= ~(1 << 3);
    kdDebug() << "flags end= " << net->flags << endl;
}

void XChatServerlist::slotCycleBoxToggled(bool sel)
{
    kdDebug() << "slotCycleBoxToggled toggled " << sel << endl;

    XChatServerlistNetworkItem *ni = (XChatServerlistNetworkItem *) m_networkListView->currentItem();

    ircnet *net = ni->getNetwork();

    kdDebug() << "flags= " << net->flags << endl;
    if(sel)
	net->flags |= (1 << 0);
    else
	net->flags &= ~(1 << 0);
    kdDebug() << "flags end= " << net->flags << endl;
}

void XChatServerlist::slotNetworkListViewCurrentChanged(QListViewItem * it)
{
    m_serverListView->clear();
    XChatServerlistNetworkItem *ni = (XChatServerlistNetworkItem *) it;

    m_settingsGroupBox->setTitle(QString(i18n("Settings for %1")).arg(ni->text(0)));
    ircnet *net = ni->getNetwork();
    GSList *list = net->servlist;
    ircserver *serv;
    int i = 0;
    KListViewItem *v;
    KListViewItem *lastItem = 0;

    while(list) {
	serv = (ircserver *) list->data;
	v = new KListViewItem(m_serverListView, lastItem, serv->hostname);
	lastItem = v;
	m_userLineEdit->setText(net->user);
	m_chansLineEdit->setText(net->autojoin);
	m_realLineEdit->setText(net->real);
	m_pwdLineEdit->setText(net->pass);
	m_cmdLineEdit->setText(net->command);
	m_nickLineEdit->setText(net->nick);

	kdDebug() << "current flags===" << net->flags << endl;
	if(net->flags & FLAG_USE_GLOBAL) {
	    m_globalCheckbox->setChecked(true);
	} else
	    m_globalCheckbox->setChecked(false);
	if(net->flags & FLAG_CYCLE)
	    m_cycleCheckbox->setChecked(true);
	else
	    m_cycleCheckbox->setChecked(false);
	if(net->flags & FLAG_USE_SSL)
	    m_secureCheckbox->setChecked(true);
	else
	    m_secureCheckbox->setChecked(false);
	if(net->flags & FLAG_AUTO_CONNECT)
	    m_autoCheckbox->setChecked(true);
	else
	    m_autoCheckbox->setChecked(false);
	if(net->flags & FLAG_USE_PROXY)
	    m_proxyCheckbox->setChecked(true);
	else
	    m_proxyCheckbox->setChecked(false);
	if(net->flags & FLAG_ALLOW_INVALID)
	    m_invalidCheckbox->setChecked(true);
	else
	    m_invalidCheckbox->setChecked(false);
	if(net->selected == i) {
	    m_serverListView->setSelected(v, true);
	    m_serverListView->ensureItemVisible(v);
	}
	i++;
	list = list->next;
    }
    if(!m_serverListView->childCount()) {
	servlist_server_add(net, "newserver/6667");
	new KListViewItem(m_serverListView, 0, "newserver/6667");
    }

}

XChatServerlistNetworkItem::XChatServerlistNetworkItem(QListView * list, XChatServerlistNetworkItem * after, ircnet * net):
KListViewItem(list, after)
{
    m_network = net;
    setText(0, QString::fromLocal8Bit(net->name));
}

XChatServerlistNetworkItem::~XChatServerlistNetworkItem()
{
}

ircnet *XChatServerlistNetworkItem::getNetwork()
{
    return m_network;
}

XChatUserOptionsDialog::XChatUserOptionsDialog(QWidget * parent)
:  KDialogBase(Swallow, i18n("Configure User Info"), Help | Default | Ok | Apply | Cancel, Ok, parent, 0, false)
{
    m_userOptionsView = new XChatUserOptionsView(this);
    setMainWidget(m_userOptionsView);
}

XChatUserOptionsDialog::~XChatUserOptionsDialog()
{
}
