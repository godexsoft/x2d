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
#include "object.h"
#include "graphics_engine.h"
#include "math_util.h"
#include "world.h"

namespace x2d 
{
    kernel::kernel()
    : sys_clock_()
    , pause_time_(sys_clock_.current_time())
    , is_paused_(false)
    , sys_timer_(*this, sys_clock_)
    , cur_viewport_(-1)
    , event_man_(boost::shared_ptr<event_manager>(new event_manager()))
    {
        sys_timer_.handler( boost::bind(&kernel::sys_timer_handler, this, _1) );
        sys_timer_.set(1.0/60.0);
    }

    void kernel::add_viewport(const boost::shared_ptr<viewport>& vp)
    {
        viewports_.push_back(vp);
    }
    
    boost::shared_ptr<viewport> kernel::get_viewport_at(const point& p)
    {
        for(int i=0; i<viewports_.size(); ++i)
        {      
            if(viewports_.at(i)->box().contains_point(p))
            {
                LOG("Found viewport %d", i);
                return viewports_.at(i);
            }
        }   
        
        LOG("Viewport could not be matched for point %f %f", p.x, p.y);
        return boost::shared_ptr<viewport>(); // not found
    }
    
    boost::signals::connection kernel::connect_update( base_object* o )
    {
        return update_signal_.connect( boost::bind(&base_object::update, o, _1) );
    }
    
    boost::signals::connection kernel::connect_render( base_object* o, float z, bool camera_space )
    {
        boost::function<void(const clock_info&)> s = boost::bind(&base_object::render, o, _1); // the real render function
        return render_signal_.connect( z, boost::bind(&kernel::render_wrapper, this, camera_space, s, _1) );
    }
    
    void kernel::connect_touch_input( space s, base_object* o )
    {
        switch(s)
        {
            case SCREEN_SPACE:
                screen_touches_began_signal_.connect( boost::bind(&base_object::touch_input_began, o, SCREEN_SPACE, _1) );
                screen_touches_moved_signal_.connect( boost::bind(&base_object::touch_input_moved, o, SCREEN_SPACE, _1) );
                screen_touches_ended_signal_.connect( boost::bind(&base_object::touch_input_ended, o, SCREEN_SPACE, _1) );
                break;
            case WORLD_SPACE:
                world_touches_began_signal_.connect( boost::bind(&base_object::touch_input_began, o, WORLD_SPACE, _1) );
                world_touches_moved_signal_.connect( boost::bind(&base_object::touch_input_moved, o, WORLD_SPACE, _1) );
                world_touches_ended_signal_.connect( boost::bind(&base_object::touch_input_ended, o, WORLD_SPACE, _1) );
                break;
            case CAMERA_SPACE:
                // TODO
                break;
        }
    }
    
    
    void kernel::dispatch_touches_began(space s, const std::vector<touch>& touches)
    {
        switch(s)
        {
            case SCREEN_SPACE:
                screen_touches_began_signal_(touches);
                break;
            case WORLD_SPACE:
                world_touches_began_signal_(touches);
                break;
            case CAMERA_SPACE:
                // TODO
                break;
        }
    }
    
    void kernel::dispatch_touches_moved(space s, const std::vector<touch>& touches)
    {
        switch(s)
        {
            case SCREEN_SPACE:
                screen_touches_moved_signal_(touches);
                break;
            case WORLD_SPACE:
                world_touches_moved_signal_(touches);
                break;
            case CAMERA_SPACE:
                // TODO
                break;
        }
    }
    
    void kernel::dispatch_touches_ended(space s, const std::vector<touch>& touches)
    {
        switch(s)
        {
            case SCREEN_SPACE:
                screen_touches_ended_signal_(touches);
                break;
            case WORLD_SPACE:
                world_touches_ended_signal_(touches);
                break;
            case CAMERA_SPACE:
                // TODO
                break;
        }
    }
    
    void kernel::dispatch_accelerometer_input( const glm::vec3& accel )
    {
        accel_input_signal_(accel);
    }
    
    void kernel::connect_accelerometer_input( base_object* o )
    {
        accel_input_signal_.connect( boost::bind(&base_object::accelerometer_input, o, _1) );
    }    
    
    void kernel::render_wrapper(bool camera_space, 
        const boost::function<void(const clock_info&)>& f, const clock_info& ci)
    {
        // setup camera
        if(camera_space)
        {
            viewports_.at(cur_viewport_)->get_camera()->remove();
        }
        else
        {
            viewports_.at(cur_viewport_)->get_camera()->apply();
        }
        
        f(ci);
    }
    
    /**
     * System timer callback
     */ 
    void kernel::sys_timer_handler(const clock_info& ci)
    {
        // update physics
    	world::instance().update(ci);
        
        // update all objects
        update_signal_(ci);
        
        graphics_engine::instance().start_frame();

        // render into every viewport
        for(int i=0; i<viewports_.size(); ++i)
        {   
            // setup current viewport
            if(cur_viewport_ != i)
            {
            	LOG("using viewport %d", i);
                viewports_.at(i)->use();           
                cur_viewport_ = i;
            }
            
            // calculate screen space out of camera space for each object which needs it
            for(int i=0; i<camera_space_objects_.size(); ++i)
            {
                object* o = camera_space_objects_.at(i);
                viewports_.at(cur_viewport_)->get_camera()->calculate_in_screen(o);
            }
            
            // draw background
            viewports_.at(cur_viewport_)->clear();
            
            // render all objects into this viewport
            // thru the render_wrapper function
            render_signal_(ci);

            // remove camera matrix (reset state)
            viewports_.at(cur_viewport_)->get_camera()->remove();
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
