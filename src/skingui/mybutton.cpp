/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2017 Ricardo Villalba <rvm@users.sourceforge.net>
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

#include "mybutton.h"
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include "myaction.h"

MyButton::MyButton(QWidget *parent) :
    QAbstractButton(parent), mouseHover(false), state(false), action(0)
{

}


void MyButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPixmap pix;
    if(isEnabled() && ( isDown() || isChecked()))
    {
        pix = icon.pixmap(MyIcon::MouseDown, state ? MyIcon::On : MyIcon::Off);
    }
    else if(isEnabled() && mouseHover)
    {
        pix = icon.pixmap(MyIcon::MouseOver, state ? MyIcon::On : MyIcon::Off);
    }
    else if(isEnabled())
    {
        pix = icon.pixmap(MyIcon::Normal, state ? MyIcon::On : MyIcon::Off);
    }
    else
    {
        pix = icon.pixmap(MyIcon::Disabled, state ? MyIcon::On : MyIcon::Off);
    }
    QPainter p(this);    
    if(!pix.isNull())
        p.drawPixmap(0,0,pix);    
}


void MyButton::enterEvent(QEvent *)
{
    mouseHover = true;
    update();
}

void MyButton::leaveEvent(QEvent *)
{
    mouseHover = false;
    update();
}

void MyButton::setAction(MyAction *pAction)
{
    action = pAction;
    if(action)
    {
        setEnabled(action->isEnabled());
        action->installEventFilter(this);
        connect(this, SIGNAL(clicked()), action, SLOT(trigger()));
        if( action->isCheckable())
        {
            toggleImage();
            connect(action, SIGNAL(toggled(bool)), this, SLOT(toggleImage()));
        }
    }
}

bool MyButton::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == action)
    {
        if(event->type() == QEvent::ActionChanged)
        {
            setEnabled(action->isEnabled());
        }
    }
    return false;
}

void MyButton::toggleImage()
{
    if(isCheckable()) setChecked(action->isChecked());
    else setState(action->isChecked());
    update();
}

#include "moc_mybutton.cpp"
