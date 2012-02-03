//
//  kernel.cpp
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "log.h"
#include "kernel.h"
#include "base_object.h"
#include "platform.h"
#include "graphics_engine.h"
#include "math_util.h"

namespace x2d 
{
    kernel::kernel()    
    : sys_clock_()
    , pause_time_(sys_clock_.current_time())
    , is_paused_(false)
    , sys_timer_(*this, sys_clock_)
    , cur_viewport_(-1)
    {
        sys_timer_.handler( boost::bind(&kernel::sys_timer_handler, this, _1) );
        sys_timer_.set(1.0/60.0);
    }

    void kernel::add_viewport(const boost::shared_ptr<viewport>& vp)
    {
        viewports_.push_back(vp);
    }
    
    void kernel::connect_update( base_object* o )
    {
        update_signal_.connect( boost::bind(&base_object::update, o, _1) );
    }
    
    void kernel::connect_render( base_object* o )
    {
        render_signal_.connect( boost::bind(&base_object::render, o, _1) );
    }
    
    /**
     * System timer callback
     */ 
    void kernel::sys_timer_handler(const clock_info& ci)
    {
        // update all objects
        update_signal_(ci);
        
        graphics_engine::instance().start_frame();

        // render into every viewport
        for(int i=0; i<viewports_.size(); ++i)
        {            
            // setup current viewport
            if(cur_viewport_ != i)
            {
                viewports_.at(i)->use();           
                cur_viewport_ = i;
            }

            // draw background
            viewports_.at(cur_viewport_)->clear();

            // setup camera
            viewports_.at(cur_viewport_)->get_camera()->apply();
            
            // render all objects into this viewport
            render_signal_(ci);
        }
        
        graphics_engine::instance().present_frame();
    }
    
    /**
     * Kernel stepper (runloop)
     */
    int kernel::step()
    {   
        if(is_paused_)
        {
            return -1;
        }
        
        for( timer_container::iterator it = timers_.begin(); it != timers_.end(); )
        {
            const clock_info ci = (*it)->clock_.current_time();
            double ct = ci.time;
            double ft = (*it)->firetime();
            
            if( ct < ft )
            {
                // Timers are ordered by fire time
                return 0;
            }
            
            timer * t = *it;
            it = timers_.erase(it);
            
            t->process(ci);
        }
        
        return 0;
    }
    
} // namespace x2d