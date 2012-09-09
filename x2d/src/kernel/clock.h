//
//  clock.h
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_CLOCK_H__
#define __X2D_CLOCK_H__

#include <boost/noncopyable.hpp>

namespace x2d {

    // forward declaration of kernel
    class kernel; 
    
namespace time {

    /**
     * @brief Clock information
     * Contains delta time and absolute time.
     */
    struct clock_info
    {    
        /**
         * Current time
         */
        double time;
        
        /**
         * Time passed since last update
         */
        double delta_time;
        
        /**
         * @param[in] t   Absolute time
         * @param[in] dt  Delta time
         */
        clock_info(double t, double dt)
        : time(t)        
        , delta_time(dt)
        {            
        }
    };
    
    /**
     * @brief Clock class with time stretch support
     */
    class clock
    : public boost::noncopyable
    {
        friend class timer;
        friend class x2d::kernel;
        
    public:        
        clock(double stretch = 1.0);        
        
        /**
         * Get current time and return it as clock_info structure
         */
        const clock_info current_time();        
        
    private:
        double  stretch_;
        double  last_query_;
    };
    
} // namespace time
} // namespace x2d

using namespace x2d::time;

#endif // __X2D_CLOCK_H__