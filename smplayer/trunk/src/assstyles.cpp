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

#include "assstyles.h"
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include "colorutils.h"

AssStyles::AssStyles() {
	fontname = "Arial";
	fontsize = 20;
	primarycolor = QColor("#FFFFFF");
	backcolor = QColor("#000000");
	outlinecolor = QColor("#000000");
	backgroundcolor = QColor("#000000");
	bold = false;
	italic = false;
	halignment = HCenter;
	valignment = Bottom;
	borderstyle = Outline;
	outline = 0.3;
	shadow = 1;
	marginl = 20;
	marginr = 20;
	marginv = 8;
}

void AssStyles::setBackgroundOpacity(int value) {
	backgroundcolor.setAlpha(value);
}

int AssStyles::backgroundOpacity() {
	return backgroundcolor.alpha();
}

void AssStyles::save(QSettings * set) {
	qDebug("AssStyles::save");

	set->setValue("styles/fontname", fontname);
	set->setValue("styles/fontsize", fontsize);
	set->setValue("styles/primarycolor/argb", ColorUtils::colorToAARRGGBB(primarycolor));
	set->setValue("styles/backcolor/argb", ColorUtils::colorToAARRGGBB(backcolor));
	set->setValue("styles/outlinecolor/argb", ColorUtils::colorToAARRGGBB(outlinecolor));
	set->setValue("styles/backgroundcolor/argb", ColorUtils::colorToAARRGGBB(backgroundcolor));
	set->setValue("styles/bold", bold);
	set->setValue("styles/italic", italic);
	set->setValue("styles/halignment", halignment);
	set->setValue("styles/valignment", valignment);
	set->setValue("styles/borderstyle", borderstyle);
	set->setValue("styles/outline", outline);
	set->setValue("styles/shadow", shadow);
	set->setValue("styles/marginl", marginl);
	set->setValue("styles/marginr", marginr);
	set->setValue("styles/marginv", marginv);
}

void AssStyles::load(QSettings * set) {
	qDebug("AssStyles::load");

	fontname = set->value("styles/fontname", fontname).toString();
	fontsize = set->value("styles/fontsize", fontsize).toInt();

	primarycolor = ColorUtils::AARRGGBBToColor(set->value("styles/primarycolor/argb", "#FFFFFFFF").toString());
	backcolor = ColorUtils::AARRGGBBToColor(set->value("styles/backcolor/argb", "#FF000000").toString());
	outlinecolor = ColorUtils::AARRGGBBToColor(set->value("styles/outlinecolor/argb", "#FF000000").toString());
	backgroundcolor = ColorUtils::AARRGGBBToColor(set->value("styles/backgroundcolor/argb", "#FF000000").toString());

	bold = set->value("styles/bold", bold).toBool();
	italic = set->value("styles/italic", italic).toBool();
	halignment = set->value("styles/halignment", halignment).toInt();
	valignment = set->value("styles/valignment", valignment).toInt();
	borderstyle = set->value("styles/borderstyle", borderstyle).toInt();
	outline = set->value("styles/outline", outline).toDouble();
	shadow = set->value("styles/shadow", shadow).toDouble();
	marginl = set->value("styles/marginl", marginl).toInt();
	marginr = set->value("styles/marginr", marginr).toInt();
	marginv = set->value("styles/marginv", marginv).toInt();
}

bool AssStyles::exportStyles(const QString & filename) const {
	qDebug("AssStyles::exportStyles: filename: %s", filename.toUtf8().constData());

	QFile f(filename);
	if (f.open(QFile::WriteOnly)) {
		QTextStream out(&f);

		int alignment = halignment;
		if (valignment == 1) alignment += 3; // Middle
		else
		if (valignment == 2) alignment += 6; // Top

		bool is_opaque = (borderstyle == Opaque);
		QColor bgcolor = outlinecolor;
		if (is_opaque) bgcolor = backgroundcolor;

		out << "[Script Info]" << endl;
		out << "ScriptType: v4.00+" << endl;
		out << "Collisions: Normal" << endl;
		out << endl;
		out << "[V4+ Styles]" << endl;
		out << "Format: Name, Fontname, Fontsize, PrimaryColour, BackColour, OutlineColour, Bold, Italic, Alignment, BorderStyle, Outline, Shadow, MarginL, MarginR, MarginV" << endl;
		out << "Style: Default,";
		out << fontname << "," ;
		out << fontsize << "," ;
		out << "&H" << ColorUtils::colorToAABBGGRR(primarycolor) << "," ;
		out << "&H" << ColorUtils::colorToAABBGGRR(backcolor) << "," ;
		out << "&H" << ColorUtils::colorToAABBGGRR(bgcolor) << "," ;
		out << (bold ? -1 : 0) << "," ;
		out << (italic ? -1 : 0) << "," ;
		out << alignment << "," ;
		out << borderstyle << "," ;
		out << outline << "," ;
		out << (is_opaque ? 0 : shadow) << "," ;
		out << marginl << "," ;
		out << marginr << "," ;
		out << marginv;
		out << endl;

		f.close();
		return true;
	}
	return false;
}

// Returns a string for -ass-force-style
// It seems that option ignores "ScriptType: v4.00+" 
// so the function uses the v4.00 format
QString AssStyles::toString() {
	int alignment = halignment;
	if (valignment == 1) alignment += 8; // Middle
	else
	if (valignment == 2) alignment += 4; // Top

	bool is_opaque = (borderstyle == Opaque);
	QColor bgcolor = outlinecolor;
	if (is_opaque) bgcolor = backgroundcolor;

	QString s = "PlayResX=512,PlayResY=320,"; // Aspect of 1.6, it doesn't look too bad either in 4:3 and 16:9

	s += QString("Name=Default,Fontname=%1,Fontsize=%2,PrimaryColour=&H%3,BackColour=&H%4,"
                 "OutlineColour=&H%5,Bold=%6,Italic=%7,Alignment=%8,BorderStyle=%9,")
                 .arg(fontname).arg(fontsize).arg(ColorUtils::colorToAABBGGRR(primarycolor))
                 .arg(ColorUtils::colorToAABBGGRR(backcolor))
                 .arg(ColorUtils::colorToAABBGGRR(bgcolor))
                 .arg(bold ? 1 : 0).arg(italic ? 1 : 0)
                 .arg(alignment).arg(borderstyle);

	s += QString("Outline=%1,Shadow=%2,MarginL=%3,MarginR=%4,MarginV=%5")
                 .arg(outline).arg(is_opaque ? 0 : shadow).arg(marginl).arg(marginr).arg(marginv);


	return s;
}
