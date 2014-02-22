//
//  platform_apple.mm
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "platform.h"
#include <Foundation/Foundation.h>

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

#if __APPLE__
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    #include <UIKit/UIKit.h>
#elif TARGET_OS_MAC
    #include <Cocoa/Cocoa.h>   
#endif // iOS or Simulator, MacOSX
#endif // __APPLE__

#include <sys/time.h>

namespace platform {
    
    device_capabilities::device_capabilities()
    : has_touch_screen(true) // all iOS devices are touch based
    , has_multitouch(true) // all iOS devices are multitouch
    , has_accelerometer(true) // pretty much all iOS devices have it
    , has_hardware_input(false) // currently hardcode NO
    , has_retina(false)
    {
        // typical display size for iPhone
//        float w = 320.0f;
//        float h = 480.0f;
        
        // mac test
        float w = 320*3;
        float h = 240*3;
        
#if __APPLE__
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        float ver = [[[UIDevice currentDevice] systemVersion] floatValue];
        if (ver >= 3.2f)
        {
            UIScreen* s = [UIScreen mainScreen];
            w = s.currentMode.size.width;
            h = s.currentMode.size.height;
        }
#endif // iOS or Simulator
#endif // __APPLE__
        
        // swap iPad size if mode is crazy
        if ( h == 768.0f && w == 1024.0f )
        {
            w = 768.0f;
            h = 1024.0f;
        }
        else if ( h == 1536.0f && w == 2048.0f )
        {
            w = 1536.0f;
            h = 2048.0f;
        }
        
        // check retina iPhone
        if (w == 640.0f && h == 960.0f)
        {
            // use scale instead
            w /= 2.0f;
            h /= 2.0f;
            has_retina = true;
        }
        
        // check iPhone 5
        if (w == 640.0f && h == 1136.0f)
        {
            w /= 2.0f;
            h /= 2.0f;
            has_retina = true;
        }
        
        // check retina iPad
        if (w == 1536.0f && h == 2048.0f)
        {
            w /= 2.0f;
            h /= 2.0f;
            has_retina = true;
        }
        
        display_size = size(w, h);
        
        LOG("Display size from iOS: %fx%f",
            display_size.width, display_size.height);
        LOG("Retina display?: %s", has_retina?"YES":"NO");
    }
    
namespace time {
    
    double current_time()
    {
#if TARGET_OS_IPHONE
        return CACurrentMediaTime();
#elif TARGET_OS_MAC
        return (double)CFAbsoluteTimeGetCurrent();
#endif
    }
    
} // namespace time

namespace filesystem {
    
    const std::string path_for_resource(const std::string& resource)
    {
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
        
        std::string tmp = [[[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:resource.c_str()] ofType:@""] UTF8String];
        
        [pool drain];
        return tmp;
    }
    
    const std::string path_for_document(const std::string& name)
    {
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
        
        NSArray* doc_paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString* root = [doc_paths objectAtIndex:0];
        std::string tmp = [[root stringByAppendingPathComponent:[NSString stringWithUTF8String:name.c_str()]] UTF8String];
        
        [pool drain];
        return tmp;
    }

} // namespace filesystem    
} // namespace platform

