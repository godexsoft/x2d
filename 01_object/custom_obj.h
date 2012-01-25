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

class custom_obj 
: public base_object
{
public:
    custom_obj(kernel& k)
    : base_object(k)
    , alpha_(0.0f)
    , li_(alpha_, 0.0f, 0.1f, 2.0f, 0.0f, 1.0f)
    , height_(1.0f)
    , qi_(height_, 1.0f, 3.0f, 1.0f, 1.0f)
    , texture_()
    {        
        connect_update();
        connect_render();
    }
    
private:
    float               alpha_;
    cubic_interpolator  li_;
    
    float                  height_;
    quadratic_interpolator qi_;
    
    texture     texture_;
    
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
        qi_.update(clock.delta_time);
    }
    
    virtual void render(const clock_info& clock) 
    {            
        glLoadIdentity();        
        
        glEnable(GL_BLEND);
        glColor4f(0.5, 0.4, 0.6, alpha_);
        
        glScalef(alpha_+1.0f, height_, 1.0f);
        texture_.draw_at_point(point(160, 240));
        
        glDisable(GL_BLEND);
    }
};


#endif // __X2D_CUSTOM_OBJ_H__
