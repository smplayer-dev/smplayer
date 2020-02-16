/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2020 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "qrcodelabel.h"
#include <QPainter>
#include <QDebug>

extern "C" {
#include "qrcodegen.h"
}

QRCodeLabel::QRCodeLabel(QWidget * parent, Qt::WindowFlags f)
	: QFrame(parent, f)
{
}

QRCodeLabel::~QRCodeLabel() {
}

void QRCodeLabel::setText(const QString & text) {
	qrcode_text = text;
}

void QRCodeLabel::paintEvent(QPaintEvent * event) {
	Q_UNUSED(event);

	QPainter painter(this);

	#if 0
	char * str = qrcode_text.toUtf8().data();
	//qDebug() << "qrcode_text:" << qrcode_text;
	//qDebug("str: '%s'", str);
	#else
	QByteArray str_utf8 = qrcode_text.toUtf8();
	char str[str_utf8.size() + 1];
	strcpy(str, str_utf8.data());
	//qDebug("str: '%s'", str);
	#endif

	uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	bool ok = qrcodegen_encodeText(str, tempBuffer, qrcode, qrcodegen_Ecc_LOW, 
                                   qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);

	if (ok) {
		const int s = qrcodegen_getSize(qrcode);
		const double w = size().width();
		const double h = size().height();
		const double aspect = w / h;
		const double size = ((aspect > 1.0) ? h :w);
		const double scale = size / (s+2);

		int hmargin = 0;
		int vmargin = 0;
		if (aspect > 1.0) hmargin = (w - size) / 2; else vmargin = (h - size) / 2;

		painter.setPen(Qt::NoPen);
		QColor fg = Qt::black;
		QColor bg = Qt::white;

		for (int y = 0; y < s; y++) {
			for (int x = 0; x < s; x++) {
				const int color = qrcodegen_getModule(qrcode, x, y);  // 0 for white, 1 for black
				if (color == 0) painter.setBrush(bg); else painter.setBrush(fg);
				const double rx1 = (x+1) * scale, ry1 = (y+1) * scale;
				QRectF r(rx1 + hmargin, ry1 + vmargin, scale, scale);
				painter.drawRects(&r, 1);
			}
		}
	}
}

#include "moc_qrcodelabel.cpp"

