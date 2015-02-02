/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2015 Ricardo Villalba <rvm@users.sourceforge.net>

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

	InfoReader * i = InfoReader::obj(pref->mplayer_bin);
	i->getInfo();
	QString mplayer_version = tr("Using %1").arg(i->playerVersion());

	info->setText(
		"<b>SMPlayer</b> &copy; 2006-2015 Ricardo Villalba &lt;rvm@users.sourceforge.net&gt;<br><br>"
		"<b>" + tr("Version: %1").arg(Version::printable()) + "</b>" +
#if PORTABLE_APP
                " (" + tr("Portable Edition") + ")" +
#endif
#ifdef EXPERIMENTAL
        "<br>Experimental branch<br>"
#endif
        "<br>" +
        tr("Using Qt %1 (compiled with Qt %2)").arg(qVersion()).arg(QT_VERSION_STR) + "<br>" +
		mplayer_version + "<br><br>" +
		"<b>"+ tr("Links:") +"</b><br>"+
		tr("Official website:") +" "+  link("http://smplayer.sourceforge.net") +"<br>"+
		tr("Support forum:") +" "+  link("http://smplayer.sourceforge.net/forum/") +"<br>"+
        "<br>" +
		/*
		tr("SMPlayer uses the award-winning MPlayer as playback engine. See %1")
		   .arg("<a href=\"http://www.mplayerhq.hu/design7/info.html\">http://www.mplayerhq.hu</a>") +
		*/
		tr("SMPlayer is a graphical interface for %1 and %2.")
			.arg("<a href=\"http://www.mplayerhq.hu/design7/info.html\">MPlayer</a>")
			.arg("<a href=\"http://www.mpv.io\">MPV</a>") +
        "<br><br>" +
		tr("Subtitles service powered by %1").arg("<a href=\"http://www.opensubtitles.org\">www.OpenSubtitles.org</a>")
        /* + "<br><a href=\"http://www.opensubtitles.org\"><img src=\":icons-png/opensubtitles-logo.png\"></a>" */
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
		"<p>" +
		 tr("Current translators from the transifex teams:") +
		"<p>" + 
		trad(tr("Spanish"), "Ricardo Villalba") +
		trad(tr("Basque"), "Xabier Aramendi") +
		trad(tr("Croatian"), "Gogo") +
		trad(tr("Czech"), QStringList() << QString::fromUtf8("Petr Šimáček") << QString::fromUtf8("Jakub Kožíšek")) +
		trad(tr("Japanese"), QStringList() << "Ever_green" << "Nardog") +
		trad(tr("Korean"), QStringList() << "ParkJS" << "Potato") +
		trad(tr("Portuguese"), QStringList() << QString::fromUtf8("Sérgio Marques") << "Hugo Carvalho") +
		trad(tr("Serbian"), QStringList() << QString::fromUtf8("Mladen Pejaković") << "Miroslav" << "Rancher" << QString::fromUtf8("Slobodan Simić")) +
		trad(tr("Ukrainian"), QStringList() << "Zubr139" << "evmir2" << "vitrolov" << QString::fromUtf8("Олександр") << "Dex") +
		trad(tr("Galician"), QStringList() << QString::fromUtf8("Adrián Chaves Fernández") << "Miguel Branco" << "antiparvos") +
		trad(tr("Lithuanian"), QString::fromUtf8("Algimantas Margevičius")) +
		trad(tr("Malay"), QStringList() << "Abuyop" << "inashdeen") +
		trad(tr("Portuguese - Brazil"), QStringList() << QString::fromUtf8("Maico Sertório") << "Vinicius" << "Ronnie Dilli" << QString::fromUtf8("Lucas Simões") << "Conservador Ressurge") +
		trad(tr("Hebrew"), QStringList() << "GenghisKhan" << "Random User") +
		trad(tr("Simplified Chinese"), QStringList() << "OpenBDH" << "Zhangzheliuli" << "Zhangmin" << "wwj402" << "775405984" << "DefineFC") +
		trad(tr("Vietnamese"), QStringList() << "Anh Phan" << "Biz Over" << "Thu Thao Nguyen Ngoc" << "Duy Truong Nguyen") +
		trad(tr("Polish"), QStringList() << "Filux" << QString::fromUtf8("Łukasz Hryniuk") << QString::fromUtf8("Piotr Strębski") << QString::fromUtf8("Michał Trzebiatowski") << "Grzegorz Pruchniakowski" << "Daniel Krawczyk") +
		trad(tr("Russian"), QStringList() << "WiseLord" << "Viktor" << "DmitryKX" << "Gleb Mekhrenin" << "ElFrio" << "Semen V. Dubina" << "Denis" << "angry_snake" << "Andrei Stepanov") +
		trad(tr("French"), QStringList() << "Olivier Devineau" << "Ybsar" << "Janmaro" << "Guillaume 'zzd10h' Boesel" << "tneskovic" << "Calinou" << "Cajetan Bouchard") +
		trad(tr("Indonesian"), QStringList() << "Mohamad Hasan Al Banna" << "Aulia Firdaus Simbolon" << "Muhammad Fikri Hariri") +
		trad(tr("Danish"), "Michael Larsen") +
		trad(tr("Hungarian"), QStringList() << "Gojko" << QString::fromUtf8("Zsolt Péter Basák") << "chris020891") +
		trad(tr("Turkish"), QStringList() << "Emre Firat" << QString::fromUtf8("Hasan Akgöz") << QString::fromUtf8("якуп") << QString::fromUtf8("Mesut Aktaş")) +
		trad(tr("Finnish"), QString::fromUtf8("Jiri Grönroos")) +
		trad(tr("German"), QStringList() << "Shaggy" << QString::fromUtf8("Michał Trzebiatowski") << "Eclipse" << "j5lx" << "Tobias Bannert" << "Ettore Atalan") +
		trad(tr("Traditional Chinese"), QStringList() << "Taijuin Lee" << "Wpliao" << "Jeff Huang" << "cges30901") +
		trad(tr("Bulgarian"), QStringList() << "Ivailo Monev" << QString::fromUtf8("Радослав") << "Elusiv_man" << "Kiril Kirilov" << QString::fromUtf8("Иван Костадинов")) +
		trad(tr("Norwegian Nynorsk"), QStringList() << "Bjorni" << "F_Sauce") +
		trad(tr("Swedish"), QStringList() << "XC" << "Andreas Gustafsson" << "Patrik Nilsson" << "Kristian" << "Johan Forsberg") +
		trad(tr("Arabic"), QStringList() << "Riyadh" << "Muhammad Fawwaz Orabi" << "Mohamed Sakhri" << QString::fromUtf8("طاهر") << "Mosaab Alzoubi" << "Leonelle Marks" << "Muhammad EL-SEED") +
		trad(tr("Arabic - Saudi Arabia"), "Mohamed") +
		trad(tr("Arabic - Syria"), QStringList() << "Leonelle Marks" << "Muhammad Fawwaz Orabi") +
		trad(tr("Georgian"), "George Machitidze") +
		trad(tr("Sinhala"), QStringList() << "Rathnayake" << "anupeiris" << "sahan777") +
		trad(tr("Greek"), QString::fromUtf8("Γιάννης Ανθυμίδης")) +
		trad(tr("Estonian"), QString::fromUtf8("Olav Mägi")) +
		trad(tr("N'ko"), QStringList() << QString::fromUtf8("Kairaba Cissé") << "Youssouf Diaby" << "Lasnei Kante" << "Kante Soufiane") +
		trad(tr("Italian"), QStringList() << "Damtux" << "Samir Hawamdeh" << "Fabio Mazza" << "Benedetto Gregorio") +
		trad(tr("Uzbek"), "Umid Almasov") +
		trad(tr("Catalan"), QStringList() << "Anna Fenoy" << "Jmontane") +
		trad(tr("Slovak"), QString::fromUtf8("Ján Ďanovský")) +
		trad(tr("British English"), "F_Sauce") +
		trad(tr("Albanian"), "rigels.gordani") +
		trad(tr("Dutch"), QStringList() << "CecilWesterhof" << "meijdam" << "Heimen Stoffels" << "Dragnadh") +
		trad(tr("Romanian"), "msalajan") +
		trad(tr("Khmer"), "Sovichet Tep") +
		trad(tr("Telugu"), "Praveen_Illa") +
		trad(tr("Tamil"), "vithushanth123") +
		trad(tr("Malayalam"), "Akhilan") +
		trad(tr("Thai"), "M. Somsak") +
		trad(tr("Persian"), QStringList() << "Hessam Shahpouri" << "Behrooz Ramezani") +
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
