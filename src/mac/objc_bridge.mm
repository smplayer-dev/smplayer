#import "objc_bridge.h"
#import "mplayerconnection.h"
#import <QDebug>

MPlayerConnection* mpc = 0;
int global_zoom=1;
NSAutoreleasePool * pool;

void startObjcFunction(char * connectionName)
{
    pool = [NSAutoreleasePool new];
    mpc = [[MPlayerConnection alloc]initWithName:[[NSString stringWithCString:connectionName] autorelease]];
}

void changezoom(int width, int height) {
    [mpc initWithName2: width myheight:height];
}

void cleararea_bridge() {
    [mpc cleanarea];
}

void stopObjcFunction()
{
    if(mpc) {
        [mpc abort];
        mpc = 0;
        [pool release];        
    }
}
