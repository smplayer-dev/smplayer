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

#include "stereo3ddialog.h"

Stereo3dDialog::Stereo3dDialog(QWidget* parent, Qt::WindowFlags f)
	: QDialog(parent, f)
{
	setupUi(this);

	in_combo->addItem(tr("Side by side parallel (left eye left, right eye right)"), "sbsl");
	in_combo->addItem(tr("Side by side crosseye (right eye left, left eye right)"), "sbsr");
	in_combo->addItem(tr("Side by side with half width resolution (left eye left, right eye right)"), "sbs2l");
	in_combo->addItem(tr("Side by side with half width resolution (right eye left, left eye right)"), "sbs2r");

	in_combo->addItem(tr("Above-below (left eye above, right eye below)"), "abl");
	in_combo->addItem(tr("Above-below (right eye above, left eye below)"), "abr");
	in_combo->addItem(tr("Above-below with half height resolution (left eye above, right eye below)"), "ab2l");
	in_combo->addItem(tr("Above-below with half height resolution (right eye above, left eye below)"), "ab2r");

	// Copy the input formats
	for (int n = 0; n < in_combo->count(); n++) {
		out_combo->addItem( in_combo->itemText(n), in_combo->itemData(n) );
	}

	out_combo->addItem(tr("Anaglyph red/cyan gray"), "arcg");
	out_combo->addItem(tr("Anaglyph red/cyan half colored"), "arch");
	out_combo->addItem(tr("Anaglyph red/cyan color"), "arcc");
	out_combo->addItem(tr("Anaglyph red/cyan color optimized with the least-squares projection of Dubois"), "arcd");

	out_combo->addItem(tr("Anaglyph green/magenta gray"), "agmg");
	out_combo->addItem(tr("Anaglyph green/magenta half colored"), "agmh");
	out_combo->addItem(tr("Anaglyph green/magenta colored"), "agmc");

	out_combo->addItem(tr("Anaglyph yellow/blue gray"), "aybg");
	out_combo->addItem(tr("Anaglyph yellow/blue half colored"), "aybh");
	out_combo->addItem(tr("Anaglyph yellow/blue colored"), "aybc");

	out_combo->addItem(tr("Interleaved rows (left eye has top row, right eye starts on next row)"), "irl");
	out_combo->addItem(tr("Interleaved rows (right eye has top row, left eye starts on next row)"), "irr");

	out_combo->addItem(tr("Mono output (left eye only)"), "ml");
	out_combo->addItem(tr("Mono output (right eye only)"), "mr");

	in_combo->insertItem(0, tr("None"), "none");
	in_combo->insertItem(1, tr("Auto"), "");

	adjustSize();
}

Stereo3dDialog::~Stereo3dDialog() {
}

void Stereo3dDialog::setInputFormat(const QString & in) {
	int i = in_combo->findData(in);
	if (i == -1) i = 0;
	in_combo->setCurrentIndex(i);
}

void Stereo3dDialog::setOutputFormat(const QString & out) {
	int i = out_combo->findData(out);
	if (i == -1) {
		// Use ml as default if the output format is not found
		i = out_combo->findData("ml");
		if (i == -1) i = 0;
	}
	out_combo->setCurrentIndex(i);
}

QString Stereo3dDialog::inputFormat() {
	int i = in_combo->currentIndex();
	return in_combo->itemData(i).toString();
}

QString Stereo3dDialog::outputFormat() {
	int i = out_combo->currentIndex();
	return out_combo->itemData(i).toString();
}

#include "moc_stereo3ddialog.cpp"
