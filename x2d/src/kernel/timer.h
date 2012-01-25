//
//  timer.h
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_TIMER_H__
#define __X2D_TIMER_H__

#include "clock.h"
#include <boost/function.hpp>

namespace x2d {
    
class kernel;
    
namespace time {
    
    class timer
    {
        friend class kernel;
        typedef boost::function<void(const clock_info&)> handler_type;
        
    public:
        
        timer(kernel& k, time::clock& c);        
        timer(kernel& k);        
        ~timer();
        
        void handler(const handler_type& h);
        void set(double freq);
        void cancel();
        
        bool operator < ( const timer& other ) const
        {
            return fire_/clock_.stretch_ < other.fire_/other.clock_.stretch_;
        }
        
    private:
        kernel&       kernel_;
        time::clock&  clock_;
        double        fire_;        
        double        frequency_;
        
        clock_info    start_clock_info_;
        handler_type  handler_;

        void process(const clock_info& ci);
        
        double firetime() const 
        {
            return fire_;
        }
    };
    
} // namespace time
} // namespace x2d

using namespace x2d::time;

#endif // __X2D_TIMER_H__