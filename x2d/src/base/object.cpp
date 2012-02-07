//
//  object.cpp
//  x2d
//
//  Created by Alex Kremer on 2/5/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "object.h"
#include "configuration.h"

namespace x2d {

    object::object(kernel& k, config::configuration& c, const object_traits& t)
    : base_object(k)
    , config_(c)
    , position_(t.position)
    , scale_(t.scale)
    , rotation_(t.rotation)
    {        
        if(t.want_screen_touch_input)
        {
            connect_touch_input(SCREEN_SPACE);
        }
        
        if(t.want_world_touch_input)
        {
            connect_touch_input(WORLD_SPACE);
        }
        
        if(t.want_accelerometer_input)
        {
            // TODO accel connect
        }
        
        if(t.has_animation)
        {
            cur_animation_ = config_.get_object<animation>(t.animation);
        }
    }
    
    void object::update(const clock_info& clock) 
    { 
        if(cur_animation_)
        {
            cur_animation_->update(clock);
        }
    }
    
    void object::render(const clock_info& clock)
    {  
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glColor4f(1.0, 1.0, 1.0, 1.0);
        
        glPushMatrix();
        glTranslatef(position_.X(), position_.Y(), 0.0f);
        
        if(rotation_ != 0.0f)
        {
            glRotatef(rotation_, 0, 0, 1);
        }
        
        if(scale_ != 0.0f)
        {
            glScalef(scale_, scale_, 1.0f);
        }
        
        if(cur_animation_)
        {
            cur_animation_->draw_at_point(point(0, 0));
        }
        
        glPopMatrix();        
        
        glColor4f(1, 1, 1, 1);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);   
        glDisable(GL_BLEND);
    }


} // namespace x2d