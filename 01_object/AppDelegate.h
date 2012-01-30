//
//  AppDelegate.h
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import "EAGLView.h"
#import "kernel.h"
#import "liverpool_manager.h"
#import "base_object.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
    x2d::kernel _k;
    x2d::liverpool::liverpool_manager _lvp_man;
    boost::shared_ptr<base_object> obj;
}

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) EAGLView *gl_view;
@property (nonatomic, retain) CADisplayLink* dl;

@end
