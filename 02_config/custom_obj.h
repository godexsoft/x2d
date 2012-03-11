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
    custom_obj(kernel& k, configuration& conf, const std::string& key)
    : base_object(k)
    , spr_( conf.get_object<sprite>(key) )
    , alpha_(0.0f)
    , li_(alpha_, 0.0f, 1.0f, 1.0f)
    , pos_( conf.get_value<float>("values.x"), conf.get_value<float>("values.y"), 0.0f ) // use random
    , velocity_( conf.get_value<float>("values.vel"), conf.get_value<float>("values.vel"), 0.0f ) // use random
    {        
        connect_update();
        connect_render(pos_.z);     
    }
    
private:
    boost::shared_ptr<sprite> spr_;
    
    float                     alpha_;
    linear_interpolator       li_;
    
    glm::vec3                 pos_;
    glm::vec3                 velocity_;
    
protected:
    
    virtual void update(const clock_info& clock) 
    { 
        li_.update(clock.delta_time);
        
        pos_ += velocity_ * (float)clock.delta_time;
        
        if(pos_.x >= 160.0f && velocity_.x>0)
            velocity_.x = -velocity_.x;
        if(pos_.x <= -160.0f && velocity_.x<0)
            velocity_.x = -velocity_.x;
        
        if(pos_.y >= 240.0f && velocity_.y>0)
            velocity_.y = -velocity_.y;
        if(pos_.y <= -240.0f && velocity_.y<0)
            velocity_.y = -velocity_.y;
    }
    
    virtual void render(const clock_info& clock) 
    {  
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glColor4f(1.0, 1.0, 1.0, alpha_);
        
        glPushMatrix();
        glTranslatef(pos_.x, pos_.y, 0);
        glRotatef(alpha_*360.0f, 0, 0, 1);
        
        spr_->draw();
        
        glPopMatrix();        

        glColor4f(1, 1, 1, 1);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);   
        glDisable(GL_BLEND);
    }
};


#endif // __X2D_CUSTOM_OBJ_H__
