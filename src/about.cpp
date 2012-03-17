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

#include "about.h"
#include "images.h"
#include "version.h"
#include "global.h"
#include "preferences.h"
#include "paths.h"
#include "mplayerversion.h"

#include <QFile>
#include <QDesktopServices>

using namespace Global;

About::About(QWidget * parent, Qt::WindowFlags f)
	: QDialog(parent, f)
{
	setupUi(this);
	setWindowIcon( Images::icon("logo", 64) );

	logo->setPixmap( QPixmap(":/icons-png/logo.png").scaledToHeight(64, Qt::SmoothTransformation) );
	contrib_icon->setPixmap( Images::icon("contributors" ) );
	translators_icon->setPixmap( Images::icon("translators" ) );
	license_icon->setPixmap( Images::icon("license" ) );

	QString mplayer_version;
	if (pref->mplayer_detected_version > 0) {
		if (pref->mplayer_is_mplayer2) {
			mplayer_version = tr("Using MPlayer2 %1").arg(pref->mplayer2_detected_version);
		} else {
			mplayer_version = tr("Using MPlayer %1").arg(MplayerVersion::toString(pref->mplayer_detected_version));
		}
		mplayer_version += "<br><br>";
	}

	info->setText(
		"<b>SMPlayer</b> &copy; 2006-2012 Ricardo Villalba &lt;rvm@users.sourceforge.net&gt;<br><br>"
		"<b>" + tr("Version: %1").arg(smplayerVersion()) + "</b>" +
#if PORTABLE_APP
                " (" + tr("Portable Edition") + ")" +
#endif
#ifdef EXPERIMENTAL
        "<br>Experimental branch<br>"
#endif
        "<br>" +
        tr("Using Qt %1 (compiled with Qt %2)").arg(qVersion()).arg(QT_VERSION_STR) + "<br><br>" +
		mplayer_version +
		tr("Visit our web for updates:") +"<br>"+
        link("http://smplayer.sf.net") +
        "<br><br>" +
		tr("Get help in our forum:") +"<br>" + link("http://smplayer.sf.net/forum") + 
        "<br><br>" + 
		tr("SMPlayer uses the award-winning MPlayer as playback engine. See %1")
		   .arg("<a href=\"http://www.mplayerhq.hu\">http://www.mplayerhq.hu</a>")
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

	adjustSize();
}

About::~About() {
}

QString About::getTranslators() {
	return QString(
		 tr("The following people have contributed with translations:") +
		"<p>" + 
         trad(tr("German"), "Panagiotis Papadopoulos <pano_90@gmx.net>") +
		 trad(tr("Slovak"), "Sweto <peter.mendel@gmail.com>") +
		 trad(tr("Italian"), QStringList()
			<< "greengreat <gmeildeno@gmail.com>"
			<< "Giancarlo Scola <scola.giancarlo@libero.it>") +
         trad(tr("French"), QStringList()
			<< "Olivier g <1got@caramail.com>"
			<< "Temet <goondy@free.fr>"
			<< "Erwann MEST <kud.gray@gmail.com>") +
         trad(tr("Simplified-Chinese"), QStringList()
			<< "Tim Green <iamtimgreen@gmail.com>"
			<< "OpenBDH <opensource@bendihua.org>") +
         trad(tr("Russian"), QString::fromUtf8("Белый Владимир <wiselord1983@gmail.com>"))+
         trad(tr("Hungarian"), QStringList()
			<< "Charles Barcza <kbarcza@blackpanther.hu>"
			<< "CyberDragon <cyberdragon777@gmail.com>") +
         trad(tr("Polish"), QStringList()
			<< "qla <qla0@vp.pl>"
			<< "Jarek <ajep9691@wp.pl>"
			<< "sake12 <sake12@gmail.com>" ) +
         trad(tr("Japanese"), "Nardog <alphisation@gmail.com>") +
         trad(tr("Dutch"), QStringList()
			<< "profoX <wesley@ubuntu-nl.org>"
			<< "BalaamsMiracle"
			<< "Kristof Bal <kristof.bal@gmail.com>") +
         trad(tr("Ukrainian"), QStringList()
			<< "Motsyo Gennadi <drool@altlinux.ru>"
			<< "Oleksandr Kovalenko <alx.kovalenko@gmail.com>" ) +
         trad(tr("Portuguese - Brazil"), "Ventura <ventura.barbeiro@terra.com.br>") +
         trad(tr("Georgian"), "George Machitidze <giomac@gmail.com>") +
         trad(tr("Czech"), QStringList()
			<< QString::fromUtf8("Martin Dvořák <martin.dvorak@centrum.cz>")
			<< QString::fromUtf8("Jaromír Smrček <jaromir.smrcek@zoner.com>") ) +
         trad(tr("Bulgarian"), "<marzeliv@mail.bg>") +
         trad(tr("Turkish"), "alper er <alperer@gmail.com>") +
         trad(tr("Swedish"), "Leif Larsson <leif.larsson@gmail.com>") +
         trad(tr("Serbian"), "Kunalagon Umuhanik <kunalagon@gmail.com>") +
         trad(tr("Traditional Chinese"), "Hoopoe <dai715.tw@yahoo.com.tw>") +
         trad(tr("Romanian"), "DoruH <DoruHushHush@gmail.com>") +
         trad(tr("Portuguese - Portugal"), QStringList()
			<< "Waxman <waxman.pt@gmail.com>"
			<< QString::fromUtf8("Sérgio Marques <smarquespt@gmail.com>") ) +
		trad(tr("Greek"), "my80s <wamy80s@gmail.com>") +
		trad(tr("Finnish"), "peeaivo <peeaivo@gmail.com>") +
		trad(tr("Korean"), "Heesu Yoon <imsu30@gmail.com>") +
		trad(tr("Macedonian"), "Marko Doda <mark0d0da@gmail.com>") +
		trad(tr("Basque"), QStringList() 
			<< "Piarres Beobide <pi@beobide.net>" 
			<< "Xabier Aramendi <azpidatziak@gmail.com>") +
		trad(tr("Catalan"), QString::fromUtf8("Roger Calvó <rcalvoi@yahoo.com>")) +
		trad(tr("Slovenian"), "Janez Troha <janez.troha@gmail.com>") +
		trad(tr("Arabic"), "Muhammad Nour Hajj Omar <arabianheart@live.com>") +
		trad(tr("Kurdish"), "Si_murg56 <simurg56@gmail.com>") +
		trad(tr("Galician"), "Miguel Branco <mgl.branco@gmail.com>") +
		trad(tr("Vietnamese"), QString::fromUtf8("Lê Xuân Thảo <thaolx@gmail.com>")) +
		trad(tr("Estonian"), QString::fromUtf8("Olav Mägi <olav.magi@hotmail.com>")) +
        trad(tr("Lithuanian"), QStringList() 
			<< "Freemail <ricka_g@freemail.lt>"
			<< QString::fromUtf8("Algimantas Margevičius <margevicius.algimantas@gmail.com>") ) +
        trad(tr("Danish"), "Martin Schlander <mschlander@opensuse.org>") +
        trad(tr("Croatian"), QString::fromUtf8("Josip Kujundžija <marshsmello@gmail.com>")) +
		"");
}

QString About::trad(const QString & lang, const QString & author) {
	return trad(lang, QStringList() << author);
}

QString About::trad(const QString & lang, const QStringList & authors) {
	QString s;
	for (int n = 0; n < authors.count(); n++) {
		QString author = authors[n];
		s += author.replace("<", "&lt;").replace(">", "&gt;");
		if (n < (authors.count()-1)) s += "<br>";
	}
	return QString("<h3>%1:</h3><h4>%2</h4><hr>").arg(lang).arg(s);
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
