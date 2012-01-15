/****************************************************************************
**
** Copyright (c) 2007 Trolltech ASA <info@trolltech.com>
**
** Use, modification and distribution is allowed without limitation,
** warranty, liability or support of any kind.
**
****************************************************************************/

#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>

class QToolButton;

class MyLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    MyLineEdit(QWidget *parent = 0);

	/* void setClearIcon(const QPixmap & pixmap); */

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void updateCloseButton(const QString &text);

private:
    QToolButton *clearButton;
	/* QIcon clear_icon; */
};

#endif // MYLINEEDIT_H

