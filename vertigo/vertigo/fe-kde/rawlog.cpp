
#include "rawlog.h"

#include "fe-kde.h"

#include <textwidget.h>
#include <kpushbutton.h>

RawlogView::RawlogView(QWidget * parent, server * s)
    :  RawlogViewBase(parent, "Rawlog")
{
    setServer(s);
    connect(m_clearButton, SIGNAL(clicked()), this, SLOT(slotClearButtonClicked()));
    connect(m_saveButton, SIGNAL(clicked()), this, SLOT(slotSaveButtonClicked()));
}

RawlogView::~RawlogView()
{
}

void RawlogView::slotClearButtonClicked()
{
    qWarning("RawlogView::slotClearButtonClicked(): Not implemented yet");
}

void RawlogView::slotSaveButtonClicked()
{
    qWarning("RawlogView::slotSaveButtonClicked(): Not implemented yet");
}

void RawlogView::appendText(QString s)
{
    m_rawlogView->appendText(s, true);
}

void RawlogView::closeView()
{
	ContainerView::closeView();
	getServer()->gui->rawlog = 0;
}

