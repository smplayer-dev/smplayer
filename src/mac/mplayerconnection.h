/*  umplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2009 Ricardo Villalba <rvm@users.sourceforge.net>
    Copyright (C) 2010 Ori Rejwan

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
#ifndef MPLAYERCONNECTION_H
#define MPLAYERCONNECTION_H

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>


@interface MPlayerConnection : NSObject
{
    NSString* connectionName;
    int dwidth, dheight;
    NSThread *renderThread;
    CVOpenGLTextureRef texture;
    NSTimer* timer;

    NSOpenGLContext *context;
    CGLContextObj ctx;


    BOOL isRendering;
    GLuint list;

    //shared memory
    int shm_fd;

    //CoreVideo
    CVPixelBufferRef currentFrameBuffer;
    CVOpenGLTextureCacheRef textureCache;
    NSRect textureFrame;
    NSRect displayFrame;
    GLfloat lowerLeft[2];
    GLfloat lowerRight[2];
    GLfloat upperRight[2];
    GLfloat upperLeft[2];

    //video texture
    unsigned char *image_data;
    unsigned char *image_buffer;
    uint32_t image_width;
    uint32_t image_height;
    uint32_t image_bytes;
    float image_aspect;
    float org_image_aspect;


    NSCondition* cond;
    BOOL cancelLoop;

}

- (id)initWithName:(NSString *)name;
- (id)initWithName2:(int)width myheight:(int)height;
- (void)boundsDidChangeTo:(NSRect)bounds withVideoFrame:(NSRect)frame;
- (void)redraw;
- (void)cleanarea;
- (void) abort;


@end






#endif // MPLAYERCONNECTION_H
