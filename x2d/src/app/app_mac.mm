//
//  app_mac.mm
//  x2d
//
//  Created by Alex Kremer on 8/9/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

#if TARGET_OS_MAC

#include "app_framework.h"
#import "mac/X2DMacAppDelegate.h"
#include "log.h"

extern app_framework* g_app;

void mac_x2d_main(app_framework& app)
{
    LOG("Launching mac application...");
    g_app = &app;
    
    @autoreleasepool {
//        const char * argv[] = {};
//        NSApplicationMain(0, argv);
        
        [NSApplication sharedApplication];
        
        X2DAppDelegate *x2dDelegate = [[X2DAppDelegate alloc] init];
        [NSApp setDelegate:x2dDelegate];
        [NSApp run];
    }
}

#endif // MacOSX