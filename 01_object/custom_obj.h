//
//  custom_obj.h
//  x2d
//
//  Created by Alex Kremer on 1/25/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_CUSTOM_OBJ_H__
#define __X2D_CUSTOM_OBJ_H__

#include "base_object.h"

#include "clock.h"
#include "timer.h"
#include "interpolator.h"
#include "graphics_engine.h"
#include "texture.h"
#include "liverpool_manager.h"
#include "resource_manager.h"
#include "sprite.h"

class custom_obj 
: public base_object
{
public:
    custom_obj(kernel& k, liverpool_manager lvp_man)
    : base_object(k)
    , res_man_(lvp_man)
    , texture_(res_man_.get<texture>("res/test/graphics/chupa.png"))
    , spr_(texture_, point(38, 5), size(30, 33)) // only head of the chupakabra :-)
    , alpha_(0.0f)
    , li_(alpha_, 0.0f, 1.0f, 1.0f)
    {        
        connect_update();
        connect_render();        
    }
    
private:
    resource_manager        res_man_;
    resource_ptr<texture>   texture_;
    sprite                  spr_;
    
    float                   alpha_;
    linear_interpolator     li_;
    
protected:
    
    virtual void update(const clock_info& clock) 
    { 
        static int fps = 0;
        static double start = clock.time;
        
        if(clock.time-start >= 1.0)
        {
            LOG("FPS: %d on %f", fps, clock.time);
            start = clock.time;
            fps = 0;
        }
        else
        {
            ++fps;
        }
        
        li_.update(clock.delta_time);
        spr_.alpha(alpha_);
    }
    
    virtual void render(const clock_info& clock) 
    {            
        spr_.draw_at_point(point(160, 240));
    }
};


#endif // __X2D_CUSTOM_OBJ_H__
