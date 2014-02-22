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

#include <iostream>

// global app pointer :-/
extern app_framework* g_app;

@implementation X2DAppDelegate

@synthesize window = _window;
@synthesize gl_view = _gl_view;
@synthesize render_timer = _render_timer;

- (void)dealloc
{
    [_window release];
    [_gl_view release];
    [_render_timer release];
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
 
    // init rendering timer
    self.render_timer = [NSTimer scheduledTimerWithTimeInterval:0.001
                                                         target:self
                                                       selector:@selector(step:)
                                                       userInfo:nil
                                                        repeats:YES];
    
    [[NSRunLoop currentRunLoop] addTimer:self.render_timer forMode:NSEventTrackingRunLoopMode];
    [[NSRunLoop currentRunLoop] addTimer:self.render_timer forMode:NSDefaultRunLoopMode];
    [[NSRunLoop currentRunLoop] addTimer:self.render_timer forMode:NSModalPanelRunLoopMode];
}

- (void)step:(id)dummy
{
    NSOpenGLContext *currentContext = [self.gl_view openGLContext];
    [currentContext makeCurrentContext];
    
    g_app->step();
    
    [currentContext flushBuffer];
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
    [self.window makeFirstResponder:self.gl_view];
    
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