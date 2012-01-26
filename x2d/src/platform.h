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

namespace platform {
namespace time {
        
    double current_time();
    
} // namespace time
        
namespace filesystem {
    const std::string path_for_resource(const std::string& resource);    
    const std::string path_for_document(const std::string& name);
    
} // namespace filesystem
} // namespace platform

using namespace platform::time;
using namespace platform::filesystem;

#endif // __X2D_PLATFORM_H__
