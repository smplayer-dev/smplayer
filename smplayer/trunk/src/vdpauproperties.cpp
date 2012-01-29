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

#include "vdpauproperties.h"

VDPAUProperties::VDPAUProperties(QWidget * parent, Qt::WindowFlags f)
	: QDialog(parent, f) 
{
	setupUi(this);

	layout()->setSizeConstraint(QLayout::SetFixedSize);
}

VDPAUProperties::~VDPAUProperties() {
}

void VDPAUProperties::setffh264vdpau(bool b) {
	ffh264vdpau_check->setChecked(b);
}

void VDPAUProperties::setffmpeg12vdpau(bool b) {
	ffmpeg12vdpau_check->setChecked(b);
}

void VDPAUProperties::setffwmv3vdpau(bool b) {
	ffwmv3vdpau_check->setChecked(b);
}

void VDPAUProperties::setffvc1vdpau(bool b) {
	ffvc1vdpau_check->setChecked(b);
}

void VDPAUProperties::setffodivxvdpau(bool b) {
	ffodivxvdpau_check->setChecked(b);
}

void VDPAUProperties::setDisableFilters(bool b) {
	disable_filters_check->setChecked(b);
}

bool VDPAUProperties::ffh264vdpau() {
	return ffh264vdpau_check->isChecked();
}

bool VDPAUProperties::ffmpeg12vdpau() {
	return ffmpeg12vdpau_check->isChecked();
}

bool VDPAUProperties::ffwmv3vdpau() {
	return ffwmv3vdpau_check->isChecked();
}

bool VDPAUProperties::ffvc1vdpau() {
	return ffvc1vdpau_check->isChecked();
}

bool VDPAUProperties::ffodivxvdpau() {
	return ffodivxvdpau_check->isChecked();
}

bool VDPAUProperties::disableFilters() {
	return disable_filters_check->isChecked();
}

#include "moc_vdpauproperties.cpp"

