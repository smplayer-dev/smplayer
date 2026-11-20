/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2026 Ricardo Villalba <ricardo@smplayer.info>

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

#include "helper.h"

#include <algorithm>

#include <QApplication>
#include <QVector>
#include <QFileInfo>
#include <QColor>
#include <QDir>
#include <QTextCodec>
#include <QWidget>
#include <QDebug>
#include "config.h"
#include "extensions.h"
#include "paths.h"
#include "qtcompat.h"

#ifdef Q_OS_WIN
#include <windows.h> // For the screensaver stuff
#include <shlwapi.h>
#elif QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
#include <QCollator>
#endif

#if EXTERNAL_SLEEP
#include <unistd.h>
#else
#include <qthread.h>
#endif


#if !EXTERNAL_SLEEP
class Sleeper : public QThread
{
public:
	static void sleep(unsigned long secs) {QThread::sleep(secs);}
	static void msleep(unsigned long msecs) {
		//qDebug("sleeping...");
		QThread::msleep(msecs);
		//qDebug("finished");
	}
	static void usleep(unsigned long usecs) {QThread::usleep(usecs);}
};
#endif

/*
QString Helper::dvdForPref(const QString & dvd_id, int title) {
	return  QString("DVD_%1_%2").arg(dvd_id).arg(title);
}
*/

QString Helper::formatTime(int secs) {
	bool negative = (secs < 0);
	secs = qAbs(secs);

	int t = secs;
	int hours = (int) t / 3600;
	t -= hours*3600;
	int minutes = (int) t / 60;
	t -= minutes*60;
	int seconds = t;

	//qDebug() << "Helper::formatTime:" << hours << ":" << minutes << ":" << seconds;

	return QString("%1%2:%3:%4").arg(negative ? "-" : "").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}

QString Helper::formatTime2(double secs) {
	bool negative = (secs < 0);
	secs = qAbs(secs);

	double t = secs + 0.00049999; // add <0.5 ms to round to nearest millisecond, display otherwise sometimes shows 0.499 instead of 0.5
	int hours = (int) t / 3600;
	t -= hours*3600;
	int minutes = (int) t / 60;
	t -= minutes*60;
	int seconds = t;
	t -= seconds;
	int milliseconds = t*1000;

	//qDebug() << "Helper::formatTime: secs:" << secs << "="  << hours << ":" << minutes << ":" << seconds << "." << milliseconds;

	return QString("%1%2:%3:%4.%5").arg(negative ? "-" : "").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')).arg(milliseconds, 3, 10, QChar('0'));
}

QString Helper::formatTimes(double current_sec, double total_time, bool use_milliseconds, bool use_remaining_time) {
	QString time;

	if (use_milliseconds) {
		time = formatTime2(current_sec);
	} else {
		time = formatTime((int) current_sec);
	}

	time += " / ";

	if (!use_remaining_time) {
		time += formatTime((int) total_time);
	} else {
		int remaining_time = (int) total_time - current_sec;
		if (remaining_time < 0) remaining_time = 0;
		time += formatTime(remaining_time);
	}

	//qDebug() << "Helper::formatTimes:" << time;

	return time;
}

QString Helper::timeForJumps(int secs) {
    int minutes = (int) secs / 60;
	int seconds = secs % 60;

	if (minutes==0) {
		return QObject::tr("%n second(s)", "", seconds);
	} else {
		if (seconds==0) 
			return QObject::tr("%n minute(s)", "", minutes);
		else {
			QString m = QObject::tr("%n minute(s)", "", minutes);
			QString s = QObject::tr("%n second(s)", "", seconds);
			return QObject::tr("%1 and %2").arg(m).arg(s);
		}
	}
}

#ifdef Q_OS_WIN
// This function has been copied (and modified a little bit) from Scribus (program under GPL license):
// http://docs.scribus.net/devel/util_8cpp-source.html#l00112
QString Helper::shortPathName(QString long_path) {
	if (QFile::exists(long_path)) {
		QString short_path = long_path;

		const int max_path = 4096;
		WCHAR shortName[max_path];

		QString nativePath = QDir::toNativeSeparators(long_path);
		int ret = GetShortPathNameW((LPCWSTR) nativePath.utf16(), shortName, max_path);
		if (ret != ERROR_INVALID_PARAMETER && ret < MAX_PATH)
			short_path = QString::fromUtf16((const ushort*) shortName);

		return short_path;
	} else {
		return long_path;
	}
}

/*
void Helper::setScreensaverEnabled(bool b) {
	qDebug("Helper::setScreensaverEnabled: %d", b);

	if (b) {
		// Activate screensaver
		SystemParametersInfo( SPI_SETSCREENSAVEACTIVE, true, 0, SPIF_SENDWININICHANGE);
		SystemParametersInfo( SPI_SETLOWPOWERACTIVE, 1, NULL, 0);
		SystemParametersInfo( SPI_SETPOWEROFFACTIVE, 1, NULL, 0);
	} else {
		SystemParametersInfo( SPI_SETSCREENSAVEACTIVE, false, 0, SPIF_SENDWININICHANGE);
		SystemParametersInfo( SPI_SETLOWPOWERACTIVE, 0, NULL, 0);
		SystemParametersInfo( SPI_SETPOWEROFFACTIVE, 0, NULL, 0);
	}
}
*/
#endif

void Helper::msleep(int ms) {
#if EXTERNAL_SLEEP
	//qDebug("Helper::msleep: %d (using usleep)", ms);
	usleep(ms*1000);
#else
	//qDebug("Helper::msleep: %d (using QThread::msleep)", ms);
	Sleeper::msleep( ms );
#endif
}

QString Helper::changeSlashes(QString filename) {
	// Only change if file exists (it's a local file)
	if (QFileInfo(filename).exists())
		return filename.replace('/', '\\');
	else
		return filename;
}

namespace {

#ifndef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
struct NaturalCollator {
	explicit NaturalCollator(Qt::CaseSensitivity sensitivity) {
		collator.setCaseSensitivity(sensitivity);
		collator.setNumericMode(true);
		// Some Qt backends and the C locale cannot collate numbers.
		supports_numbers = collator.compare(QLatin1String("2"), QLatin1String("10")) < 0;
	}

	QCollator collator;
	bool supports_numbers;
};
#endif

bool isAsciiDigit(QChar character) {
	return character >= QLatin1Char('0') && character <= QLatin1Char('9');
}

// Compare digit runs by significant length, then by digit. This also works
// for numbers too long to fit in an integer.
int compareDigitRuns(const QString & left, int & l, const QString & right, int & r) {
	int left_start = l;
	int right_start = r;
	while (l < left.size() && isAsciiDigit(left.at(l))) ++l;
	while (r < right.size() && isAsciiDigit(right.at(r))) ++r;

	while (left_start < l && left.at(left_start) == QLatin1Char('0')) ++left_start;
	while (right_start < r && right.at(right_start) == QLatin1Char('0')) ++right_start;

	if (l - left_start != r - right_start) {
		return (l - left_start < r - right_start) ? -1 : 1;
	}
	while (left_start < l) {
		QChar a = left.at(left_start++);
		QChar b = right.at(right_start++);
		if (a != b) return a < b ? -1 : 1;
	}
	return 0;
}
#endif

struct FileNameEntry {
	explicit FileNameEntry(const QString & path)
		: path(path), name(QFileInfo(QDir::cleanPath(path)).fileName()) {}

	bool operator<(const FileNameEntry & other) const {
		return Helper::compareNaturally(name, other.name, Qt::CaseInsensitive) < 0;
	}

	QString path;
	QString name;
};

}

int Helper::compareNaturally(const QString & left, const QString & right, Qt::CaseSensitivity sensitivity) {
#ifdef Q_OS_WIN
	int result = StrCmpLogicalW(reinterpret_cast<LPCWSTR>(left.utf16()),
	                          reinterpret_cast<LPCWSTR>(right.utf16()));
	if (result == 0 && sensitivity == Qt::CaseSensitive) {
		return QString::compare(left, right, Qt::CaseSensitive);
	}
	return result;
#else
#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
	static const NaturalCollator insensitive(Qt::CaseInsensitive);
	static const NaturalCollator sensitive(Qt::CaseSensitive);
	const NaturalCollator & natural = sensitivity == Qt::CaseInsensitive ? insensitive : sensitive;
	if (natural.supports_numbers) return natural.collator.compare(left, right);
#endif
	const QString a = sensitivity == Qt::CaseInsensitive ? left.toCaseFolded() : left;
	const QString b = sensitivity == Qt::CaseInsensitive ? right.toCaseFolded() : right;
	int l = 0;
	int r = 0;
	while (l < a.size() && r < b.size()) {
		if (isAsciiDigit(a.at(l)) && isAsciiDigit(b.at(r))) {
			int result = compareDigitRuns(a, l, b, r);
			if (result != 0) return result;
		} else {
			QChar left_char = a.at(l++);
			QChar right_char = b.at(r++);
			if (left_char != right_char) return left_char < right_char ? -1 : 1;
		}
	}
	return (l < a.size()) - (r < b.size());
#endif
}

void Helper::sortFileNames(QStringList & files) {
	// Preserve the order of streams and any local batches separated by them.
	int first = 0;
	while (first < files.size()) {
		int end = first;
		while (end < files.size() && !files.at(end).contains(QLatin1String("://"))) ++end;
		if (end - first > 1) {
			QVector<FileNameEntry> entries;
			entries.reserve(end - first);
			for (int i = first; i < end; ++i) entries.append(FileNameEntry(files.at(i)));
			std::stable_sort(entries.begin(), entries.end());
			for (int i = first; i < end; ++i) files[i] = entries.at(i - first).path;
		}
		first = end + 1;
	}
}

bool Helper::directoryContainsDVD(QString directory) {
	//qDebug("Helper::directoryContainsDVD: '%s'", directory.latin1());

	QDir dir(directory);
	QStringList l = dir.entryList();
	bool valid = false;
	for (int n=0; n < l.count(); n++) {
		//qDebug("  * entry %d: '%s'", n, l[n].toUtf8().data());
		if (l[n].toLower() == "video_ts") valid = true;
	}

	return valid;
}

int Helper::qtVersion() {
	QRegExp rx("(\\d+)\\.(\\d+)\\.(\\d+)");
	QString v(qVersion());

	int r = 0;

	if (rx.indexIn(v) > -1) {
		int n1 = rx.cap(1).toInt();
		int n2 = rx.cap(2).toInt();
		int n3 = rx.cap(3).toInt();
		r = n1 * 1000 + n2 * 100 + n3;
	}

	qDebug("Helper::qtVersion: %d", r);

	return r;
}

QStringList Helper::searchForConsecutiveFiles(const QString & initial_file) {
	qDebug("Helper::searchForConsecutiveFiles: initial_file: '%s'", initial_file.toUtf8().constData());

	QStringList files_to_add;
	QStringList matching_files;

	QFileInfo fi(initial_file);
	QString basename = fi.completeBaseName();
	QString extension = fi.suffix();
	QString path = fi.absolutePath();

	QDir dir(path);
	dir.setFilter(QDir::Files);
	dir.setSorting(QDir::Name);

	QRegExp rx("(\\d+)");

	int digits;
	int current_number;
	int pos = 0;
	QString next_name;
	bool next_found = false;
	qDebug("Helper::searchForConsecutiveFiles: trying to find consecutive files");
	while  ( ( pos = rx.indexIn(basename, pos) ) != -1 ) {
		qDebug("Helper::searchForConsecutiveFiles: captured: %s",rx.cap(1).toUtf8().constData());
		digits = rx.cap(1).length();
		current_number = rx.cap(1).toInt() + 1;
		next_name = basename.left(pos) + QString("%1").arg(current_number, digits, 10, QLatin1Char('0'));
		next_name.replace(QRegExp("([\\[\\]?*])"), "[\\1]");
		next_name += "*." + extension;
		qDebug("Helper::searchForConsecutiveFiles: next name = %s",next_name.toUtf8().constData());
		matching_files = dir.entryList((QStringList)next_name);

		if ( !matching_files.isEmpty() ) {
			next_found = true;
			break;
		}
		qDebug("Helper::searchForConsecutiveFiles: pos = %d",pos);
		pos  += digits;
	}

	if (next_found) {
		qDebug("Helper::searchForConsecutiveFiles: adding consecutive files");
		while ( !matching_files.isEmpty() ) {
			qDebug("Helper::searchForConsecutiveFiles: '%s' exists, added to the list", matching_files[0].toUtf8().constData());
			QString filename = path  + "/" + matching_files[0];
			#ifdef Q_OS_WIN
			filename = QDir::toNativeSeparators(filename);
			#endif
			files_to_add << filename;
			current_number++;
			next_name = basename.left(pos) + QString("%1").arg(current_number, digits, 10, QLatin1Char('0'));
			next_name.replace(QRegExp("([\\[\\]?*])"), "[\\1]");
			next_name += "*." + extension;
			matching_files = dir.entryList((QStringList)next_name);
			qDebug("Helper::searchForConsecutiveFiles: looking for '%s'", next_name.toUtf8().constData());
		}
	}

	return files_to_add;
}

QStringList Helper::filesInDirectory(const QString & initial_file, const QStringList & filter) {
	qDebug("Helper::filesInDirectory: initial_file: %s", initial_file.toUtf8().constData());
	//qDebug() << "Helper::filesInDirectory: filter:" << filter;

	QFileInfo fi(initial_file);
	QString current_file = fi.fileName();
	QString path = fi.absolutePath();

	QDir d(path);
	QStringList all_files = d.entryList(filter, QDir::Files);

	QStringList r;
	for (int n = 0; n < all_files.count(); n++) {
		//if (all_files[n] != current_file) {
			QString s = path +"/" + all_files[n];
			#ifdef Q_OS_WIN
			s = QDir::toNativeSeparators(s);
			#endif
			r << s;
		//}
	}

	//qDebug() << "Helper::filesInDirectory: result:" << r;

	return r;
}

QStringList Helper::filesForPlaylist(const QString & initial_file, Preferences::AutoAddToPlaylistFilter filter) {
	QStringList res;

	if (filter == Preferences::ConsecutiveFiles) {
		res = searchForConsecutiveFiles(initial_file);
	} else {
		Extensions e;
		QStringList exts;
		switch (filter) {
			case Preferences::VideoFiles: exts = e.video().forDirFilter(); break;
			case Preferences::AudioFiles: exts = e.audio().forDirFilter(); break;
			case Preferences::MultimediaFiles: exts = e.multimedia().forDirFilter(); break;
			default: ;
		}
		if (!exts.isEmpty()) res = Helper::filesInDirectory(initial_file, exts);
	}

	return res;
}

#ifdef Q_OS_WIN
// Check for Windows shortcuts
QStringList Helper::resolveSymlinks(const QStringList & files) {
	QStringList list = files;
	for (int n=0; n < list.count(); n++) {
		QFileInfo fi(list[n]);
		if (fi.isSymLink()) {
			list[n] = fi.symLinkTarget();
		}
	}
	return list;
}
#endif

#ifndef Q_OS_WIN
QString Helper::findExecutable(const QString & name) {
	QByteArray env = qgetenv("PATH");
	QStringList search_paths = QString::fromLocal8Bit(env.constData()).split(':', QTC_SkipEmptyParts);
	search_paths.prepend(Paths::appPath());

	for (int n = 0; n < search_paths.count(); n++) {
		QString candidate = search_paths[n] + "/" + name;
		qDebug("Helper::findExecutable: candidate: %s", candidate.toUtf8().constData());
		QFileInfo info(candidate);
		if (info.isFile() && info.isExecutable()) {
			qDebug("Helper::findExecutable: executable found: %s", candidate.toUtf8().constData());
			return candidate;
		}
	}
	return QString();
}
#endif
