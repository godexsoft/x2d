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

#include "interpolator.h"
#include "sprite.h"
#include "configuration.h"

class custom_obj 
: public base_object
{
public:
    custom_obj(kernel& k, configuration& conf, const std::string& key, vector_2d pos, vector_2d vel)
    : base_object(k)
    , spr_( conf.get<sprite>(key) )
    , alpha_(0.0f)
    , li_(alpha_, 0.0f, 1.0f, 1.0f)
    , pos_(pos)
    , velocity_(vel)
    {        
        connect_update();
        connect_render();        
    }
    
private:
    boost::shared_ptr<sprite> spr_;
    
    float                     alpha_;
    linear_interpolator       li_;
    
    vector_2d                 pos_;
    vector_2d                 velocity_;
    
protected:
    
    virtual void update(const clock_info& clock) 
    { 
        li_.update(clock.delta_time);
        
        pos_ += velocity_ * clock.delta_time;
        
        if(pos_.X() >= 320.0f && velocity_.X()>0)
            velocity_.X(-velocity_.X());
        if(pos_.X() <= 0.0f && velocity_.X()<0)
            velocity_.X(-velocity_.X());    
        
        if(pos_.Y() >= 480.0f && velocity_.Y()>0)
            velocity_.Y(-velocity_.Y());
        if(pos_.Y() <= 0.0f && velocity_.Y()<0)
            velocity_.Y(-velocity_.Y());
    }
    
    virtual void render(const clock_info& clock) 
    {  
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glColor4f(1.0, 1.0, 1.0, 1.0);
        
        glPushMatrix();
        glTranslatef(pos_.X(), pos_.Y(), 0);
        glRotatef(alpha_*360.0f, 0, 0, 1);
//        glScalef(1.0f-alpha_, alpha_, 1.0f);
        
        spr_->draw_at_point(point(0, 0));
        
        glPopMatrix();        

        glColor4f(1, 1, 1, 1);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);   
        glDisable(GL_BLEND);
    }
};


#endif // __X2D_CUSTOM_OBJ_H__
