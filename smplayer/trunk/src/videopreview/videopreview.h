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

#ifndef _VIDEOPREVIEW_H_
#define _VIDEOPREVIEW_H_

#include <QWidget>
#include <QString>
#include <QList>

class QProgressDialog;
class QGridLayout;
class QLabel;
class QSettings;

class VideoInfo 
{
public:
	VideoInfo() { filename.clear(); width = 0; height = 0; length = 0; 
                  size = 0; fps = 0; aspect = 0; video_bitrate = 0; 
                  audio_bitrate = 0; audio_rate = 0; video_format.clear(); };
	~VideoInfo() {};

	QString filename;
	int width;
	int height;
	int length;
	qint64 size;
	double fps;
	double aspect;
	int video_bitrate;
	int audio_bitrate;
	int audio_rate;
	QString video_format;
};

class VideoPreview : public QWidget
{
	Q_OBJECT

public:
	VideoPreview(QString mplayer_path, QWidget * parent = 0, Qt::WindowFlags f = 0);
	~VideoPreview();

	void setVideoFile(QString file) { input_video = file; };
	QString videoFile() { return input_video; };

	void setCols(int cols) { n_cols = cols; };
	int cols() { return n_cols; };

	void setRows(int rows) { n_rows = rows; };
	int rows() { return n_rows; };

	void setGrid(int cols, int rows) { n_cols = cols; n_rows = rows; };

	void setInitialStep(int step) { initial_step = step; };
	int initialStep() { return initial_step; };

	void setMaxWidth(int w) { max_width = w; };
	int maxWidth() { return max_width; };

	void setDisplayOSD(bool b) { display_osd = b; };
	bool displayOSD() { return display_osd; };

	void setAspectRatio(double asp) { aspect_ratio = asp; };
	double aspectRatio() { return aspect_ratio; };

	void clearThumbnails();

	bool createThumbnails();

	bool showConfigDialog();

	void setSettings(QSettings * settings);
	QSettings * settings() { return set; };

	VideoInfo getInfo(const QString & mplayer_path, const QString & filename);
	QString errorMessage() { return error_message; };

protected slots:
	void cancelPressed();
	void saveImage();

protected:
	bool extractImages();
	bool addPicture(const QString & filename, int col, int row, int time); 
	void cleanDir(QString directory);
	void saveSettings();
	void loadSettings();

	QList <QLabel *> label_list;

	QGridLayout * grid_layout;
	QLabel * info;
	QLabel * foot;
	QWidget * w_contents;

	QString mplayer_bin;

	QString output_dir;
	QString full_output_dir;

	QProgressDialog * progress;
	bool canceled;

	QSettings * set;

	QString input_video;
	int n_cols, n_rows, initial_step, max_width;
	double aspect_ratio;
	bool display_osd;

	int thumbnail_width;

	QString last_directory;
	QString error_message;
};

#endif
