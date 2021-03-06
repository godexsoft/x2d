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
    : max_frame_rate_(90.0)
    , min_frame_rate_(15.0)
    , max_cycles_per_frame_(max_frame_rate_/min_frame_rate_)
    , update_interval_(1.0/max_frame_rate_) // fixed update interval
    , last_frame_time_(0.0f)
    , cycles_left_over_(0.0f)
    , pause_time_(sys_clock_.current_time())
    , is_paused_(false)
    , cur_viewport_(-1)
    , event_man_(boost::shared_ptr<event_manager>(new event_manager()))
    {
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
    
    boost::signals2::connection kernel::connect_update( base_object* o )
    {
        return update_signal_.connect( boost::bind(&base_object::update, o, _1) );
    }
    
    boost::signals2::connection kernel::connect_render( base_object* o, float z, bool camera_space )
    {
        boost::function<void(const clock_info&)> s = boost::bind(&base_object::render, o, _1); // the real render function
        return render_signal_.connect( z, boost::bind(&kernel::render_wrapper, this, camera_space, s, _1) );
    }

    kernel::input_connections_t kernel::connect_touch_input( space s, base_object* o )
    {
        switch(s)
        {
            case SCREEN_SPACE:
                return kernel::input_connections_t(
                        screen_touches_began_signal_.connect( boost::bind(&base_object::touch_input_began, o, SCREEN_SPACE, _1), boost::signals2::at_front ),
                        screen_touches_moved_signal_.connect( boost::bind(&base_object::touch_input_moved, o, SCREEN_SPACE, _1), boost::signals2::at_front ),
                        screen_touches_ended_signal_.connect( boost::bind(&base_object::touch_input_ended, o, SCREEN_SPACE, _1), boost::signals2::at_front )
                );

            case WORLD_SPACE:
                return kernel::input_connections_t(
                        world_touches_began_signal_.connect( boost::bind(&base_object::touch_input_began, o, WORLD_SPACE, _1), boost::signals2::at_front ),
                        world_touches_moved_signal_.connect( boost::bind(&base_object::touch_input_moved, o, WORLD_SPACE, _1), boost::signals2::at_front ),
                        world_touches_ended_signal_.connect( boost::bind(&base_object::touch_input_ended, o, WORLD_SPACE, _1), boost::signals2::at_front )
                );
                
            case CAMERA_SPACE:
                return kernel::input_connections_t(
                        camera_touches_began_signal_.connect( boost::bind(&base_object::touch_input_began, o, CAMERA_SPACE, _1), boost::signals2::at_front ),
                        camera_touches_moved_signal_.connect( boost::bind(&base_object::touch_input_moved, o, CAMERA_SPACE, _1), boost::signals2::at_front ),
                        camera_touches_ended_signal_.connect( boost::bind(&base_object::touch_input_ended, o, CAMERA_SPACE, _1), boost::signals2::at_front )
                );
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
                camera_touches_began_signal_(touches);
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
                camera_touches_moved_signal_(touches);
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
                camera_touches_ended_signal_(touches);
                break;
        }
    }
    
    void kernel::dispatch_accelerometer_input( const glm::vec3& accel )
    {
        accel_input_signal_(accel);
    }
    
    void kernel::dispatch_keyboard_up(const std::string& name)
    {
        keyboard_up_signal_(name);
    }
    
    void kernel::dispatch_keyboard_down(const std::string& name)
    {
        keyboard_down_signal_(name);
    }
    
    void kernel::connect_accelerometer_input( base_object* o )
    {
        accel_input_signal_.connect( boost::bind(&base_object::accelerometer_input, o, _1) );
    }    
    
    kernel::keyboard_input_connections_t kernel::connect_keyboard_input( base_object* o )
    {
        return kernel::keyboard_input_connections_t(
            keyboard_down_signal_.connect( boost::bind(&base_object::on_key_down, o, _1), boost::signals2::at_front ),
            keyboard_up_signal_.connect( boost::bind(&base_object::on_key_up, o, _1), boost::signals2::at_front )
        );
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
     * Kernel stepper (runloop)
     */
    int kernel::step()
    {   
        if(is_paused_)
        {
            return -1;
        }

        // TODO: use sys_clock
        const clock_info ci(platform::time::current_time(), update_interval_);
        double update_iterations = ((ci.time - last_frame_time_) + cycles_left_over_);
        
        if(update_iterations > (max_cycles_per_frame_ * update_interval_))
        {
            update_iterations = (max_cycles_per_frame_ * update_interval_);
        }
        
        // update all timers first
        while(timers_.size())
        {
            timer * t = timers_.front();
            
            const clock_info tci = t->clock_.current_time();
            double ct = tci.time;
            double ft = t->firetime();
            
            if( ct < ft )
            {
                // timers are ordered by fire time
                break;
            }
            
            timers_.pop_front(); // remove this timer
            t->process(tci);
        }
        
        while (update_iterations >= update_interval_)
        {
            update_iterations -= update_interval_;
            
            // update physics
            world::instance().update(ci);
            
            // update all objects
            update_signal_(ci);
        }
        
        cycles_left_over_ = update_iterations;
        last_frame_time_ = ci.time;
        
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
            
            // draw background
            viewports_.at(cur_viewport_)->clear();
            
            // render all objects into this viewport
            // thru the render_wrapper function
            render_signal_(ci);
            
            // remove camera matrix (reset state)
            viewports_.at(cur_viewport_)->get_camera()->remove();
        }
        
        graphics_engine::instance().present_frame();
        
        return 0;
    }
    
} // namespace x2d
