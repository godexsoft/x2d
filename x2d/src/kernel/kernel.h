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
        }
        
        void resume()
        {
            is_paused_ = false;
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
        bool            is_paused_;
        
        time::clock     sys_clock_;
        timer           sys_timer_;
        
        timer_container timers_;
        
        update_signal   update_signal_;
        render_signal   render_signal_;
    };
    
} // namespace x2d
using namespace x2d;

#endif // __X2D_KERNEL_H__
