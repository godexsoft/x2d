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
#import "liverpool_manager.h"
#import "kernel.h"
#import "game.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
    liverpool_manager         _lvp_man;
    kernel                    _k;
    boost::shared_ptr<game>   _game;
}

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) EAGLView *gl_view;
@property (nonatomic, retain) CADisplayLink* dl;

@end