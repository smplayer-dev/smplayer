/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2009 Ricardo Villalba <rvm@escomposlinux.org>

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


#include "prefdrives.h"
#include "images.h"
#include "preferences.h"

#include <QFile>
#include <QFileInfoList>
#include <QDir>

#ifdef Q_OS_WIN
#include <windows.h>

bool isCDDevice(QString drive) {
	if (QSysInfo::WindowsVersion >= QSysInfo::WV_NT) {
		unsigned int r =  GetDriveTypeW((LPCWSTR) drive.utf16());
		qDebug("isCDDevice: '%s' r: %d", drive.toUtf8().data(), r);
		return (r == DRIVE_CDROM);
	} else {
		//Win98
		return true;
	}
}

#endif

PrefDrives::PrefDrives(QWidget * parent, Qt::WindowFlags f)
	: PrefWidget(parent, f )
{
	setupUi(this);

#if !DVDNAV_SUPPORT
	use_dvdnav_check->hide();
#endif

	// DVD device combo
	// In windows, insert the drives letters
#ifdef Q_OS_WIN
	QFileInfoList list = QDir::drives();
	for (int n = 0; n < list.size(); n++) {
		QString s = list[n].filePath();
		if (isCDDevice(s)) {
			if (s.endsWith("/")) s = s.remove( s.length()-1,1);
			dvd_device_combo->addItem( s );
			cdrom_device_combo->addItem( s );
		}
	}
#else
	QDir dev_dir("/dev");
	QStringList devices = dev_dir.entryList( QStringList() << "dvd*" << "cdrom*" << "cdrw*" << "sr*" << "cdrecorder*", 
                                             QDir::Files | QDir::System | QDir::Readable);
	for (int n=0; n < devices.count(); n++) {
		QString device_name = "/dev/" + devices[n];
		qDebug("PrefDrives::PrefDrives: device found: '%s'", device_name.toUtf8().constData());
		dvd_device_combo->addItem(device_name);
		cdrom_device_combo->addItem(device_name);
	}
#endif

	retranslateStrings();
}

PrefDrives::~PrefDrives()
{
}

QString PrefDrives::sectionName() {
	return tr("Drives");
}

QPixmap PrefDrives::sectionIcon() {
    return Images::icon("pref_devices");
}


void PrefDrives::retranslateStrings() {
	retranslateUi(this);

	cdrom_drive_icon->setPixmap( Images::icon("cdrom_drive") );
	dvd_drive_icon->setPixmap( Images::icon("dvd_drive") );

	createHelp();
}

void PrefDrives::setData(Preferences * pref) {
	setDVDDevice( pref->dvd_device );
	setCDRomDevice( pref->cdrom_device );

#if DVDNAV_SUPPORT
	setUseDVDNav( pref->use_dvdnav );
#endif
}

void PrefDrives::getData(Preferences * pref) {
	requires_restart = false;

	pref->dvd_device = dvdDevice();
	pref->cdrom_device = cdromDevice();

#if DVDNAV_SUPPORT
	pref->use_dvdnav = useDVDNav();
#endif
}

void PrefDrives::setDVDDevice( QString dir ) {
	dvd_device_combo->setCurrentText( dir );
}

QString PrefDrives::dvdDevice() {
	return dvd_device_combo->currentText();
}

void PrefDrives::setCDRomDevice( QString dir ) {
	cdrom_device_combo->setCurrentText( dir );
}

QString PrefDrives::cdromDevice() {
	return cdrom_device_combo->currentText();
}

#if DVDNAV_SUPPORT
void PrefDrives::setUseDVDNav(bool b) {
	use_dvdnav_check->setChecked(b);
}

bool PrefDrives::useDVDNav() {
	return use_dvdnav_check->isChecked();
}
#endif

void PrefDrives::createHelp() {
	clearHelp();

	setWhatsThis(cdrom_device_combo, tr("CD device"),
		tr("Choose your CDROM device. It will be used to play "
		   "VCDs and Audio CDs.") );

	setWhatsThis(dvd_device_combo, tr("DVD device"),
		tr("Choose your DVD device. It will be used to play DVDs.") );

#if DVDNAV_SUPPORT
	setWhatsThis(use_dvdnav_check, tr("Enable DVD menus"),
		tr("If this option is checked, smplayer will play DVDs using "
           "dvdnav. Requires a recent version of mplayer compiled with dvdnav "
           "support.") +"<br>" +
        tr("<b>Note 1</b>: cache will be disabled, this can affect performance.") +"<br>"+
        tr("<b>Note 2</b>: you may want to assign the action "
           "\"activate option in DVD menus\" to one of the mouse buttons.") + "<br>"+
        tr("<b>Note 3</b>: this feature is under development, expect a lot of "
           "issues with it."));
#endif
}

#include "moc_prefdrives.cpp"
