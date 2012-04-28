//
//  camera.cpp
//  x2d
//
//  Created by Alex Kremer on 2/7/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "camera.h"
#include "object.h"

namespace x2d {
namespace base {

    void camera::position(const glm::vec2& v)
    {
        position_ = v;
    }
    
    const glm::vec2 camera::position() const
    {
        return position_;
    }
    
    void camera::rotation(float angle)
    {
        if(angle < 0.0f) 
        {
            angle = 360.0f + angle;    
        }
        
        rotation_ = glm::clamp(angle, 0.0f, 360.0f);
    }
    
    const float camera::zoom() const
    {
        return zoom_;
    }
    
    void camera::zoom(float z)
    {
        zoom_ = z;
    }
    
    const size camera::frustum() const
    {
        return frustum_;
    }
    
    const point camera::inverted_transformation(const point& p)
    {
        glm::vec4 pp(p.x, p.y, 0.0f, 1.0f); // w = 1
        
        // save us from matrix inverse
        glm::mat4 m(1.0f);
        
        if(position_.x != 0.0f || position_.y != 0.0f)
            m = glm::translate(m, glm::vec3(-position_.x, -position_.y, 0.0f));

        if(zoom_ != 1.0f)
            m = glm::scale(m, glm::vec3(1.0f/zoom_, 1.0f/zoom_, 1.0f));

        if(rotation_ != 0.0f)
            m = glm::rotate(m, 360.0f-rotation_, glm::vec3(0,0,1));

        m = glm::translate(m, glm::vec3(-frustum_.width/2.0f, -frustum_.height/2.0f, 0.0f) );

        pp = m * pp;    
        return point(pp.x, pp.y);
    }
    
    const glm::vec3 camera::to_world(const glm::vec3& p)
    {            
        glm::vec4 pp( p.x * frustum_.width, p.y * frustum_.height, 0.0f, 1.0f );
        
        // save us from matrix inverse
        glm::mat4 m(1.0f);
        
        if(position_.x != 0.0f || position_.y != 0.0f)
            m = glm::translate(m, glm::vec3(-position_.x, -position_.y, 0.0f));

        if(zoom_ != 1.0f)
            m = glm::scale(m, glm::vec3(1.0f/zoom_, 1.0f/zoom_, 1.0f));
                
        if(rotation_ != 0.0f)
            m = glm::rotate(m, 360.0f-rotation_, glm::vec3(0,0,1));

        m = glm::translate(m, glm::vec3(-frustum_.width/2.0f, -frustum_.height/2.0f, 0.0f) );
        
        pp = m * pp;            
        return glm::vec3(pp.x, pp.y, p.z);
    }
    
    void camera::apply()
    {
        if(is_applied)
            return;
        
        glLoadIdentity();

        // move to center of screen
        glTranslatef(frustum_.width / 2.0f, 
                     frustum_.height / 2.0f, 0.0);
        
        // rotate about center of screen
        if(rotation_ != 0.0f)
        {
            glRotatef(rotation_, 0, 0, 1);
        }
        
        // zoom scene
        if(zoom_ != 1.0f)
        {
            glScalef(zoom_, zoom_, 1.0);
        }
        
        // move to camera position
        glTranslatef(position_.x, position_.y, 0.0);
        
        is_applied = true;
    }
    
    void camera::calculate_in_screen(object* o)
    {
        // position is 0.0,0.0 == left-bottom corner; 1.0,1.0 == right-top corner. 0.5,0.5 = center
        glm::vec4 pp( o->camera_space_position().x * frustum_.width, o->camera_space_position().y * frustum_.height, 0.0f, 1.0f ); // w = 1        
        o->position( glm::vec2(pp.x, pp.y) );
        
        // also calculate all children which happen to be camera-spaced
        // their frustum is their parent object's box
        o->position_children_camera_space();
    }

} // namespace base
} // namespace x2d