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
    : position("", glm::vec3(0.0f,0.0f,0.0f))
    , scale("", 0.0f)
    , rotation("", 0.0f)
    , box(size(10.0f, 10.0f)) // TODO: other defaults
    , bgcolor(color_info(0.0f, 0.0f, 0.0f, 0.0f))
    , want_screen_touch_input(false)
    , want_world_touch_input(true)
    , want_accelerometer_input(false)
    , has_animation(false)
    , has_sprite(false)
    , has_spawner(false)
    , has_zone(false)
    , has_text(false)
    , align(LEFT_ALIGN)
    , camera("camera") // FIXME
    , obj_space(WORLD_SPACE)
    , has_parent(false)
    {            
    }
    
    object::object(kernel& k, config::configuration& c, const object_traits& t)
    : base_object(k)
    , config_(c)
    , camera_(config_.get_object<camera>(t.camera))
    , space_(t.obj_space)
    , position_(t.position.get(c))
    , camera_space_position_(t.position.get(c))
    , scale_(t.scale.get(c))
    , rotation_(t.rotation.get(c))    
    , box_(t.box)
    , bgcolor_(t.bgcolor)
    , align_(t.align)
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
        
        if(t.has_animation || t.has_sprite || t.has_text)
        {
            if(!t.has_parent)
            {
                LOG("Connecting render: %f", position_.z);
                connect_render(position_.z, space_ == CAMERA_SPACE);
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
        
        if(t.has_spawner)
        {
            spawner_ = config_.create_sys_object<spawner>(t.spawner);
            spawner_->set_parent(this); // the spawner will live while this object lives
        }
        
        if(t.has_zone)
        {
            zone_ = config_.get_object<zone>(t.zone);
            zone_->set_parent(this); // moves along the object
        }
        
        if(t.has_text)
        {
            cur_font_ = config_.get_object<font>(t.font);
            text_ = boost::shared_ptr<std::string>( new std::string(t.text) );
        }
    
        // check space and add object to list of camera space objects
        if(space_ == CAMERA_SPACE && !t.has_parent)
        {
            kernel_.add_camera_space_object(this);
        }
        
        // populate contexts and register with them
        for(std::vector<std::string>::const_iterator it = t.contexts.begin(); it != t.contexts.end(); ++it)
        {
            LOG("Getting context with key '%s'", (*it).c_str());
            boost::shared_ptr<context> ctx = config_.get_object<context>( *it );

            ctx->reg_object(this);
            contexts_.push_back( ctx );            
        }
    }
    
    object::~object()
    {
        LOG("Removing object from everywhere");
        
        // unregister object from all contexts it's in atm.
        for(std::vector<boost::shared_ptr<context> >::const_iterator it = contexts_.begin(); it != contexts_.end(); ++it)
        {
            (*it)->unreg_object(this);            
        }
        
        kernel_.remove_camera_space_object(this);
    }
    
    const glm::vec3 object::world_position() const
    {
        // TODO: if we are in camera-space then we must calculate using the camera_
        if(space_ == CAMERA_SPACE)
        {
            if(parent_)
                return parent_->world_position() + camera_->to_world(camera_space_position_);

            return camera_->to_world(camera_space_position_);
        }
        else 
        {        
            if(parent_)
                return parent_->world_position() + position_;
            
            return position_;
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
        glPushMatrix();
        glTranslatef(position_.x, position_.y, 0.0f);
        
        if(rotation_ != 0.0f)
        {
            glRotatef(rotation_, 0, 0, 1);
        }
        
        if(scale_ != 0.0f)
        {
            glScalef(scale_, scale_, 1.0f);
        }
        
        // draw all children which are behind the parent 
        // in current space (relative to this object)
        std::for_each(std::find_if(children_.begin(), children_.end(), 
            boost::bind(&glm::vec3::z, boost::bind(&object::position_, _1)) > position_.z ), children_.end(), 
                boost::bind(&object::render, _1, clock) );
       
        // draw the rect with bgcolor if required
        if(bgcolor_.a > 0.0f)
        {
            glDisable(GL_TEXTURE_2D);
            glColor4f(bgcolor_.r, bgcolor_.g, bgcolor_.b, bgcolor_.a);
            glPushMatrix();
            
            float vertices[] =
            {      
                -box_.width / 2,     -box_.height / 2,    0.0,
                box_.width / 2,      -box_.height / 2,    0.0,
                -box_.width / 2,     box_.height / 2,     0.0,
                box_.width / 2,      box_.height / 2,     0.0
            };
            
            glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            glVertexPointer(3, GL_FLOAT, 0, vertices);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glDisable(GL_BLEND);        
            glPopMatrix();
            glEnable(GL_TEXTURE_2D);
        }
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0, 1.0, 1.0, 1.0);

        if(cur_animation_)
        {
            cur_animation_->draw();
        }
        
        if(cur_sprite_)
        {
            cur_sprite_->draw();
        }
        
        if(text_)
        {
            cur_font_->print(*text_, align_);
        }

        glColor4f(1, 1, 1, 1);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);   
        glDisable(GL_BLEND);
        
        // draw all children which are in front of the parent 
        // in current space (relative to this object)
        std::for_each(std::find_if(children_.begin(), children_.end(), 
            boost::bind(&glm::vec3::z, boost::bind(&object::position_, _1)) <= position_.z ), children_.end(), 
                boost::bind(&object::render, _1, clock) );        
        
        glPopMatrix();        
    }

    void object::position_children_camera_space()
    {
        for(std::vector< boost::shared_ptr<object> >::iterator it = children_.begin(); 
            it != children_.end(); ++it)
        {
            // position is 0.0,0.0 == left-bottom corner; 1.0,1.0 == right-top corner. 0.5,0.5 = center
            glm::vec4 pp( (*it)->camera_space_position().x * box_.width, (*it)->camera_space_position().y * box_.height, 0.0f, 1.0f ); // w = 1        
            (*it)->position( glm::vec2(-(box_.width/2)+pp.x, -(box_.height/2)+pp.y) );
        }
    }
    
} // namespace x2d