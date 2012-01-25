//
//  clock.cpp
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "clock.h"
#include "platform.h"

namespace x2d {
namespace time {
    
    clock::clock(double stretch)
    : stretch_(stretch)
    , last_query_(platform::time::current_time() * stretch_)
    {            
    }
        
    const clock_info clock::current_time()
    {   
        double t = platform::time::current_time() * stretch_;
        double dt = t - last_query_;
        last_query_ = t;
        return clock_info(t, dt);
    }

} // namespace time
} // namespace x2d