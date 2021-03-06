//
//  app_ios.mm
//  x2d
//
//  Created by Alex Kremer on 8/9/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

#include "app_framework.h"
#import "ios/X2DIOSAppDelegate.h"
#include "log.h"

extern app_framework* g_app;

void ios_x2d_main(app_framework& app)
{
    LOG("Launching ios application...");
    g_app = &app;
    
    @autoreleasepool {
        char * argv[] = {};
        UIApplicationMain(0, argv, nil, NSStringFromClass([X2DAppDelegate class]));
    }
}

#endif // Simulator or iPhone