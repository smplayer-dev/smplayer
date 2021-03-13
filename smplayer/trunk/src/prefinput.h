/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2021 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef PREFINPUT_H
#define PREFINPUT_H

#include "ui_prefinput.h"
#include "preferences.h"
#include "prefwidget.h"
#include <QStringList>

class Preferences;

class PrefInput : public PrefWidget, public Ui::PrefInput
{
	Q_OBJECT

public:
	PrefInput( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~PrefInput();

	virtual QString sectionName();
	virtual QPixmap sectionIcon();

    // Pass data to the dialog
    void setData(Preferences * pref);

    // Apply changes
    void getData(Preferences * pref);

	// Pass action's list to dialog
	/* void setActionsList(QStringList l); */

protected:
	virtual void createHelp();

	void createMouseCombos();

	void setLeftClickFunction(QString f);
	QString leftClickFunction();

	void setRightClickFunction(QString f);
	QString rightClickFunction();

	void setDoubleClickFunction(QString f);
	QString doubleClickFunction();

	void setMiddleClickFunction(QString f);
	QString middleClickFunction();

	void setXButton1ClickFunction(QString f);
	QString xButton1ClickFunction();

	void setXButton2ClickFunction(QString f);
	QString xButton2ClickFunction();

	void setWheelFunction(int function);
	int wheelFunction();

	void setWheelFunctionCycle(Preferences::WheelFunctions flags);
	Preferences::WheelFunctions wheelFunctionCycle();

	void setWheelFunctionSeekingReverse(bool b);
	bool wheelFunctionSeekingReverse();

	void setDragFunction(int function);
	int dragFunction();

#ifdef GLOBALSHORTCUTS
	void setUseGlobalShortcuts(bool);
	bool useGlobalShortcuts();

	void setGrabbedKeys(int);
	int grabbedKeys();
#endif

protected:
	virtual void retranslateStrings();

#ifdef GLOBALSHORTCUTS
protected slots:
	void showGlobalShortcutsDialog();

protected:
	int grabbed_keys;
#endif
};

#endif
