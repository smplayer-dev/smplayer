/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2021 Ricardo Villalba <ricardo@smplayer.info>

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

#include "videolayercv.h"
#include <QCoreApplication>
#include <QTimer>
#include <QMetaObject>
#include <QPainter>
#include <QDebug>

#include <stdio.h>
#include <Cocoa/Cocoa.h>

VideoLayerCV * vl_obj = 0;

// MPlayer OS X VO Protocol
@protocol MPlayerOSXVOProto
- (int) startWithWidth: (bycopy int)width
            withHeight: (bycopy int)height
             withBytes: (bycopy int)bytes
            withAspect: (bycopy int)aspect;
- (void) stop;
- (void) render;
- (void) toggleFullscreen;
- (void) ontop;
@end

@interface MPlayerConnection : NSObject <MPlayerOSXVOProto> {
	NSString* connectionName;
}

- (id)initWithName:(NSString *)name;
@end

MPlayerConnection* mpc = 0;


VideoLayerCV::VideoLayerCV(QWidget* parent, Qt::WindowFlags f)
	: VideoLayer(parent, f)
	, is_corevideo(false)
{
	vl_obj = this;
	buffer_name = QString("smplayer-%1").arg(QCoreApplication::applicationPid());
}

VideoLayerCV::~VideoLayerCV() {
}

void VideoLayerCV::playingStarted() {
	qDebug("VideoLayerCV::playingStarted");
	VideoLayer::playingStarted();
}

void VideoLayerCV::playingStopped() {
	qDebug("VideoLayerCV::playingStopped");
	VideoLayer::playingStopped();
	is_corevideo = false;
}

void VideoLayerCV::gotVO(QString vo) {
	qDebug() << "VideoLayerCV::gotVO:" << vo;
	if (vo == "corevideo") {
		is_corevideo = true;
		start_connection(); 
	} else {
		is_corevideo = false;
		stop_connection();
	}
}

void VideoLayerCV::init_slot(int width, int height, int bytes, int aspect) {
	qDebug("VideoLayerCV::init_slot: %d %d %d %d", width, height, bytes, aspect);

	image_width = width;
	image_height = height;
	image_bytes = bytes;

	shm_fd = shm_open(buffer_name.toLatin1().constData(), O_RDONLY, S_IRUSR);
	if (shm_fd == -1) {
		qDebug("VideoLayerCV::init_slot: shm_open failed");
		return;
	}

	image_data = (unsigned char*) mmap(NULL, image_width*image_height*image_bytes,
                                  PROT_READ, MAP_SHARED, shm_fd, 0);

	if (image_data == MAP_FAILED) {
		qDebug("VideoLayerCV::init_slot: mmap failed");
		return;
	}

	image_buffer = (unsigned char*) malloc(image_width*image_height*image_bytes);
	//qDebug() << "VideoLayerCV::init_slot: image_data:" << image_data;
}

void VideoLayerCV::render_slot() {
	//qDebug("VideoLayerCV::render_slot");
	memcpy(image_buffer, image_data, image_width*image_height*image_bytes);

	QImage i(image_buffer, image_width, image_height, image_width * image_bytes, QImage::Format_RGB888);
	frame = QPixmap::fromImage(i);
	update();
}

void VideoLayerCV::stop_slot() {
	qDebug("VideoLayerCV::stop_slot");
}

void VideoLayerCV::paintEvent(QPaintEvent *event) {
	if (playing && is_corevideo && !frame.isNull()) {
		QPainter painter(this);
		painter.drawPixmap(0,0,frame.scaled(size()));
	} else {
		VIDEOLAYER_PARENT::paintEvent(event);
	}
}

void VideoLayerCV::start_connection() {
	char *name = buffer_name.toLatin1().data();
	mpc = [[MPlayerConnection alloc]initWithName:[[NSString stringWithCString:name] autorelease]];
}

void VideoLayerCV::stop_connection() {
	if (mpc) {
		[mpc abort];
		mpc = 0;
	}
}


@implementation MPlayerConnection
- (id)initWithName:(NSString *)name {
	self = [super init];
	printf("initWithName: %s\n", [name UTF8String]);
	if (self != nil) {        
	connectionName = [name retain];
		NSConnection * conn = [[NSConnection new] autorelease];
		[conn setRootObject:self];
		[conn runInNewThread];
		[conn registerName:connectionName];
	}
	return self;
}

- (int) startWithWidth: (bycopy int)width
            withHeight: (bycopy int)height
             withBytes: (bycopy int)bytes
            withAspect: (bycopy int)aspect
{
	//printf("MPlayerConnection::startWithWidth %d %d %d %d\n", width, height, bytes, aspect);
	if (vl_obj) {
		QMetaObject::invokeMethod(vl_obj, "init_slot", Qt::QueuedConnection,
			Q_ARG(int, width), Q_ARG(int, height), Q_ARG(int, bytes), Q_ARG(int, aspect));
	}
	return 1;
}

- (void) stop
{
	//printf("MPlayerConnection::stop\n");
	if (vl_obj) {
		QMetaObject::invokeMethod(vl_obj, "stop_slot", Qt::QueuedConnection);
	}
}

- (void) render
{
	//printf("MPlayerConnection::render\n");
	if (vl_obj) {
		QMetaObject::invokeMethod(vl_obj, "render_slot", Qt::QueuedConnection);
	}
}

- (void) toggleFullscreen { }
- (void) ontop { }

@end

#include "moc_videolayercv.cpp"
