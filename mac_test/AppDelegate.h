//
//  AppDelegate.h
//  mac_test
//
//  Created by Alex Kremer on 1/23/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <OpenGL/OpenGL.h>
#import "kernel.h"
#import "base_object.h"

@interface OGLView : NSOpenGLView
{
}
- (void) drawRect: (NSRect) bounds;
@end

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    x2d::kernel _k;
    boost::shared_ptr<base_object> obj;
}
@property (assign) IBOutlet NSWindow *window;
@property (nonatomic, strong) NSOpenGLView* gl_view;

@end
