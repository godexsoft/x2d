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

//namespace lp = x2d::liverpool;
//namespace fs = x2d::filesystem;

@implementation X2DAppDelegate

@synthesize window = _window;
@synthesize gl_view = _gl_view;
@synthesize timer = _timer;
// @synthesize dl = _dl;

- (void)dealloc
{
    [_window release];
    [_gl_view release];
//    [_dl release];
    [super dealloc];
}

+ (NSApplication*) sharedApplication
{
    return NSApp;
}

- (void)start:(id)dummy
{
    NSLog(@"x2d mac: Starting.");

//    self.dl = [[CADisplayLink displayLinkWithTarget:self selector:@selector(step)] autorelease];
//    self.dl.frameInterval = 1;
//    [self.dl addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    
    // run user code allowing them to initialize some objects
    // just before starting the main looper.
    g_app->main();
    
    // TODO: replace with Display link if possible on macosx
    self.timer = [NSTimer scheduledTimerWithTimeInterval:1.0/60.0
                                                  target:self
                                                selector:@selector(step)
                                                userInfo:nil
                                                 repeats:YES];
}

- (void)step
{
    g_app->step();
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

//- (void)applicationWillResignActive:(UIApplication *)application
//{
//    LOG("Will resign active");
//}
//
//- (void)applicationDidEnterBackground:(UIApplication *)application
//{
//    LOG("Pause.");
//    g_app->get_kernel().pause();
//}
//
//- (void)applicationWillEnterForeground:(UIApplication *)application
//{
//    LOG("Will enter foreground");
//}
//
//- (void)applicationDidBecomeActive:(UIApplication *)application
//{
//    LOG("Resume.");
//    g_app->get_kernel().resume();
//}
//
//- (void)applicationWillTerminate:(UIApplication *)application
//{
//    LOG("Shutdown graphics engine..");
//    graphics_engine::instance().shutdown();
//}

- (BOOL)windowShouldClose:(id)sender
{
    NSLog(@"App is closed by user x button");
    
    // TODO: schedule shutdown of the engine and program
    
    return YES;
}

@end

#endif // MacOSX