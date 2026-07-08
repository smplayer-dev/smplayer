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

#ifndef THUMBNAILPREVIEW_H
#define THUMBNAILPREVIEW_H

#include <QWidget>
#include <QPixmap>

class QLabel;

// Small frameless, always-on-top popup that shows a video frame thumbnail
// plus a time label, positioned above the seek bar following the cursor.
// Visually it's the image-based sibling of the plain-text QToolTip that
// TimeSlider already shows.
class ThumbnailPreview : public QWidget
{
	Q_OBJECT

public:
	ThumbnailPreview(QWidget * parent = 0);
	~ThumbnailPreview();

	// Shows the popup with "pixmap" near global position "pos".
	// "pos" is expected to be the same point QToolTip would be shown at
	// (i.e. the cursor position over the slider). The time itself isn't
	// shown here: the plain-text QToolTip that TimeSlider already displays
	// covers that, right next to this popup.
	void showThumbnail(const QPixmap & pixmap, const QPoint & pos);

	void hideThumbnail();

private:
	QLabel * image_label;

	static const int MAX_THUMB_WIDTH = 200;
};

#endif
