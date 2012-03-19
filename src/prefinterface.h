/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef _PREFINTERFACE_H_
#define _PREFINTERFACE_H_

#include "ui_prefinterface.h"
#include "prefwidget.h"

class Preferences;

class PrefInterface : public PrefWidget, public Ui::PrefInterface
{
	Q_OBJECT

public:
	PrefInterface( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~PrefInterface();

	virtual QString sectionName();
	virtual QPixmap sectionIcon();

    // Pass data to the dialog
    void setData(Preferences * pref);

    // Apply changes
    void getData(Preferences * pref);

	bool languageChanged() { return language_changed; };
	bool iconsetChanged() { return iconset_changed; };
	bool guiChanged() { return gui_changed; }
	bool styleChanged() { return style_changed; };
	bool recentsChanged() { return recents_changed; };

protected:
	virtual void createHelp();
	void createLanguageCombo();

	void setLanguage(QString lang);
	QString language();

	void setIconSet(QString set);
	QString iconSet();

	void setResizeMethod(int v);
	int resizeMethod();

	void setSaveSize(bool b);
	bool saveSize();

	void setGUI(QString gui_name);
	QString GUI();

	void setStyle(QString style);
	QString style();

#ifdef SINGLE_INSTANCE
	void setUseSingleInstance(bool b);
	bool useSingleInstance();
#endif

	void setRecentsMaxItems(int n);
	int recentsMaxItems();

	void setSeeking1(int n);
	int seeking1();

	void setSeeking2(int n);
	int seeking2();

	void setSeeking3(int n);
	int seeking3();

	void setSeeking4(int n);
	int seeking4();

	void setUpdateWhileDragging(bool);
	bool updateWhileDragging();

	void setRelativeSeeking(bool);
	bool relativeSeeking();

	void setPreciseSeeking(bool);
	bool preciseSeeking();

	void setDefaultFont(QString font_desc);
	QString defaultFont();

	void setHideVideoOnAudioFiles(bool b);
	bool hideVideoOnAudioFiles();

	// Floating tab
	void setFloatingAnimated(bool b);
	bool floatingAnimated();

	void setFloatingWidth(int percentage);
	int floatingWidth();

	void setFloatingMargin(int pixels);
	int floatingMargin();

	void setDisplayFloatingInCompactMode(bool b);
	bool displayFloatingInCompactMode();

#ifndef Q_OS_WIN
	void setFloatingBypassWindowManager(bool b);
	bool floatingBypassWindowManager();
#endif

protected slots:
	void on_changeFontButton_clicked();
#ifdef SINGLE_INSTANCE
	void changeInstanceImages();
#endif

protected:
	virtual void retranslateStrings();

private:
	bool language_changed;
	bool iconset_changed;
	bool gui_changed;
	bool style_changed;
	bool recents_changed;
};

#endif
