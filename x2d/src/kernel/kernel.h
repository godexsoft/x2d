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
#include "log.h"

namespace x2d 
{
    class base_object;
    
    class kernel
    {
    public:
        typedef boost::signal<void (const clock_info&)> update_signal;        
        typedef boost::signal<void (const clock_info&)> render_signal;        
        
        typedef std::deque<timer*> timer_container;
        
        kernel();
        
        void pause()
        {
            is_paused_ = true;
            pause_time_ = sys_clock_.current_time();
        }
        
        void resume()
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
        
        int step();
        void sys_timer_handler(const clock_info& ci);
        
        // Signal exposure
        // TODO: priorities (by z?)
        void connect_update( base_object* o );
        void connect_render( base_object* o );
        
        time::clock& sys_clock() 
        { 
            return sys_clock_; 
        }
        
        void enqueue(timer* t)
        {
            timers_.insert( 
                std::lower_bound(timers_.begin(),
                                 timers_.end(),
                                    t, ptr_cmp<timer>()), t);
        }
        
        void dequeue(timer* t)
        {
            timers_.erase( 
                std::remove(timers_.begin(),
                            timers_.end(),
                                t ), timers_.end());  
        }
        
    private:                
        /**
         * System clock. Runs without time stretching
         */
        time::clock     sys_clock_;
        timer           sys_timer_;
        
        clock_info      pause_time_;
        bool            is_paused_;
        
        timer_container timers_;
        
        update_signal   update_signal_;
        render_signal   render_signal_;
        
        // rendering
        std::vector<viewport>   viewports_;
        int                     cur_viewport_;
        camera          camera_, camera2_;
    };
    
} // namespace x2d
using namespace x2d;

#endif // __X2D_KERNEL_H__
