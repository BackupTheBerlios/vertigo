#ifndef HW_H
#define HW_H

#include <qtextedit.h>
#include <qlineedit.h>
#include <klistview.h>

#include "../common/xchat.h"
#include "../common/userlist.h"

QString unescapeText(QString t);

class TopicEdit:public QTextEdit {
  Q_OBJECT public:
     TopicEdit(QWidget * parent, QString name);
    ~TopicEdit();

	void setTopicText(QString s);
	void focusInEvent ( QFocusEvent * );
	void focusOutEvent ( QFocusEvent * );
QSize sizeHint() const;
QSize minimumSizeHint() const;
	void keyPressEvent( QKeyEvent *e );
	
	private:
	QString m_topicText;
};

class InputLineEdit:public QLineEdit {
  Q_OBJECT public:
     InputLineEdit(QWidget * parent, QString name);
    ~InputLineEdit();
    bool eventFilter(QObject * o, QEvent * e);
    
     signals:void tabPressed();
    void upPressed();
    void downPressed();

};



class UserlistItem:public KListViewItem {
  public:
    UserlistItem(KListView * list, QPixmap * pix, User * u);
    ~UserlistItem();

QPixmap *getPixmap();

    User *getUser() {
	return m_user;
    }

	void setUser(User *user);

int compare ( QListViewItem * i, int col, bool ascending ) const;
void paintCell ( QPainter * p, const QColorGroup & cg, int col, int width, int align );


  private:
    User * m_user;
	QPixmap *m_pixmap;
};

#endif
