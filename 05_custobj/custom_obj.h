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
    , x_(0.0f)
    , qi_(x_, -50.0f, 100.0f, -50.0f, 3.0f)
    {    
        connect_update();
        connect_render();  
    }
    
private:
    float                   x_;
    quadratic_interpolator  qi_;
    
protected:
        
    virtual void touch_input_began(space s, const std::vector<touch>& touches) 
    {
        if(s == WORLD_SPACE)
        {
            LOG("NUM WORLD TOUCHES: %d", touches.size());
            position_ = vector_2d( touches.at(0).location() );
        }
        else
        {
            LOG("NUM SCREEN TOUCHES: %d", touches.size());
            point loc = touches.at(0).location();
            LOG("Screen space received: %f %f", loc.x, loc.y);
        }
    }
    
    virtual void touch_input_moved(space s, const std::vector<touch>& touches) 
    {
        if(s == WORLD_SPACE)
        {
            position_ = vector_2d( touches.at(0).location() );
        }
    }

};


#endif // __X2D_CUSTOM_OBJ_H__
