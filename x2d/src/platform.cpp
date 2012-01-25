//
//  platform.cpp
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "platform.h"
#include <sys/time.h>

namespace platform {
namespace time {
    
    double current_time()
    {
        struct timeval start;        
        gettimeofday(&start, NULL);
        return double(start.tv_sec) + start.tv_usec * 1E-6;
    }
    
} // namespace time
} // namespace platform
using namespace platform::time;

