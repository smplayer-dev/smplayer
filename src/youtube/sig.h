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

#ifndef SIG_H
#define SIG_H

#include <QString>

#define SIG_FROM_SMPLAYER_SITE

#define SIG_USE_JSCODE
#define SIG_USE_NO_JSCODE

#ifndef SIG_FROM_SMPLAYER_SITE
#undef SIG_USE_NO_JSCODE
#endif

#ifndef QT_SCRIPT_LIB
#undef SIG_USE_JSCODE
#define SIG_USE_NO_JSCODE
#define SIG_FROM_SMPLAYER_SITE
#endif

class QSettings;

class Sig
{
public:
	void clear() { html5_player = ""; sts = ""; decrypt_function = ""; decrypt_code = "";}

	QString html5_player;
	QString sts;

	QString function_name;
	QString decrypt_function; // javascript

	QString decrypt_code; // No javascript

	QString findFunctions(const QString & text);
	QString aclara(const QString & signature);

	static QString playerURL(const QString & player_name);

	void save(QSettings * set);
	void load(QSettings * set);

protected:
#ifdef SIG_USE_JSCODE
	QString aclaraJS(const QString & signature);
#endif
#ifdef SIG_USE_NO_JSCODE
	QString aclaraNoJS(const QString & signature);
#endif

private:
#ifndef SIG_FROM_SMPLAYER_SITE
	static QString findText(const QString & text, const QString & begin, const QString & end);
#endif

#ifdef SIG_USE_NO_JSCODE
	static QString reverseString(const QString & orig);
#endif
};

#endif

