//
//  fps_counter.h
//  x2d
//
//  Created by Alex Kremer on 1/31/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifndef __FPS_COUNTER_H__
#define __FPS_COUNTER_H__

#include "kernel.h"
#include "base_object.h"

class fps_counter 
: public base_object
{
public:
    fps_counter(kernel& k)
    : base_object(k)
    {        
        connect_render();        
    }
protected:
    
    virtual void render(const clock_info& clock) 
    { 
        static int fps = 0;
        static double start = clock.time;
        
        if(clock.time-start >= 1.0)
        {
            LOG("FPS: %d on %f", fps, clock.time);
            start = clock.time;
            fps = 0;
        }
        else
        {
            ++fps;
        }        
    }
};

#endif // __FPS_COUNTER_H__
