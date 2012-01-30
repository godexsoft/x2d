//
//  custom_obj.h
//  x2d
//
//  Created by Alex Kremer on 1/30/12.
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
    // , spr_(texture_, point(38, 5), size(30, 33)) // only head of the chupakabra :-)
    , spr_(texture_, point(0, 0), size(120, 120))
    , alpha_(0.0f)
    , li_(alpha_, 0.0f, 1.0f, 1.0f)
    , pos_(160.0f, 240.0f)
    , velocity_(100.0f, 200.0f)
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
    
    vector_2d               pos_;
    vector_2d               velocity_;
    
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
        
        pos_ += velocity_ * clock.delta_time;
        
        if(pos_.X() >= 320.0f || pos_.X() <= 0.0f)
            velocity_.X(-velocity_.X());
        
        if(pos_.Y() >= 480.0f || pos_.Y() <= 0.0f)
            velocity_.Y(-velocity_.Y());
    }
    
    virtual void render(const clock_info& clock) 
    {  
        glEnable(GL_BLEND);
        if( 0 )
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        else {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        
        glColor4f(1.0, 1.0, 1.5, alpha_);
        
        glPushMatrix();
        glTranslatef(pos_.X(), pos_.Y(), 0);
        glRotatef(alpha_*360.0f, 0, 0, 1);    
        glScalef(alpha_, alpha_, 1.0f);
            
        spr_.draw_at_point(point(0, 0));

        glPopMatrix();        
        glDisable(GL_BLEND);
    }
};


#endif // __X2D_CUSTOM_OBJ_H__
