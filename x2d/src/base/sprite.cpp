//
//  sprite.cpp
//  x2d
//
//  Created by Alex Kremer on 1/29/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "sprite.h"
#include "graphics_engine.h"

namespace x2d {
namespace base {

    void sprite::hide()
    {
        is_hidden_ = true;
    }
    
    void sprite::show()
    {
        is_hidden_ = false;
    }
    
    void sprite::flip_x(bool flag)
    {
        flip_x_= flag;
    }
    
    void sprite::flip_y(bool flag)
    {
        flip_y_ = flag;
    }
    
    void sprite::pivot(const glm::vec2& p)
    {
        pivot_ = p;
    }

    const glm::vec2& sprite::pivot() const
    {
        return pivot_;
    }
    
    void sprite::draw() const
    {
        if(is_hidden_)
        {
            return;
        }
        
        glPushMatrix();
        glTranslatef(-pivot_.x, -pivot_.y, 0.0f);
                
        if(flip_x_)
            glScalef(-1.0f, 1.0f, 1.0f);
        if(flip_y_)
            glScalef(1.0f, -1.0f, 1.0f);
        
        float uv_width =  size_.width / t_->width_;
        float uv_height = size_.height / t_->height_;
        
        float x_off = origin_.x / t_->width_;
        float y_off = origin_.y / t_->height_;

        // 14/512 = width of sprite 0.027343
        // 32/512 = offset_x 0.0625
        // 0.0625 + 0.027343 = 0.089843
        // 512 * 0.089843 = 46
        
        float width_off = x_off + uv_width;
        float height_off = y_off + uv_height;

        float coordinates[] = 
        {
            x_off,        height_off,
            width_off,    height_off,
            x_off,        y_off,
            width_off,    y_off
        };     
        
        float width = size_.width;
        float height = size_.height;

        float vertices[] =
        {      
            -width / 2,     -height / 2,    0.0,
            width / 2,      -height / 2,    0.0,
            -width / 2,     height / 2,     0.0,
            width / 2,      height / 2,     0.0
        };

        glEnable(GL_BLEND);
        
        graphics_engine::instance().bind_texture(t_->name_);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glDisable(GL_BLEND);        
        glPopMatrix();
    }

} // namespace base
} // namespace x2d