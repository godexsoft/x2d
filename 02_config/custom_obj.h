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
#include "configuration.h"

class custom_obj 
: public base_object
{
public:
    custom_obj(kernel& k, liverpool_manager lvp_man)
    : base_object(k)
    , res_man_(lvp_man)
    , config_(res_man_, "res/main.xml")
    , spr_( config_.get<sprite>("txtr.cafe") )
    , spr2_( config_.get<sprite>("txtr.house") )
    , alpha_(0.0f)
    , li_(alpha_, 0.0f, 1.0f, 1.0f)
    , pos_(160.0f, 240.0f)
    , velocity_(100.0f, 200.0f)
    {        
        connect_update();
        connect_render();        
    }
    
private:
    resource_manager          res_man_;
    configuration             config_;
    boost::shared_ptr<sprite> spr_, spr2_;
    
    float                     alpha_;
    linear_interpolator       li_;
    
    vector_2d                 pos_;
    vector_2d                 velocity_;
    
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
        
        /*
        pos_ += velocity_ * clock.delta_time;
        
        if(pos_.X() >= 320.0f || pos_.X() <= 0.0f)
            velocity_.X(-velocity_.X());
        
        if(pos_.Y() >= 480.0f || pos_.Y() <= 0.0f)
            velocity_.Y(-velocity_.Y());
*/
    }
    
    virtual void render(const clock_info& clock) 
    {  
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glColor4f(1.0, 1.0, 1.0, alpha_);
        
        glPushMatrix();
        glTranslatef(pos_.X(), pos_.Y(), 0);
        glScalef(1.0f-alpha_, alpha_, 1.0f);
        
        glTranslatef(100, 100, 0);
        
        spr2_->draw_at_point(point(0, 0));
        
        glPopMatrix();        

        glColor4f(1, 1, 1, 1);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);   
        glDisable(GL_BLEND);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glColor4f(1.0, 1.0, 1.0, 1.0f-alpha_);
        spr_->draw_at_point(point(210, 170));
        
        glColor4f(1, 1, 1, 1);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);   
        glDisable(GL_BLEND);

    }
};


#endif // __X2D_CUSTOM_OBJ_H__
