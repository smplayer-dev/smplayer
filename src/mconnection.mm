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

#include "mconnection.h"

#include <QMetaObject>
#include "connectioncv.h"

#include <stdio.h>
#include <Cocoa/Cocoa.h>
//#include <QuartzCore/QuartzCore.h>

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

static ConnectionCV * vl_obj = 0;
static MPlayerConnection * mpc = 0;

MConnection::MConnection(ConnectionCV * w, const QString & name) {
	vl_obj = w;
	buffer_name = name;
}

void MConnection::startConnection() {
	char *name = buffer_name.toLatin1().data();
	mpc = [[MPlayerConnection alloc]initWithName:[[NSString stringWithCString:name] autorelease]];
}

void MConnection::stopConnection() {
	if (mpc) {
		//[mpc abort];
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

#include "moc_mconnection.cpp"
