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
    
    void frame::draw() const
    {        
        sprite_->draw();
    }
    
    void animation::draw() const
    {
        glPushMatrix();
        glTranslatef(-pivot_.x, -pivot_.y, 0.0f);
        
        if(flip_x_)
            glScalef(-1.0f, 1.0f, 1.0f);
        if(flip_y_)
            glScalef(1.0f, -1.0f, 1.0f);
        
        frames_.at(cur_frame_).draw();
        
        glPopMatrix();
    }
    
} // namespace anim
} // namespace x2d
