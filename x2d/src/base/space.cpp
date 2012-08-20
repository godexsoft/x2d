//
//  space.cpp
//  x2d
//
//  Created by Alex Kremer on 20/8/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//
/*
#include "space.h"

namespace x2d {
namespace base {

    spatial_info::spatial_info()
        : space_(t.obj_space)
        , parent_space_(t.par_space)
        , position_(t.position.get(c))
        , camera_space_position_(t.position.get(c))
        , scale_(t.scale.get(c))
        , rotation_(t.rotation.get(c))
        , box_(t.box)
        , camera_space_box_(t.box)
        , pivot_(t.pivot)
        , camera_space_pivot_(t.pivot)
    {        
    }
    
    const glm::mat4 spatial_info::final_transform() const
    {
        // TODO: make this a member and cache until invalidated by position/scale/rotation etc.
        glm::mat4 transform_(1.0f);
        
        if(position_.x != 0.0f || position_.y != 0.0f)
            transform_ = glm::translate(transform_, glm::vec3(position_.x, position_.y, 0.0f));        

        if(rotation_ != 0.0f)
            transform_ = glm::rotate(transform_, rotation_, glm::vec3(0,0,1));

        if(scale_ != 1.0f)
            transform_ = glm::scale(transform_, glm::vec3(scale_, scale_, 1.0f));
   
        if(pivot_.x != 0.0f || pivot_.y != 0.0f)
            transform_ = glm::translate(transform_, glm::vec3(-pivot_.x, -pivot_.y, 0.0f));
        
        if(parent_)
            return parent_->final_transform() * transform_;
        
        return transform_;
    }

    const glm::vec3 spatial_info::position() const
    {
        return position_;
    }
    
    void spatial_info::position(const glm::vec3& p)
    {
        position_ = p;
        
        if(body_)
        {
            body_->position(glm::vec2(position_.x, position_.y));
        }
    }
    
    void spatial_info::position(const glm::vec2& p)
    {
        position_.x = p.x;
        position_.y = p.y;
        // retain z value
        
        if(body_)
        {
            body_->position(p);
        }
    }
    
    const float spatial_info::rotation() const
    {
        return rotation_;
    }
    
    void spatial_info::rotation(float a)        
    {
        rotation_ = a;
    }
    
    const float spatial_info::scale() const
    {
        return scale_;
    }
    
    void spatial_info::scale(float s)
    {
        scale_ = s;
    }
    
    void spatial_info::pivot(const glm::vec2& p)
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
    
    void spatial_info::box(const size& s)
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
    
    const glm::vec3& spatial_info::camera_space_position() const
    {
        return camera_space_position_;
    }
    
    const size spatial_info::box() const
    {        
        return box_ * scale_;
    }
    
    const size spatial_info::camera_space_box() const
    {
        return camera_space_box_;
    }
    
    const glm::vec2 spatial_info::pivot() const
    {
        return pivot_;
    }
    
    const glm::vec2 spatial_info::camera_space_pivot() const
    {
        return camera_space_pivot_;
    }
   
} // namespace base
} // namespace x2d
*/