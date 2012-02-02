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
    
    void frame::draw_at_point(point pos) const
    {
        sprite_->draw_at_point(pos);
    }
    
    void animation::draw_at_point(point pos) const
    {
        frames_.at(cur_frame_).draw_at_point(pos);
    }
    
} // namespace anim
} // namespace x2d
