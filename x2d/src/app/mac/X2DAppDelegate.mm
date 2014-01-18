//
//  X2DAppDelegate.mm
//  x2d
//
//  Created by Alex Kremer on 8/9/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

#if TARGET_OS_MAC && !TARGET_OS_IPHONE

#import "X2DMacAppDelegate.h"

#import "platform.h"
#import "app_framework.h"

#import <QuartzCore/QuartzCore.h>
#include <iostream>

// global app pointer :-/
extern app_framework* g_app;

@implementation X2DAppDelegate

@synthesize window = _window;
@synthesize gl_view = _gl_view;

- (void)dealloc
{
    CVDisplayLinkRelease(_dl);
    
    [_window release];
    [_gl_view release];
    [super dealloc];
}

+ (NSApplication*) sharedApplication
{
    return NSApp;
}

- (void)start:(id)dummy
{
    NSLog(@"x2d mac: Starting.");

    // run user code allowing them to initialize some objects
    // just before starting the main looper.
    g_app->main();
    
    // init display link
    GLint swapInt = 1;
    [[self.gl_view openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
    CVDisplayLinkCreateWithActiveCGDisplays(&_dl);
    CVDisplayLinkSetOutputCallback(_dl, &x2dDisplayLinkCallback, self);
    
    CGLContextObj cglContext = static_cast<CGLContextObj>([[self.gl_view openGLContext] CGLContextObj]);
    CGLPixelFormatObj cglPixelFormat = static_cast<CGLPixelFormatObj>([[self.gl_view pixelFormat] CGLPixelFormatObj]);
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(_dl, cglContext, cglPixelFormat);

    // start spinning
    CVDisplayLinkStart(_dl);
}

static CVReturn x2dDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(X2DAppDelegate*)displayLinkContext step:outputTime];
    return result;
}

- (CVReturn)step:(const CVTimeStamp*)outputTime
{
    NSOpenGLContext *currentContext = [self.gl_view openGLContext];
    [currentContext makeCurrentContext];
    
    // must lock GL context because display link is threaded
    CGLLockContext(static_cast<CGLContextObj>([currentContext CGLContextObj]));
    
    g_app->step();
    
    [currentContext flushBuffer];
    CGLUnlockContext(static_cast<CGLContextObj>([currentContext CGLContextObj]));
    
    return kCVReturnSuccess;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    device_capabilities caps;
    
    NSRect rect = NSMakeRect(0, 0,
                             caps.display_size.width,
                             caps.display_size.height);
    
    NSUInteger mask = NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask;
    self.window  = [[[NSWindow alloc] initWithContentRect:rect
                                                styleMask:mask
                                                  backing:NSBackingStoreBuffered
                                                    defer:NO] autorelease];
    [self.window setDelegate:self];
    [self.window setTitle:@"x2d (TODO: put custom name)"];
    
    // create opengl view
    self.gl_view = [[[MACGLView alloc] initWithCapabilities:caps] autorelease];

    [self.window setContentView:self.gl_view];
    [self performSelectorOnMainThread:@selector(start:) withObject:nil waitUntilDone:NO];
    
    [self.window makeKeyAndOrderFront:NSApp];
}

- (BOOL)windowShouldClose:(id)sender
{
    NSLog(@"App is closed by user x button");
    
    // TODO: schedule shutdown of the engine and program
    
    return YES;
}

@end

#endif // MacOSX