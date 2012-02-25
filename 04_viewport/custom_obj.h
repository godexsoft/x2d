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

#include "base_object.h"

#include "interpolator.h"
#include "sprite.h"
#include "configuration.h"
#include "animation.h"
#include "space.h"

class custom_obj 
: public base_object
{
public:
    custom_obj(kernel& k, configuration& conf)
    : base_object(k)
    , x_(0.0f)
    , qi_(x_, -50.0f, 100.0f, -50.0f, 3.0f)
    , position_(0,0,0)
    {        
        connect_update();
        connect_render(position_.z);  
        
        connect_touch_input(SCREEN_SPACE);
        connect_touch_input(WORLD_SPACE);
        
        anim_ = conf.get_object<animation>("animations.player");        
        anim_->start();
    }
    
private:
    boost::shared_ptr<animation> anim_;
    float                   x_;
    quadratic_interpolator  qi_;
    
    glm::vec3               position_;
        
protected:
    
    virtual void update(const clock_info& clock) 
    { 
        anim_->update(clock);
        qi_.update(clock.delta_time);
    }
    
    virtual void render(const clock_info& clock) 
    {  
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glColor4f(1.0, 1.0, 1.0, 1.0);
        
        glPushMatrix();
        glTranslatef(position_.x, position_.y, 0.0f);
        anim_->draw_at_point(point(0, 0));
        glPopMatrix();        

        glColor4f(1, 1, 1, 1);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);   
        glDisable(GL_BLEND);
    }
    
    virtual void touch_input_began(space s, const std::vector<touch>& touches) 
    {
        if(s == WORLD_SPACE)
        {
            LOG("NUM WORLD TOUCHES: %d", touches.size());
            position_ = glm::vec3( touches.at(0).location().x, touches.at(0).location().y, 0.0f );
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
            position_ = glm::vec3( touches.at(0).location().x, touches.at(0).location().y, 0.0f );
        }
    }

};


#endif // __X2D_CUSTOM_OBJ_H__
