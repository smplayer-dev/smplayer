/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2008 Ricardo Villalba <rvm@escomposlinux.org>

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

#include "videopreviewconfigdialog.h"

VideoPreviewConfigDialog::VideoPreviewConfigDialog( QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
{
	setupUi(this);

	aspect_ratio_combo->addItem(tr("Default"), 0);
	aspect_ratio_combo->addItem("4:3", (double) 4/3);
	aspect_ratio_combo->addItem("16:9", (double) 16/9);
	aspect_ratio_combo->addItem("2.35:1", 2.35);
}

VideoPreviewConfigDialog::~VideoPreviewConfigDialog() {
}

void VideoPreviewConfigDialog::setVideoFile(const QString & video_file) {
	filename_edit->setText(video_file);
}

QString VideoPreviewConfigDialog::videoFile() {
	return filename_edit->text();
}

void VideoPreviewConfigDialog::setCols(int cols) {
	columns_spin->setValue(cols);
}

int VideoPreviewConfigDialog::cols() {
	return columns_spin->value();
}

void VideoPreviewConfigDialog::setRows(int rows) {
	rows_spin->setValue(rows);
}

int VideoPreviewConfigDialog::rows() {
	return rows_spin->value();
}

void VideoPreviewConfigDialog::setInitialStep(int step) {
	initial_step_spin->setValue(step);
}

int VideoPreviewConfigDialog::initialStep() {
	return initial_step_spin->value();
}

void VideoPreviewConfigDialog::setMaxWidth(int w) {
	max_width_spin->setValue(w);
}

int VideoPreviewConfigDialog::maxWidth() {
	return max_width_spin->value();
}

void VideoPreviewConfigDialog::setDisplayOSD(bool b) {
	osd_check->setChecked(b);
}

bool VideoPreviewConfigDialog::displayOSD() {
	return osd_check->isChecked();
}

void VideoPreviewConfigDialog::setAspectRatio(double asp) {
	int idx = aspect_ratio_combo->findData(asp);
	if (idx < 0) idx = 0;
	aspect_ratio_combo->setCurrentIndex(idx);
}

double VideoPreviewConfigDialog::aspectRatio() {
	int idx = aspect_ratio_combo->currentIndex();
	return aspect_ratio_combo->itemData(idx).toDouble();
}

#include "moc_videopreviewconfigdialog.cpp"
