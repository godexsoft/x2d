//
//  kernel.h
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_KERNEL_H__
#define __X2D_KERNEL_H__

#include <boost/signals.hpp>
#include <boost/bind.hpp>
#include <deque>
#include "compare.h"
#include "clock.h"
#include "timer.h"
#include "camera.h"
#include "viewport.h"
#include "input_manager.h"
#include "event_manager.h"
#include "space.h"
#include "glm.hpp"
#include "log.h"

namespace x2d 
{
    class base_object;
    class object;
 
    /**
     * @brief x2d kernel
     */
    class kernel
    {
    public:        
        typedef boost::signal<void(const clock_info&)>          update_signal;        
        typedef boost::signal<void(const clock_info&), 
            boost::last_value<boost::function_traits<void(const clock_info&)>::result_type>,
            float, std::greater<float> >                        render_signal;        
        typedef boost::signal<void (const std::vector<touch>&)> touch_input_signal;        
        typedef boost::signal<void (const glm::vec3&)>          accel_input_signal;        
        
        typedef std::deque<timer*> timer_container;
        
        kernel();
        
        /**
         * Add a viewport
         * @param[in] vp Shared pointer containing a viewport to add
         */
        void add_viewport(const boost::shared_ptr<viewport>& vp);
        
        /**
         * Add a camera-space object.
         * These objects will be first positioned in world-space so that they remain on their place
         * independent of camera transformations.
         * @param[in] obj The object to add
         */
        void add_camera_space_object(object* obj)
        {
            camera_space_objects_.push_back(obj);
        }

        /**
         * Remove a camera-space object if it was added.
         * @param[in] obj The object to remove
         */
        void remove_camera_space_object(object* obj)
        {
            camera_space_objects_.erase(
                std::remove(camera_space_objects_.begin(), 
                            camera_space_objects_.end(), obj), 
                    camera_space_objects_.end());
        }

        /**
         * Get viewport at screen-space location
         * @param[in] p Point to examine
         * @return Viewport in shared pointer if found; null otherwise
         */
        boost::shared_ptr<viewport> get_viewport_at(const point& p); // point is screen-space
        
        /**
         * Pause the kernel
         */
        void pause()
        {
            is_paused_ = true;
            pause_time_ = sys_clock_.current_time();
        }
        
        /**
         * Resume the kernel after pause
         */
        void resume()
        {
            if(is_paused_)
            {
                is_paused_ = false;
                clock_info resume_time = sys_clock_.current_time();
                
                double compensation = resume_time.time - pause_time_.time;

                // compensate all active timers
                for( timer_container::iterator it = timers_.begin(); it != timers_.end(); ++it)
                {
                    LOG("Compensating timer by %f", compensation / sys_clock_.stretch_);
                    (*it)->compensate(compensation / sys_clock_.stretch_);
                }
            }
        }
        
        /**
         * Perform one step of the runloop
         */
        int step();
        
        /**
         * Handler for system timer
         */
        void sys_timer_handler(const clock_info& ci);
        
        // Signal exposure
        boost::signals::connection connect_update( base_object* o );
        boost::signals::connection connect_render( base_object* o, float z, bool camera_space = false );
        void connect_touch_input( space s, base_object* o );
        void connect_accelerometer_input( base_object* o );
        
        time::clock& sys_clock() 
        { 
            return sys_clock_; 
        }
        
        /**
         * Enqueue a timer
         * @param[in] t Timer to add
         */
        void enqueue(timer* t)
        {
            timers_.insert( 
                std::lower_bound(timers_.begin(),
                                 timers_.end(),
                                    t, ptr_cmp<timer>()), t);
        }
        
        /**
         * Remove a timer from the timers queue
         * @param[in] t Timer to remove
         */
        void dequeue(timer* t)
        {
            timers_.erase( 
                std::remove(timers_.begin(),
                            timers_.end(),
                                t ), timers_.end());  
        }
        
        // input support. this will be called by the input device
        // the kernel just passes it through to the input manager if any
        inline void on_touches_began(const std::vector<touch>& touches)
        {
            if(input_man_)
            {
                input_man_->on_touches_began(touches);
            }
        }
        
        inline void on_touches_moved(const std::vector<touch>& touches)
        {
            if(input_man_)
            {
                input_man_->on_touches_moved(touches);
            }
        }
        
        inline void on_touches_ended(const std::vector<touch>& touches)
        {
            if(input_man_)
            {
                input_man_->on_touches_ended(touches);
            }
        }
        
        inline void on_acceleration(float x, float y, float z)
        {
            if(input_man_)
            {
                input_man_->on_acceleration(x, y, z);
            }
        }
        
        /**
         * Attach an input manager
         * @param[in] inp The input manager
         */
        void set_input_manager(const boost::shared_ptr<input_manager>& inp)
        {
            input_man_ = inp;
        }
        
        void dispatch_touches_began(space s, const std::vector<touch>& touches);
        void dispatch_touches_moved(space s, const std::vector<touch>& touches);
        void dispatch_touches_ended(space s, const std::vector<touch>& touches);
        void dispatch_accelerometer_input( const glm::vec3& accel );

        boost::shared_ptr<event_manager> get_event_manager()
        {
            return event_man_;
        }
        
    private:          
        
        /**
         * We use this wrapper when rendering registered objects.
         * This wrapper knows whether the object is in world space or camera space.
         * @param[in] camera_space True if in camera space; false otherwise
         * @param[in] f The real renderer to call
         * @param[in] ci The clock info to pass to the real renderer
         */
        void render_wrapper(bool camera_space, 
            const boost::function<void(const clock_info&)>& f, const clock_info& ci);
        
        // system
        time::clock     sys_clock_;
        timer           sys_timer_;
        
        clock_info      pause_time_;
        bool            is_paused_;
        
        timer_container timers_;
        
        // slots
        update_signal       update_signal_;
        render_signal       render_signal_;
        touch_input_signal  world_touches_began_signal_, world_touches_moved_signal_, world_touches_ended_signal_;
        touch_input_signal  screen_touches_began_signal_, screen_touches_moved_signal_, screen_touches_ended_signal_;
        accel_input_signal  accel_input_signal_;
        
        // rendering
        typedef std::vector<object*>   obj_vec;
        obj_vec         camera_space_objects_;
        
        typedef std::vector<boost::shared_ptr<viewport> > vp_vec;
        vp_vec          viewports_;
        int             cur_viewport_;
        
        // input
        boost::shared_ptr<input_manager> input_man_;
        
        // events
        boost::shared_ptr<event_manager> event_man_;
    };
    
} // namespace x2d
using namespace x2d;

#endif // __X2D_KERNEL_H__
