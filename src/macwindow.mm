#import <Cocoa/Cocoa.h>

void* GetNativeWindowHandle(void *window)
{
    NSWindow* nsw = (NSWindow*)window;
    NSView* v = nsw.contentView;
    //[v setWantsLayer:TRUE];
    //return (__bridge NativeWindowType)[v layer]
    return (__bridge void *)[v layer];

}

void GetWindowDrawableSize(void *window, int* w, int* h)
{
    NSWindow* nsw = (NSWindow*)window;
    NSView* contentView = nsw.contentView;
    NSRect viewport = [contentView bounds];
    viewport = [contentView convertRectToBacking:viewport];

    if (w) {
        *w = viewport.size.width;
    }

    if (h) {
        *h = viewport.size.height;
    }
}
