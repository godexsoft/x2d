//
//  timer.cpp
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "log.h"
#include "timer.h"
#include "kernel.h"

namespace x2d {
namespace time {

    timer::timer(kernel& k, time::clock& c)
    : kernel_(k)
    , clock_(c)
    , fire_( 0 )
    , frequency_( 0 )
    , start_clock_info_(clock_.current_time())
    { }

    timer::timer(kernel& k)
    : kernel_(k)
    , clock_(k.sys_clock())
    , fire_( 0 )
    , frequency_( 0 )
    , start_clock_info_(clock_.current_time())
    { }
    
    timer::~timer()
    {
        cancel();
    }
    
    void timer::cancel()
    {
        kernel_.dequeue( this );
    }
    
    void timer::set(double freq)
    {
        cancel();
        frequency_ = freq;
        start_clock_info_ = clock_.current_time();
        
        fire_ = start_clock_info_.time + frequency_;
        kernel_.enqueue( this );
    }
    
    void timer::handler(const handler_type& h)
    {
        handler_ = h;
    }
    
    void timer::process(const clock_info& ci)
    {
        if(handler_) 
        {
            // calculate timer delta time instead of clock delta time
            double dt = ci.time - start_clock_info_.time;
            clock_info tci(ci.time, dt);
            
            handler_(tci);
        }
        
        // and reset
        start_clock_info_.time = ci.time;
        fire_ += frequency_;        
        kernel_.enqueue( this );
    }
    
} // namespace time
} // namespace x2d