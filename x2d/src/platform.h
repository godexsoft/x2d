//
//  platform.h
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifndef __X2D_PLATFORM_H__
#define __X2D_PLATFORM_H__

#include <string>
#include "math_util.h"

namespace platform {
    
    struct device_capabilities
    {
        device_capabilities();
        
        /// physical display size in pixels (native)
        size display_size;
        
        /// iOS specific retina display support
        bool has_retina;
        
        /// accelerometer input available
        bool has_accelerometer;
        
        /// touch screen input available
        bool has_touch_screen;
        
        /// multitouch available
        bool has_multitouch;
        
        /// iCP, usb/bluetooth joystick/pad, keyboard, etc.
        bool has_hardware_input;
    };
    
namespace time {
        
    double current_time();
    
} // namespace time
        
namespace filesystem {
    const std::string path_for_resource(const std::string& resource);    
    const std::string path_for_document(const std::string& name);
    
} // namespace filesystem
} // namespace platform

using namespace platform;
using namespace platform::time;
using namespace platform::filesystem;

#endif // __X2D_PLATFORM_H__
