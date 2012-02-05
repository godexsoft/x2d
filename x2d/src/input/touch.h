//
//  touch.h
//  x2d
//
//  Created by Alex Kremer on 2/3/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_TOUCH_H__
#define __X2D_TOUCH_H__

#include "math_util.h"

namespace x2d {
namespace input {
    
    class touch 
    {
    private:
        bool            is_valid_;
        float           timestamp_;            // Time of touch event
        point           location_;             // Location of this touch
        point           prev_location_;        // Previous location
        unsigned int    tap_count_;
                
    public:
        touch()
        : is_valid_(false)
        , timestamp_(0.0f)
        , location_(0,0)
        , prev_location_(0,0)
        , tap_count_(0)
        {            
        }
        
        touch(const point& loc, const point& prev_loc, unsigned int tap_count, float timestamp)
        : is_valid_(true)
        , timestamp_(timestamp)
        , location_(loc)
        , prev_location_(prev_loc)
        , tap_count_(tap_count)
        {
        }
        
        touch(const point& loc)
        : is_valid_(true)
        , timestamp_(0.0f)
        , location_(loc)
        , prev_location_(loc)
        , tap_count_(1)
        {
        }
        
        inline const point& location() const 
        { 
            return location_; 
        }
        
        inline const point& prev_location() const 
        { 
            return prev_location_; 
        }

        inline const unsigned int tap_count() const 
        { 
            return tap_count_; 
        }
        
        inline const float timestamp() const 
        { 
            return timestamp_; 
        }
        
        inline const bool is_valid() const
        {
            return is_valid_;
        }
    };

} // namespace input
} // namespace x2d
using namespace x2d::input;

#endif
