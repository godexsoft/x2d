//
//  custom_obj.h
//  x2d
//
//  Created by Alex Kremer on 2/1/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_CUSTOM_OBJ_H__
#define __X2D_CUSTOM_OBJ_H__

#include "object.h"

#include "interpolator.h"
#include "sprite.h"
#include "configuration.h"
#include "animation.h"
#include "space.h"

class custom_obj 
: public object
{
public:
    custom_obj(kernel& k, configuration& c, const object_traits& t)
    : object(k, c, t)
    , li_(rotation_, 0.0f, 360.0f, 2.0f)
    {    
    }
    
private:
    linear_interpolator     li_;
    
protected:
     
    void update(const clock_info& clock)
    {
        li_.update(clock.delta_time);
        
        // call parent update
        object::update(clock);
    }
    
    void touch_input_began(space s, const std::vector<touch>& touches) 
    {
        if(s == WORLD_SPACE)
        {
            position(vector_2d( touches.at(0).location() ));
        }
    }
    
    void touch_input_moved(space s, const std::vector<touch>& touches) 
    {
        if(s == WORLD_SPACE)
        {
            position(vector_2d( touches.at(0).location() ));
        }
    }

};


class small_obj 
: public object
{
public:
    small_obj(kernel& k, configuration& c, const object_traits& t)
    : object(k, c, t)
    , li_(rotation_, 360.0f, 0.0f, 1.0f)
    {    
        // it's a child object so it will get updated and rendered regardless
    }
    
private:
    linear_interpolator     li_;
    
protected:
    
    void update(const clock_info& clock)
    {
        li_.update(clock.delta_time);
        
        // call parent update
        object::update(clock);
    }
};

#endif // __X2D_CUSTOM_OBJ_H__
