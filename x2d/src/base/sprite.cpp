//
//  sprite.cpp
//  x2d
//
//  Created by Alex Kremer on 1/29/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "sprite.h"

namespace x2d {
namespace base {

    void sprite::draw_at_point(point pos) const
    {
        glPushMatrix();
        glTranslatef(pos.x, pos.y, 0.0f);
        
        float uv_width =  (size_.width * t_->max_s_)  / (t_->size_.width * t_->max_s_);
        float uv_height = (size_.height * t_->max_t_) / (t_->size_.height * t_->max_t_);
        
        float y_off = (origin_.y * t_->max_s_) / (t_->size_.width * t_->max_s_);
        float x_off = (origin_.x * t_->max_t_) / (t_->size_.height * t_->max_t_);
        
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
        
        glBindTexture(GL_TEXTURE_2D, t_->name_);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glDisable(GL_BLEND);        
        glPopMatrix();
    }

} // namespace base
} // namespace x2d