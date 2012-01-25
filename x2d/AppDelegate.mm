//
//  AppDelegate.m
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "AppDelegate.h"
#import "EAGLView.h"
#import "kernel.h"
#import "base_object.h"

#import <QuartzCore/QuartzCore.h>

@implementation AppDelegate

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
    NSLog(@"Starting.");
        
    // register some object with our kernel
    obj = boost::shared_ptr<base_object>( new base_object(_k) );
    
    self.dl = [[CADisplayLink displayLinkWithTarget:self selector:@selector(step)] autorelease];
    self.dl.frameInterval = 1;
    [self.dl addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)step
{
    _k.step();
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[[UIWindow alloc] initWithFrame:CGRectMake(0, 0, 320, 480)] autorelease];
    
    // create opengl view
    self.gl_view = [[[EAGLView alloc] initWithFrame:CGRectMake(0, 0, 320, 480)] autorelease];    
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
    _k.pause();
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    LOG("Will enter foreground");
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    LOG("Resume.");
    _k.resume();
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    LOG("Shutdown graphics engine..");
    graphics_engine::instance().shutdown();
}

@end
