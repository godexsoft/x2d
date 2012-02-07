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
    , cam_rot_(0.0f)
    , li_(cam_rot_, 0.0f, 360.0f, 2.0f)
    {    
    }
    
private:
    float                   cam_rot_;
    linear_interpolator     li_;
    
protected:
     
    void update(const clock_info& clock)
    {
        li_.update(clock.delta_time);
        
        // rotate the cam
        config().get_object<camera>("camera")->rotation(cam_rot_);
        
        // call parent update
        object::update(clock);
    }
    
    void touch_input_began(space s, const std::vector<touch>& touches) 
    {
        if(s == WORLD_SPACE)
        {
            LOG("NUM WORLD TOUCHES: %d", touches.size());
            config().get_object<camera>("camera")->position( vector_2d(touches.at(0).location().x, touches.at(0).location().y ) );
        }
        else
        {
            LOG("NUM SCREEN TOUCHES: %d", touches.size());
            point loc = touches.at(0).location();
            LOG("Screen space received: %f %f", loc.x, loc.y);
        }
    }
    
    void touch_input_moved(space s, const std::vector<touch>& touches) 
    {
        if(s == WORLD_SPACE)
        {
            position_ = vector_2d( touches.at(0).location() );
        }
    }

};

#endif // __X2D_CUSTOM_OBJ_H__
