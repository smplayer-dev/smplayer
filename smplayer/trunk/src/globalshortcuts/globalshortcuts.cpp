/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2017 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "globalshortcuts.h"
#include <QAbstractEventDispatcher>
#include <QKeySequence>
#include <QKeyEvent>
#include <QEvent>
#include <QApplication>
#include <QWidget>
#include <QAction>
#include <QDebug>


GlobalShortcuts::GlobalShortcuts(QObject* parent)
	: QObject(parent)
	, enabled(false)
{
	qDebug("GlobalShortcuts::GlobalShortcuts");
	QAbstractEventDispatcher::instance()->installNativeEventFilter(this);

	grabbed_keys = VolumeMute | VolumeDown | VolumeUp |
                   MediaPlay | MediaStop | MediaPrevious |
                   MediaNext | MediaPause | MediaRecord;

	createKeysList();
	//setEnabled(true);
}

GlobalShortcuts::~GlobalShortcuts() {
	if (enabled) setEnabled(false);
}

void GlobalShortcuts::setEnabled(bool b) {
	qDebug() << "GlobalShortcuts::setEnabled:" << b;

	if (b == enabled) return;

	if (b) registerAll(); else unregisterAll();
	enabled = b;
}

void GlobalShortcuts::registerAll() {
	QMapIterator<quint32, Qt::Key> i(key_list);
	while (i.hasNext()) {
		i.next();
		registerShortcut(i.key(), 0);
	}
}

void GlobalShortcuts::unregisterAll() {
	QMapIterator<quint32, Qt::Key> i(key_list);
	while (i.hasNext()) {
		i.next();
		unregisterShortcut(i.key(), 0);
	}
}

void GlobalShortcuts::activateShortcut(Qt::Key key) {
#ifdef Q_OS_LINUX
	//if (QApplication::activeWindow()) return;
#endif

	QKeySequence ks(key);

	qDebug() << "GlobalShortcuts::activateShortcut:" << key << "shortcut:" << ks;

#if 0
	Qt::KeyboardModifiers modifier = Qt::NoModifier;
	QString name = ks.toString();
	QKeyEvent key_event(QEvent::KeyPress, key, modifier, name);

	qDebug() << "GlobalShortcuts::activateShortcut: name:" << name;

	if (parent()) QCoreApplication::sendEvent(parent(), &key_event);

#else
	// Search actions with the shortcut
	if (isEnabled() && parent()) {
		QWidget * w = qobject_cast<QWidget *>(parent());
		if (!w) {
			qWarning("GlobalShortcuts::activateShortcut: parent is not a widget");
			return;
		}
		QList<QAction *> actions = w->actions();
		foreach(QAction * action, actions) {
			QList<QKeySequence> shortcuts = action->shortcuts();
			foreach(QKeySequence s, shortcuts) {
				bool match = (s == ks);
				//qDebug() << "GlobalShortcuts::activateShortcut: action:" << action << "shortcut:" << s << "match:" << match;
				if (match) {
					qDebug() << "GlobalShortcuts::activateShortcut: action found:" << action->objectName() << "enabled:" << action->isEnabled();
				}
				if (match && action->isEnabled()) {
					// SkinGui changes the play/pause action to checkable, which doesn't work if called toggle
					if (action->isCheckable() && action->objectName() != "play_or_pause") {
						action->toggle(); 
					} else { 
						action->trigger();
					}
					return;
				}
			}
		}
	}
#endif
}

#include "moc_globalshortcuts.cpp"

#ifdef Q_OS_UNIX
#include "globalshortcuts_linux.cpp"
#endif

#ifdef Q_OS_WIN
#include "globalshortcuts_win.cpp"
#endif
