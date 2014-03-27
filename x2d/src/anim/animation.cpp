//
//  animation.cpp
//  x2d
//
//  Created by Alex Kremer on 2/1/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "animation.h"

namespace x2d {
namespace anim {

    void animation::update(const clock_info& clock)
    {
        if(is_paused_ || is_hidden_)
        {
            return;
        }
        
        elapsed_ += clock.delta_time;
        
        if(elapsed_ >= frames_.at(cur_frame_).duration_)
        {
            // switch to next frame
            ++cur_frame_;
            if(cur_frame_ >= frames_.size())
            {
                // restart
                cur_frame_ = 0;
            }
            
            elapsed_ = 0.0f;
        }
    }
    
    void animation::flip_x(bool flag)
    {
        flip_x_ = flag;
    }
    
    void animation::flip_y(bool flag)
    {
        flip_y_ = flag;
    }

    void animation::pivot(const glm::vec2& p)
    {
        pivot_ = p;
    }
    
    const glm::vec2& animation::pivot() const
    {
        return pivot_;
    }
    
    void animation::hide()
    {
        is_hidden_ = true;
    }
    
    void animation::show()
    {
        is_hidden_ = false;
    }
    
    void frame::draw() const
    {
        sprite_->draw();
    }
    
    void animation::draw() const
    {
        if(is_hidden_)
        {
            return;
        }
        
        glPushMatrix();
        glTranslatef(-pivot_.x, -pivot_.y, 0.0f);
        
        if(flip_x_)
        {
            glScalef(-1.0f, 1.0f, 1.0f);
        }
        
        if(flip_y_)
        {
            glScalef(1.0f, -1.0f, 1.0f);
        }
        
        frames_.at(cur_frame_).draw();
        
        glPopMatrix();
    }
    
} // namespace anim
} // namespace x2d
