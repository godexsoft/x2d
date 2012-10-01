//
//  X2DAppDelegate.mm
//  x2d
//
//  Created by Alex Kremer on 8/9/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#import "X2DAppDelegate.h"
#import "EAGLView.h"

#import "platform.h"
#import "app_framework.h"

#import <QuartzCore/QuartzCore.h>
#include <iostream>

// global app pointer :-/
extern app_framework* g_app;

namespace lp = x2d::liverpool;
namespace fs = x2d::filesystem;

@implementation X2DAppDelegate

@synthesize window = _window;
@synthesize gl_view = _gl_view;
@synthesize dl = _dl;

- (void)dealloc
{
    [_window release];
    [_gl_view release];
    [_dl release];
    [super dealloc];
}

- (void)start:(id)dummy
{
    NSLog(@"x2d iOS: Starting.");

    self.dl = [[CADisplayLink displayLinkWithTarget:self selector:@selector(step)] autorelease];
    self.dl.frameInterval = 1;
    [self.dl addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    
    // run user code allowing them to initialize some objects
    // just before starting the main looper.
    g_app->main();
}

- (void)step
{
    g_app->step();
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    device_capabilities caps;
    
    self.window = [[[UIWindow alloc] initWithFrame:
                        CGRectMake(0, 0,
                                    caps.display_size.width,
                                    caps.display_size.height)] autorelease];
    
    // create opengl view
    self.gl_view = [[[EAGLView alloc] initWithCapabilities:caps] autorelease];
    
    self.window.multipleTouchEnabled = YES;
    self.window.userInteractionEnabled = YES;
    self.window.rootViewController = nil;
    
    [self.window addSubview:self.gl_view];
    [self.window makeKeyAndVisible];
    [self performSelectorOnMainThread:@selector(start:) withObject:nil waitUntilDone:NO];
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    LOG("Will resign active");
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    LOG("Pause.");
    g_app->get_kernel().pause();
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    LOG("Will enter foreground");
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    LOG("Resume.");
    g_app->get_kernel().resume();
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    LOG("Shutdown graphics engine..");
    graphics_engine::instance().shutdown();
}

@end
