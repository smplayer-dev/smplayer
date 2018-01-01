/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2018 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "actiontools.h"
#include "myaction.h"

void ActionTools::setActionToButton(MyButton * button, const QString & name, QList<QAction*> actions) {
	MyAction * a = static_cast<MyAction*>( findAction(name, actions) );
	if (a) button->setAction(a);
}

QAction * ActionTools::findAction(const QString & name, QList<QAction*> actions) {
	QAction * a = 0;
	for (int n=0; n < actions.count(); n++) {
		a = actions.at(n);
		/* qDebug("**** name: %s", a->objectName().toLatin1().constData()); */
		if (a->objectName() == name) return a;
	}
	return 0;
}
