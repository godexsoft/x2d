//
//  X2DAppDelegate.h
//  x2d
//
//  Created by Alex Kremer on 8/9/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>

#import "MACGLView.h"
// #import "liverpool_manager.h"
// #import "kernel.h"

@interface X2DAppDelegate : NSObject <NSApplicationDelegate>
{
    NSWindow *_window;
    MACGLView *_gl_view;
}

@property (strong, nonatomic) NSWindow *window;
@property (strong, nonatomic) MACGLView *gl_view;
// @property (nonatomic, retain) CADisplayLink* dl;

@end
