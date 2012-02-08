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

    object_traits::object_traits()
    : position("", vector_2d(0.0f,0.0f))
    , scale("", 0.0f)
    , rotation("", 0.0f)
    , want_screen_touch_input(false)
    , want_world_touch_input(true)
    , want_accelerometer_input(false)
    , has_animation(false)
    , has_sprite(false)
    , obj_space(WORLD_SPACE)
    , has_parent(false)
    {            
    }
    
    object::object(kernel& k, config::configuration& c, const object_traits& t)
    : base_object(k)
    , config_(c)
    , space_(t.obj_space)
    , position_(t.position.get(c))
    , camera_space_position_(t.position.get(c))
    , scale_(t.scale.get(c))
    , camera_space_scale_(t.scale.get(c))
    , rotation_(t.rotation.get(c))    
    , camera_space_rotation_(t.rotation.get(c))
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
        
        if(t.has_animation || t.has_sprite)
        {
            if(!t.has_parent)
            {
                connect_render();
            }
        }
        
        if(t.has_animation)
        {
            cur_animation_ = config_.create_sys_object<animation>(t.animation);
            if(!t.has_parent)
            {
                connect_update(); // animation needs updating
            }
        }
        
        if(t.has_sprite)
        {
            cur_sprite_ = config_.get_object<sprite>(t.sprite);
        }
    }
    
    void object::update(const clock_info& clock) 
    { 
        if(cur_animation_)
        {
            cur_animation_->update(clock);
        }
        
        // update all children
        std::for_each(children_.begin(), children_.end(), 
            boost::bind(&object::update, _1, clock));
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
        
        if(cur_sprite_)
        {
            cur_sprite_->draw_at_point(point(0, 0));
        }
        
        // draw all children in current space (relative to this object)
        std::for_each(children_.begin(), children_.end(), 
            boost::bind(&object::render, _1, clock));
        
        glPopMatrix();        
        
        glColor4f(1, 1, 1, 1);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);   
        glDisable(GL_BLEND);
    }


} // namespace x2d