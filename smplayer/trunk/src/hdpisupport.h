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

#ifndef HDPISUPPORT_H
#define HDPISUPPORT_H

//#ifndef PORTABLE_APP
#define HDPI_STORE_DATA
//#endif

#include <QString>

class QSettings;

class HDPISupport {

public:
	HDPISupport(const QString & config_path = QString::null);
	~HDPISupport();

#ifdef HDPI_STORE_DATA
	void setConfigPath(const QString & config_path);

	bool load();
	bool save();
#endif

	void apply();

	void setHDPIEnabled(bool b) { enabled = b; };
	bool isHDPIEnabled() { return enabled; };

	void setAutoScale(bool b) { auto_scale = b; };
	bool autoScale() { return auto_scale; };

	void setScaleFactor(double v) { scale_factor = v; };
	double scaleFactor() { return scale_factor; };

	void setPixelRatio(int v) { pixel_ratio = v; };
	int pixelRatio() { return pixel_ratio; };

	static HDPISupport * instance();

private:
	static HDPISupport * instance_obj;
#ifdef HDPI_STORE_DATA
	QSettings * set;
#endif
	bool enabled;
	bool auto_scale;
	double scale_factor;
	int pixel_ratio;
};

#endif

