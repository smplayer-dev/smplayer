/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2023 Ricardo Villalba <ricardo@smplayer.info>

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

#include "myapplication.h"
#include <QSessionManager>

#include <QEvent>
#include <QFileOpenEvent>
#include <QTimer>
#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#ifdef OS_UNIX_NOT_MAC
#include <QProxyStyle>
#include <QStyle>
#include <QStyleFactory>

class MyProxyStyle : public QProxyStyle {
  public:
    int styleHint(StyleHint hint, const QStyleOption *option = 0,
                  const QWidget *widget = 0, QStyleHintReturn *returnData = 0) const
    {
        if (hint == QStyle:: SH_ItemView_ActivateItemOnSingleClick)
            return 0;
        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
};
#endif

#ifdef SINGLE_INSTANCE
MyApplication::MyApplication (const QString & appId, int & argc, char ** argv)
	: QtSingleApplication(appId, argc, argv)
#else
MyApplication::MyApplication (const QString & /*appId*/, int & argc, char ** argv)
	: QApplication(argc, argv)
#endif
{
#if QT_VERSION >= 0x050600
	setFallbackSessionManagementEnabled(false);
#endif
#if QT_VERSION >= 0x050000
	connect(this, SIGNAL(commitDataRequest(QSessionManager &)),
            this, SLOT(commitData(QSessionManager &)), Qt::DirectConnection);
#endif

#ifdef OS_UNIX_NOT_MAC
	proxy_style = new MyProxyStyle;
	setStyle(proxy_style);
#endif
};

#ifdef OS_UNIX_NOT_MAC
void MyApplication::changeStyle(const QString & style_name) {
	QStyle * style = QStyleFactory::create(style_name);
	if (style) proxy_style->setBaseStyle(style);
}
#endif

void MyApplication::commitData(QSessionManager & manager) {
	manager.release();
}

#ifdef Q_OS_MACX
bool MyApplication::event(QEvent *e) {
	//qDebug() << "MyApplication::event:" << e;

	if (e->type() == QEvent::FileOpen) {
		QFileOpenEvent * open_event = static_cast<QFileOpenEvent *>(e);
		qDebug() << "MyApplication::event: open file:" << open_event->file();
		files_to_open << open_event->file();
		QTimer::singleShot(100, this, SLOT(sendFilesToOpen()));
		return true;
	}
	#ifdef SINGLE_INSTANCE
	return QtSingleApplication::event(e);
	#else
	return QApplication::event(e);
	#endif
}

void MyApplication::sendFilesToOpen() {
	emit openFiles(files_to_open);
	files_to_open.clear();
}
#endif

#ifdef Q_OS_WIN
// TODO: the arguments for Qt (like -style) shouldn't be returned
QStringList MyApplication::winArguments() {
	QString cmdLine = QString::fromWCharArray(GetCommandLine());
	QStringList result;
	int size;
	if (wchar_t **argv = CommandLineToArgvW((const wchar_t *)cmdLine.utf16(), &size)) {
		result.reserve(size);
		wchar_t **argvEnd = argv + size;
		for (wchar_t **a = argv; a < argvEnd; ++a)
			result.append(QString::fromWCharArray(*a));
		LocalFree(argv);
	}
	return result;
}
#endif

#include "moc_myapplication.cpp"

