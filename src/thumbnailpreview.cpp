/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2025 Ricardo Villalba <ricardo@smplayer.info>

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

#include "thumbnailpreview.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>

/*
#include <QScreen>
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
#include <QDesktopWidget>
#endif
*/

ThumbnailPreview::ThumbnailPreview(QWidget * parent)
	: QWidget(parent, Qt::ToolTip | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::BypassGraphicsProxyWidget)
{
	setAttribute(Qt::WA_ShowWithoutActivating);
	setAttribute(Qt::WA_TransparentForMouseEvents);

	image_label = new QLabel(this);
	image_label->setAlignment(Qt::AlignCenter);

	QVBoxLayout * layout = new QVBoxLayout(this);
	layout->setContentsMargins(2, 2, 2, 2);
	layout->setSpacing(2);
	layout->addWidget(image_label);

	setStyleSheet(
		"ThumbnailPreview { background-color: #000000; border: 1px solid #555555; }"
		"QLabel { color: #ffffff; background: transparent; }"
	);

	hide();
}

ThumbnailPreview::~ThumbnailPreview() {
}

void ThumbnailPreview::showThumbnail(const QPixmap & pixmap, const QPoint & pos) {
	if (pixmap.isNull()) {
		hideThumbnail();
		return;
	}

	QPixmap scaled = pixmap;
	if (scaled.width() > MAX_THUMB_WIDTH) {
		scaled = scaled.scaledToWidth(MAX_THUMB_WIDTH, Qt::SmoothTransformation);
	}
	image_label->setPixmap(scaled);

	adjustSize();

	// Center horizontally on the cursor, place the popup above it, similar
	// to where a QToolTip would appear for the same event. The gap is
	// larger than a typical tooltip offset so the (separate) text tooltip
	// that TimeSlider shows right at the cursor doesn't end up covered by
	// the bottom edge of this popup.
	static const int GAP_ABOVE_CURSOR = 32;

	int x = pos.x() - (width() / 2);
	int y = pos.y() - height() - GAP_ABOVE_CURSOR;
/*
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
	QScreen * s = QApplication::screenAt(pos);
	QRect screen_geometry = s ? s->availableGeometry() : QApplication::primaryScreen()->availableGeometry();
#else
	QRect screen_geometry = QApplication::desktop()->availableGeometry(pos);
#endif

	if (x < screen_geometry.left()) x = screen_geometry.left();
	if (x + width() > screen_geometry.right()) x = screen_geometry.right() - width();
	if (y < screen_geometry.top()) y = pos.y() + GAP_ABOVE_CURSOR; // not enough room above, show below instead
*/
	move(x, y);
	show();
}

void ThumbnailPreview::hideThumbnail() {
	hide();
}
