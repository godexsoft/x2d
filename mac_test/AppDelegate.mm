//
//  AppDelegate.m
//  mac_test
//
//  Created by Alex Kremer on 1/23/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#import "AppDelegate.h"
#import "graphics_engine.h"
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>

@implementation OGLView
-(void) drawRect: (NSRect) bounds
{
}
@end

@implementation AppDelegate

@synthesize window = _window;
@synthesize gl_view = _gl_view;

- (void)dealloc
{
    [super dealloc];
}

- (void)step:(id)dummy
{
    _k.step();
}

- (void)start:(id)dummy
{
    NSLog(@"Starting.");
    graphics_engine::instance().init(0, 0, 320, 480);

    // register some object with our kernel
    obj = boost::shared_ptr<base_object>( new base_object(_k) );
    
    [NSTimer scheduledTimerWithTimeInterval:1.0/100.0 target:self 
                                   selector:@selector(step:) userInfo:nil repeats:YES]; 
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    self.gl_view = [[[OGLView alloc] initWithFrame:
                     CGRectMake(0, 0, 320, 480) pixelFormat:[NSOpenGLView defaultPixelFormat]]
                        autorelease];
    
    [self.window.contentView addSubview:self.gl_view];
    [self performSelectorOnMainThread:@selector(start:) withObject:nil waitUntilDone:NO];
}

@end
