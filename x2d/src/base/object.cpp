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
    : name("_no_name_") // can't really happen if you use x2d config system
    , position("", glm::vec3(0.0f,0.0f,0.0f))
    , scale("", 0.0f)
    , rotation("", 0.0f)
    , box(size(1.0f, 1.0f)) // TODO: other defaults? maybe size of sprite if any?
    , pivot(glm::vec2(0.0f, 0.0f)) // center in world space
    , has_bgcolor(false)
    , bgcolor(color_info(0.0f, 0.0f, 0.0f, 0.0f))
    , want_screen_touch_input(false)
    , want_world_touch_input(true)
    , want_accelerometer_input(false)
    , visible(true)
    , has_animation(false)
    , has_sprite(false)
    , has_spawner(false)
    , has_zone(false)
    , has_text(false)
    , align(LEFT_ALIGN)
    , has_body(false)
    , camera("camera") // FIXME
    , obj_space(WORLD_SPACE)
    , par_space(PARENT_SPACE_NONE) // all in world space by default
    , has_parent(false)
    {            
    }
    
    object::object(kernel& k, config::configuration& c, const object_traits& t)
    : base_object(k)
    , config_(c)
    , name_(t.name)
    , camera_(config_.get_object<camera>(t.camera))
    , space_(t.obj_space)
    , parent_space_(t.par_space)
    , position_(t.position.get(c))
    , camera_space_position_(t.position.get(c))
    , scale_(t.scale.get(c))
    , rotation_(t.rotation.get(c))    
    , box_(t.box)
    , camera_space_box_(t.box)
    , pivot_(t.pivot)
    , camera_space_pivot_(t.pivot)
    , bgcolor_(t.bgcolor)
    , align_(t.align)
    , is_visible_(t.visible)
    , parent_(NULL)
    , has_parent_(t.has_parent)
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
        
        if(t.has_animation || t.has_sprite || t.has_bgcolor || t.has_text)
        {
            if(!has_parent_)
            {
                LOG("Connecting render: %f", position_.z);
                connect_render(position_.z, space_ == CAMERA_SPACE);
            }
        }
        
        if(t.has_animation)
        {
            cur_animation_ = config_.create_sys_object<animation>(t.animation);
            if(!has_parent_)
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
        if(space_ == CAMERA_SPACE && !has_parent_)
        {
            kernel_.add_camera_space_object(this);
        }
        
        // add physics body if available
        if(t.has_body) 
        {
            body_ = config_.create_sys_object_1<body>(t.body, *this);
            
            if(!has_parent_)
            {
                connect_update(); // physics require updating
            }
        }
        
        // add children
        for(std::vector<std::string>::const_iterator it = t.children.begin(); it != t.children.end(); ++it)
        {
            add_child( config_.create_object(*it) );
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
        
    void object::update(const clock_info& clock) 
    { 
        // physics
        if(body_) 
        {
            position(body_->position());
            rotation( glm::degrees(body_->angle()) );
        }
        
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
        if(!is_visible_)
            return;
        
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
            glTranslatef(-pivot_.x, -pivot_.y, 0.0f);
            
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

    void object::reposition_in_parent_space(const size& b)
    {
        // only need to calculate box if working in parent space
        if(parent_space_ == PARENT_SPACE_BOTH || parent_space_ == PARENT_SPACE_BOX)
        {
            box_ = size( camera_space_box_.width * b.width, 
                         camera_space_box_.height * b.height);
        }

        // position is 0.0,0.0 == left-bottom corner; 1.0,1.0 == right-top corner. 0.5,0.5 = center
        if(parent_ && 
           (parent_space_ == PARENT_SPACE_BOTH || parent_space_ == PARENT_SPACE_POSITION))
        {            
            position( glm::vec2(-(b.width/2) + camera_space_position_.x * b.width, 
                                -(b.height/2) + camera_space_position_.y * b.height) );
        } 
        else
        {
            position( glm::vec2(camera_space_position_.x * b.width, 
                                camera_space_position_.y * b.height) );            
        }
        
        if(parent_space_ == PARENT_SPACE_BOTH || parent_space_ == PARENT_SPACE_BOX)
        {
            glm::vec2 pi( camera_space_pivot_.x * box_.width, camera_space_pivot_.y * box_.height );
            pivot_ = glm::vec2(-(box_.width/2)+pi.x, -(box_.height/2)+pi.y);
        }
        
        // and all children relative to their parent
        for(std::vector< boost::shared_ptr<object> >::iterator it = children_.begin(); 
            it != children_.end(); ++it)
        {
            (*it)->reposition_in_parent_space(box_);
        }
    }
    
    boost::shared_ptr<object> object::child_by_name(const std::string& n)
    {
        for(std::vector< boost::shared_ptr<object> >::iterator it = children_.begin(); 
            it != children_.end(); ++it)
        {
            if((*it)->name_ == n)
                return *it;                        
        }
        
        // TODO: custom exception?
        throw std::runtime_error("Couldn't find child by name: '" + n + "'");
    }
    
    void object::add_child(const boost::shared_ptr<object>& child)
    {
        child->set_parent(this);
        children_.push_back(child);
        
        if(!has_parent_)
        {
            // Now connect update and render if we did not before.
            // Need these connections to update children.
            connect_update();

            LOG("Connecting render: %f", position_.z);
            connect_render(position_.z, space_ == CAMERA_SPACE);
        }
    }
    
    void object::release(object& obj)
    {
        if(spawner_)
        {
            spawner_->release(obj);
        }
    }
    
    const glm::vec3 object::position() const
    {
        return position_;
    }
    
    void object::position(const glm::vec3& p)        
    {
        position_ = p;

        if(body_) 
        {
            body_->position(glm::vec2(position_.x, position_.y));
        }
    }
    
    void object::position(const glm::vec2& p)        
    {
        position_.x = p.x;
        position_.y = p.y;
        // retain z value
        
        if(body_) 
        {
            body_->position(p);
        }
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
    
    const float object::rotation() const
    {
        return rotation_;
    }
    
    void object::rotation(float a)        
    {
        rotation_ = a;
    }
    
    void object::scale(float s)
    {
        scale_ = s;
    }
    
    void object::pivot(const glm::vec2& p)
    {
        if(parent_space_ == PARENT_SPACE_BOTH || parent_space_ == PARENT_SPACE_BOX)
        {
            camera_space_pivot_ = p;
        }
        else
        {
            pivot_ = p;
        }
    }
    
    void object::box(const size& s)
    {
        if(parent_space_ == PARENT_SPACE_BOTH || parent_space_ == PARENT_SPACE_BOX)
        {
            camera_space_box_ = s;
        }
        else
        {
            box_ = s;
        }
    }
    
    const std::string object::text() const
    {
        return *text_;
    }
    
    void object::text(const std::string& t)
    {
        text_ = boost::shared_ptr<std::string>(new std::string(t) );
    }
    
    const glm::vec3& object::camera_space_position() const
    {
        return camera_space_position_;
    }
    
    const size object::box() const
    {        
        if(box_.width != 0 && box_.height != 0) 
        {
            LOG("Return box_ as box() of object: %f %f", 
                box_.width * scale_, box_.height * scale_);
            
            return box_ * scale_;
        }
        else if(cur_sprite_)
        {
            LOG("Return cur_sprite_->box() as box() of object: %f %f", 
                cur_sprite_->box().width * scale_, 
                cur_sprite_->box().height * scale_);
            
            return cur_sprite_->box() * scale_;
        }
        else if(cur_animation_)
        {
            LOG("Return cur_animation_->box() as box() of object: %f %f", 
                cur_animation_->box().width * scale_, 
                cur_animation_->box().height * scale_);
            
            return cur_animation_->box() * scale_;
        }
        
        return size(); // return default
    }
    
    const size object::camera_space_box() const
    {
        return camera_space_box_;
    }
    
    const glm::vec2 object::pivot() const
    {
        return pivot_;
    }
    
    const glm::vec2 object::camera_space_pivot() const
    {
        return camera_space_pivot_;
    }
    
    void object::set_sprite(const boost::shared_ptr<sprite>& spr)
    {
        cur_sprite_ = spr;
    }
    
    void object::visible(bool v)
    {
        is_visible_ = v;
    }
    
    void object::on_collision_begin(object* with)
    {
        LOG("Object collision BEGIN: 0x%X with 0x%X", this, with);
    }

    void object::on_collision_end(object* with)
    {
        LOG("Object collision END: 0x%X with 0x%X", this, with);        
    }

} // namespace x2d