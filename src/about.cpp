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

#include "about.h"
#include "images.h"
#include "version.h"
#include "global.h"
#include "preferences.h"
#include "paths.h"
#include "inforeader.h"
#include "links.h"

#include <QFile>
#include <QDesktopServices>
#include <QPushButton>

#if QT_VERSION >= 0x050000
#include "myscroller.h"
#endif

using namespace Global;

About::About(QWidget * parent, Qt::WindowFlags f)
	: QDialog(parent, f)
{
	setupUi(this);
	setWindowIcon( Images::icon("logo", 64) );

	button_box->button(QDialogButtonBox::Ok)->setText(tr("&OK"));

#if QT_VERSION >= 0x050000
	MyScroller::setScroller(info->viewport());
	MyScroller::setScroller(contributions->viewport());
	MyScroller::setScroller(translators->viewport());
	MyScroller::setScroller(license->viewport());
#endif

	logo->setPixmap( QPixmap(":/default-theme/logo.png").scaledToHeight(64, Qt::SmoothTransformation) );
	contrib_icon->setPixmap( Images::icon("contributors" ) );
	translators_icon->setPixmap( Images::icon("translators" ) );
	license_icon->setPixmap( Images::icon("license" ) );

	InfoReader * i = InfoReader::obj(pref->mplayer_bin);
	i->getInfo();
	QString mplayer_version = tr("Using %1").arg(i->playerVersion());

	info->setText(
		"<b>SMPlayer</b> &copy; 2006-2017 Ricardo Villalba &lt;rvm@users.sourceforge.net&gt;<br><br>"
		"<b>" + tr("Version: %1").arg(Version::printable()) + "</b>" +
		(Version::is_unstable() ? "<br><i>" + tr("Development version") +"</i>" : "") +
#if PORTABLE_APP
		"<br>" + tr("Portable Edition") +
#endif
		"<p>" +
		tr("Using Qt %1 (compiled with Qt %2)").arg(qVersion()).arg(QT_VERSION_STR) + "<br>" +
		mplayer_version + "<br><br>" +
		"<b>"+ tr("Links:") +"</b><br>"+
		tr("Official website:") +" "+  link(URL_HOMEPAGE) +"<br>"+
		tr("Support forum:") +" "+  link(URL_FORUM) +"<br>"+
        "<br>" +
		/*
		tr("SMPlayer uses the award-winning MPlayer as playback engine. See %1")
		   .arg("<a href=\"http://www.mplayerhq.hu/design7/info.html\">http://www.mplayerhq.hu</a>") +
		*/
#if defined(MPV_SUPPORT) && defined(MPLAYER_SUPPORT)
		tr("SMPlayer is a graphical interface for %1 and %2.")
			.arg("<a href=\"http://www.mplayerhq.hu/design7/info.html\">MPlayer</a>")
			.arg("<a href=\"http://www.mpv.io\">mpv</a>") +
#else
		tr("SMPlayer is a graphical interface for %1.")
	#ifdef MPV_SUPPORT
			.arg("<a href=\"http://www.mpv.io\">mpv</a>") +
	#endif
	#ifdef MPLAYER_SUPPORT
			.arg("<a href=\"http://www.mplayerhq.hu/design7/info.html\">MPlayer</a>") +
	#endif
#endif
        "<br>"
	);


	QString license_text =
		"<i>"
		"This program is free software; you can redistribute it and/or modify "
		"it under the terms of the GNU General Public License as published by "
		"the Free Software Foundation; either version 2 of the License, or "
		"(at your option) any later version."  "</i><br><br>";
		
	QString license_file = Paths::doc("gpl.html", "en");
	if (QFile::exists(license_file)) {
		license_file = QUrl::fromLocalFile(license_file).toString();
		license_text += QString("<a href=\"%1\">%2</a>").arg(license_file).arg(tr("Read the entire license"));
	}

	if ((pref->language != "en") && (pref->language != "en_US")) {
		QString license_trans_file = Paths::doc("gpl.html", pref->language, false);
		//qDebug("license_trans_file: %s", license_trans_file.toUtf8().constData());
		if (QFile::exists(license_trans_file)) {
			license_trans_file = QUrl::fromLocalFile(license_trans_file).toString();
			license_text += QString("<br><a href=\"%1\">%2</a>").arg(license_trans_file).arg(tr("Read a translation"));
		}
	}
	license->setText(license_text);
	license->setOpenLinks(false);
	license->setOpenExternalLinks(false);
	connect(license, SIGNAL(anchorClicked(const QUrl &)), this, SLOT(openLink(const QUrl&)));

	translators->setHtml( getTranslators() );

	contributions->setText(
        tr("SMPlayer logo by %1").arg("Charles Barcza &lt;kbarcza@blackpanther.hu&gt;") + "<br><br>" +
		tr("Packages for Windows created by %1").arg("redxii &lt;redxii@users.sourceforge.net&gt;") + "<br><br>" +
		tr("Many other people contributed with patches. See the Changelog for details.")
	);


	// Copy the background color ("window") of the tab widget to the "base" color of the qtextbrowsers
	// Problem, it doesn't work with some styles, so first we change the "window" color of the tab widgets.
	info_tab->setAutoFillBackground(true);
	contributions_tab->setAutoFillBackground(true);
	translations_tab->setAutoFillBackground(true);
	license_tab->setAutoFillBackground(true);

	QPalette pal = info_tab->palette();
	pal.setColor(QPalette::Window, palette().color(QPalette::Window) );

	info_tab->setPalette(pal);
	contributions_tab->setPalette(pal);
	translations_tab->setPalette(pal);
	license_tab->setPalette(pal);

	QPalette p = info->palette();
	//p.setBrush(QPalette::Base, info_tab->palette().window());
	p.setColor(QPalette::Base, info_tab->palette().color(QPalette::Window));

	info->setPalette(p);
	contributions->setPalette(p);
	translators->setPalette(p);
	license->setPalette(p);

	tab_widget->removeTab(0);

	adjustSize();
}

About::~About() {
}

QString About::getTranslators() {
	return QString(
		 tr("Many people contributed with translations.") +" "+
		 tr("You can also help to translate SMPlayer into your own language.") +"<p>"+
		 tr("Visit %1 and join a translation team.").arg("<a href=\"http://www.transifex.com/projects/p/smplayer/\">http://www.transifex.com/projects/p/smplayer/</a>") +
		"<p><a href=\"" URL_TRANSLATORS "\">" +
		 tr("Click here to know the translators from the transifex teams") +
		"</a>");
}

QString About::trad(const QString & lang, const QString & author) {
	return trad(lang, QStringList() << author);
}

QString About::trad(const QString & lang, const QStringList & authors) {
	QString s;
	for (int n = 0; n < authors.count(); n++) {
		QString author = authors[n];
		s += author.replace("<", "&lt;").replace(">", "&gt;");
		if (n < (authors.count()-1)) s += ", ";
	}
	//return QString("<h3>%1:</h3><h4>%2</h4><hr>").arg(lang).arg(s);
	return QString("<p><b>%1</b>: %2</p>").arg(lang).arg(s);
}

QString About::link(const QString & url, QString name) {
	if (name.isEmpty()) name = url;
	return QString("<a href=\"" + url + "\">" + name +"</a>");
}

QString About::contr(const QString & author, const QString & thing) {
	return "<li>"+ tr("<b>%1</b> (%2)").arg(author).arg(thing) +"</li>";
}

QSize About::sizeHint () const {
	return QSize(518, 326);
}

void About::openLink(const QUrl & link) {
	qDebug("About::openLink: '%s'", link.toString().toUtf8().constData());
	QDesktopServices::openUrl(link);
}

#include "moc_about.cpp"
