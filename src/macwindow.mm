#import <Cocoa/Cocoa.h>

void* GetNativeWindowHandle(void *window)
{
    NSWindow* nsw = (NSWindow*)window;
    NSView* v = nsw.contentView;
    //[v setWantsLayer:TRUE];
    //return (__bridge NativeWindowType)[v layer]
    return (__bridge void *)[v layer];

}
