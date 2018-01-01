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

#ifndef MYICON_H
#define MYICON_H

#include <QObject>
#include <QPixmap>

class MyIcon
{
public:
    enum Mode
    {
        Normal,
        MouseOver,
        MouseDown,
        Disabled
    };

    enum State
    {
        Off,
        On
    };

    explicit MyIcon();
    QSize size(Mode mode, State state = Off );
    void setPixmap(QPixmap pix, Mode mode, State state = Off);
    QPixmap pixmap(Mode mode, State state =Off) const;

private:
    QPixmap normalPixOff;
    QPixmap mouseOverPixOff;
    QPixmap mouseDownPixOff;
    QPixmap disabledPixOff;
    QPixmap normalPixOn;
    QPixmap mouseOverPixOn;
    QPixmap mouseDownPixOn;
    QPixmap disabledPixOn;
    QPixmap actualPixmap(Mode mode, State state =Off) const;

signals:

public slots:

};

#endif // MYICON_H
