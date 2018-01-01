/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2018 Ricardo Villalba <rvm@users.sourceforge.net>
    umplayer, Copyright (C) 2010 Ori Rejwan

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QAbstractButton>
#include "myicon.h"

class MyAction;
class MyButton : public QAbstractButton
{
Q_OBJECT
public:
    explicit MyButton(QWidget *parent = 0);
    void setState(bool on) {state = on; }
    void setMyIcon(MyIcon p_icon) { icon = p_icon; }
    MyIcon myIcon() { return icon;}
    void setAction(MyAction* pAction);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    MyIcon icon;
    bool mouseHover;
    bool state;
    MyAction* action;

protected:
    virtual void paintEvent(QPaintEvent *e);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);


signals:

public slots:
    void toggleImage();

};

#endif // MYBUTTON_H
