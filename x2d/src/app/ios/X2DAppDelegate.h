//
//  X2DAppDelegate.h
//  x2d
//
//  Created by Alex Kremer on 8/9/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import "EAGLView.h"
#import "liverpool_manager.h"
#import "kernel.h"

@interface X2DAppDelegate : UIResponder <UIApplicationDelegate>
{
}

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) EAGLView *gl_view;
@property (nonatomic, retain) CADisplayLink* dl;

@end
