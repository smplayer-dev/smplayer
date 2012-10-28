/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>
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


#include "myicon.h"

MyIcon::MyIcon()
{
}


void MyIcon::setPixmap(QPixmap pix, Mode mode, State state )
{
    switch(mode)
    {
    case Normal:
        if(state == Off) normalPixOff = pix;
        else normalPixOn = pix; break;
    case MouseOver:
        if(state == Off) mouseOverPixOff = pix;
        else mouseOverPixOn = pix; break;
    case MouseDown:
        if(state == Off) mouseDownPixOff = pix;
        else mouseDownPixOn = pix; break;
    case Disabled:
        if(state == Off) disabledPixOff = pix;
        else disabledPixOn = pix;break;
    }
}

QSize MyIcon::size(Mode mode, State state)
{
    return pixmap(mode, state).size();
}

QPixmap MyIcon::pixmap(Mode mode, State state) const
{
    QPixmap p = actualPixmap(mode, state);
    if(p.isNull())
    {
        if(state == Off) return normalPixOff;
        else return normalPixOn;
    }
    else return p;
}

QPixmap MyIcon::actualPixmap(Mode mode, State state ) const
{
    switch(mode)
    {
    case Normal:
        if(state == Off) return normalPixOff;
        else return normalPixOn;
    case MouseOver:
        if(state == Off) return mouseOverPixOff;
        else return mouseOverPixOn;
    case MouseDown:
        if(state == Off) return mouseDownPixOff;
        else return mouseDownPixOn;
    case Disabled:
        if(state == Off) return disabledPixOff;
        else return disabledPixOn;
    }
    return QPixmap();
}
