#ifndef RL_H
#define RL_H

#include "rawlogbase.h"

class QGridLayout;

class RawlogView :  public RawlogViewBase
{
    Q_OBJECT

public:
    RawlogView( QWidget* parent = 0, server * s=0);
    ~RawlogView();

    void appendText(QString s);

public slots:
    void slotClearButtonClicked();
    void slotSaveButtonClicked();
    void closeView();

protected:
    QGridLayout* RawlogViewLayout;
};

#endif

