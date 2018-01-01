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


#ifndef SHAREWIDGET_H
#define SHAREWIDGET_H

#include <QWidget>
#include <QPushButton>

class QSettings;
class QEvent;

class ShareButton : public QPushButton
{
	Q_OBJECT

public:
	ShareButton(const QString icon_name, const QString & tooltip, QWidget * parent = 0);
	~ShareButton() {};

	QSize sizeHint() const;

protected:
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
};


class ShareWidget : public QWidget
{
	Q_OBJECT

public:
	enum Display { Never = 0, Random = 1, Always = 2 };

	ShareWidget(QSettings * settings, QWidget * parent = 0, Qt::WindowFlags f = 0);
	~ShareWidget();

	virtual void setVisible(bool visible);

	void setActions(int a);
	int actions() { return actions_taken; }

	void loadConfig();
	void saveConfig();

signals:
	void supportClicked();

protected:
	void setActionPerformed(int action);
	void updateButtons();
	void retranslateStrings();
	virtual void changeEvent(QEvent * event);

protected slots:
	void donate();
	void facebook();
	void twitter();

protected:
	ShareButton * donate_button;
	ShareButton * fb_button;
	ShareButton * twitter_button;
	QPushButton * support_button;

	QSettings * set;
	int actions_taken;
	int count;
	int display;
};

#endif

