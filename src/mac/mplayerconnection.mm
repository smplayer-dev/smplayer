
/*  umplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2009 Ricardo Villalba <rvm@escomposlinux.org>
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


#import "mplayerconnection.h"
//#import <debug/debug.h>
#import <OpenGL/gl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "globaldataclass.h"
#include <QDebug>



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

@interface MPlayerConnection (PrivateMethods) <MPlayerOSXVOProto>
- (void)threadMain: (id) contextPassed;
- (void) renderOpenGL;
- (void) adaptSize;
- (void) exitThread;
@end

@implementation MPlayerConnection (PrivateMethods)

-(void) threadMain: (id) contextPassed {
    NSAutoreleasePool * pool = [NSAutoreleasePool new];

   NSRunLoop* myRunLoop = [NSRunLoop currentRunLoop];

   NSConnection *serverConnection = [[NSConnection new] autorelease];
   [serverConnection setRootObject:self];
   [serverConnection registerName:connectionName];
   context = [contextPassed retain];
   [context makeCurrentContext];
   GLint swapInterval = 1;
   ctx = (CGLContextObj) [context CGLContextObj];
   [context setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
   cancelLoop = false;
   while (!cancelLoop && [myRunLoop runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]])
   { }   
   [cond lock];
   NSPort* port = [[NSPortNameServer systemDefaultPortNameServer] portForName:connectionName];
   [serverConnection invalidate];
   if(port != nil)
   {
       [port invalidate];
   }
   [pool release];
   [cond signal];
   [cond unlock];
}

-(void) exitThread
{
    if(isRendering)
        [self stop];
     cancelLoop = true;

}

- (int) startWithWidth: (bycopy int)width
            withHeight: (bycopy int)height
             withBytes: (bycopy int)bytes
            withAspect: (bycopy int)aspect
{
        CVReturn error = kCVReturnSuccess;

        image_width = width;
        image_height = height;
        image_bytes = bytes;
        image_aspect = aspect;
        image_aspect = image_aspect/100;
        org_image_aspect = image_aspect;

        shm_fd = shm_open([connectionName UTF8String], O_RDONLY, S_IRUSR);
        if (shm_fd == -1)
        {
                //[Debug log:ASL_LEVEL_ERR withMessage:@"mplayergui: shm_open failed"];
                return 0;
        }

        image_data = (unsigned char*) mmap(NULL, image_width*image_height*image_bytes,
                                          PROT_READ, MAP_SHARED, shm_fd, 0);

        if (image_data == MAP_FAILED)
        {
                //[Debug log:ASL_LEVEL_ERR withMessage:@"mplayergui: mmap failed"];
                return 0;
        }

        image_buffer = (unsigned char*) malloc(image_width*image_height*image_bytes);

        CGLLockContext(ctx);
        [context makeCurrentContext];

        // Setup gl
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);

        // Setup CoreVideo Texture
        error = CVPixelBufferCreateWithBytes( NULL, image_width, image_height, kYUVSPixelFormat, image_buffer, image_width*image_bytes, NULL, NULL, NULL, &currentFrameBuffer);
        if(error != kCVReturnSuccess)
        {
            printf("error");
                //[Debug log:ASL_LEVEL_ERR withMessage:@"Failed to create Pixel Buffer (%d)", error];
         }

        error = CVOpenGLTextureCacheCreate(NULL, 0, ctx, CGLGetPixelFormat(ctx), 0, &textureCache);
        if(error != kCVReturnSuccess)
        {
            printf("error");   // [Debug log:ASL_LEVEL_ERR withMessage:@"Failed to create OpenGL texture Cache (%d)", error];
        }

        CGLUnlockContext(ctx);

        NSRect rect = NSMakeRect (0, 0, width, height);


        textureFrame = rect;
        [self adaptSize];

        isRendering = YES;
        return 1;
}

- (void) stop
{
        isRendering = NO;

        //make sure we destroy the shared buffer
        if (munmap(image_data, image_width*image_height*image_bytes) == -1)
        {
                //[Debug log:ASL_LEVEL_ERR withMessage:@"munmap failed"];
        }

        close(shm_fd);

        CGLLockContext(ctx);

        CVOpenGLTextureCacheRelease(textureCache);
        CVPixelBufferRelease(currentFrameBuffer);

        CGLUnlockContext(ctx);

        free(image_buffer);


}
- (void) render
{
    memcpy(image_buffer, image_data, image_width*image_height*image_bytes);
    [self renderOpenGL];
    //[self adaptSize];
}

- (void) renderOpenGL
{
    CGLLockContext(ctx);
    [context makeCurrentContext];

    CVReturn error = kCVReturnSuccess;
    //CVOpenGLTextureRef texture;

    glClear(GL_COLOR_BUFFER_BIT);

    if (isRendering)
            error = CVOpenGLTextureCacheCreateTextureFromImage (NULL, textureCache,  currentFrameBuffer,  0, &texture);

    if (error == kCVReturnSuccess && isRendering) {

            //Render Video Texture
            CVOpenGLTextureGetCleanTexCoords(texture, lowerLeft, lowerRight, upperRight, upperLeft);

            glEnable(CVOpenGLTextureGetTarget(texture));
            glBindTexture(CVOpenGLTextureGetTarget(texture), CVOpenGLTextureGetName(texture));
            glColor3f(1,1,1);
            NSRect rect = NSMakeRect (0, 0, lowerRight[0], lowerRight[1]);

            textureFrame = rect;


            glBegin(GL_QUADS);
            glTexCoord2f(upperLeft[0], upperLeft[1]);
            glVertex2i(	textureFrame.origin.x,		textureFrame.origin.y);
            glTexCoord2f(lowerLeft[0], lowerLeft[1]);
            glVertex2i(	textureFrame.origin.x,		NSMaxY(textureFrame));
            glTexCoord2f(lowerRight[0], lowerRight[1]);
            glVertex2i(	NSMaxX(textureFrame),		NSMaxY(textureFrame));
            glTexCoord2f(upperRight[0], upperRight[1]);
            glVertex2i(	NSMaxX(textureFrame),		textureFrame.origin.y);
            glEnd();

            glDisable(CVOpenGLTextureGetTarget(texture));

            glFlush();
            //SwapBuffers();

            CVOpenGLTextureRelease(texture);
            globaldata.gl->updateView();

    } else {
            glClearColor(0,0,0,0);
            glFlush();
    }

    CGLUnlockContext(ctx);
}

- (void)adaptSize {

        [self redraw];
        CGLLockContext(ctx);
        [context makeCurrentContext];

        double texture_aspect=textureFrame.size.width/textureFrame.size.height;
        double display_aspect=displayFrame.size.width/displayFrame.size.height;

        int nwidth, nheight, nleft, ntop;

        if (display_aspect>texture_aspect) {
            nheight=displayFrame.size.height;
            ntop=0;
            nwidth=(int)(nheight*texture_aspect);
            nleft=(int)((displayFrame.size.width-nwidth)/2);
        } else {
            nwidth=displayFrame.size.width;
            nheight=(int)(nwidth/texture_aspect);
            nleft=0;
            ntop=(int)((displayFrame.size.height-nheight)/2);
        }

        nleft = 0;
        ntop=0;
        nheight=displayFrame.size.height;
        nwidth=displayFrame.size.width;


        //Setup OpenGL Viewport
        glViewport(nleft, ntop, nwidth, nheight);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, textureFrame.size.width, textureFrame.size.height, 0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        CGLUnlockContext(ctx);
        [self redraw];
}

/* Handled completely by MPE: Ignored */
- (void) toggleFullscreen { }
- (void) ontop { }





@end

@implementation MPlayerConnection

- (id)initWithName:(NSString *)name {
    self = [super init];
    if(self != nil){        
        connectionName = [name retain];
        dwidth=dheight=0;
       /*NSRect rect = NSMakeRect (100, 100, 600, 400);
          unsigned int styleMask = NSTitledWindowMask
                                   | NSMiniaturizableWindowMask;


          NSWindow* myWindow = [NSWindow alloc];
          myWindow = [myWindow initWithContentRect: rect
                               styleMask: styleMask
                               backing: NSBackingStoreBuffered
                               defer: NO];
          [myWindow setTitle: @"This is a test window"];


        NSOpenGLView* glv = [[NSOpenGLView alloc] initWithFrame:NSMakeRect(0,0, 600, 400)];
        [glv retain];
        [myWindow setContentView:glv];
        [myWindow makeKeyAndOrderFront: nil];*/

        cond = [[NSCondition alloc] init];
        renderThread = [[NSThread alloc] initWithTarget:self
                        selector:@selector(threadMain:)
                        object:[NSOpenGLContext currentContext]];        
        [renderThread start];        

    }
    return self;

}

- (id)initWithName2:(int)width myheight:(int)height {

    dwidth = width;

    dheight = height;

    NSRect rectd = NSMakeRect (0, 0, dwidth, dheight);
     displayFrame = rectd;
     [self adaptSize];



    return self;

}

- (void)cleanarea {

    CGLLockContext(ctx);
    [context makeCurrentContext];
     glClear(GL_COLOR_BUFFER_BIT);
    CGLUnlockContext(ctx);
    globaldata.gl->updateView();


}


/* Update OpenGL context and and video frame to bounds (display frame) and
 * frame (video frame).
 */
- (void)boundsDidChangeTo:(NSRect)bounds withVideoFrame:(NSRect)frame {

        CGLLockContext(ctx);
        textureFrame = frame;
        displayFrame = bounds;
        CGLUnlockContext(ctx);
        [self adaptSize];
}

/* Force a redraw of the current frame
 */
- (void)redraw {

        [self renderOpenGL];
}


- (void)abort {
    MPlayerConnection* thread =  (MPlayerConnection*) [NSConnection rootProxyForConnectionWithRegisteredName:connectionName host:nil];
    if(thread != nil)
    {
        [cond lock];
        [thread exitThread];
        //[thread release];
        if(![renderThread isFinished])
        {
            [cond wait];
        }
        [cond unlock];
    }

}

- (void)dealloc {

        [connectionName release];
        [cond dealloc];
        [super dealloc];
}


@end
