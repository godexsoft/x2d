//
//  animation.h
//  x2d
//
//  Created by Alex Kremer on 2/1/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_ANIMATION_H__
#define __X2D_ANIMATION_H__

#include <vector>
#include <boost/shared_ptr.hpp>

#include "sprite.h"
#include "math_util.h"
#include "clock.h"

namespace x2d {
namespace anim {
        
    /**
     * @brief This class represents a single frame in an animation
     */
    class frame
    {
    public:
        frame( const boost::shared_ptr<sprite>& spr )
        : sprite_(spr)
        {            
        }
        
        void draw_at_point(point pos) const;
        
    private:
        boost::shared_ptr<sprite> sprite_;
    };
    
    typedef std::vector<frame> frames_vec;
    
    /**
     * @brief This class provides basic animation support
     */
    class animation
    {
    public:
        animation(float d)
        : duration_(d)
        , is_paused_(false)
        , cur_frame_(0)
        , elapsed_(0.0f)
        {            
        }
        
        void add(const frame& f)
        {
            frames_.push_back(f);
        }
        
        void start()
        {
            cur_frame_ = 0;
            elapsed_ = 0.0f;
            is_paused_ = false;
        }
        
        void pause()
        {
            is_paused_ = true;            
        }
        
        void resume()
        {
            is_paused_ = false;
        }
        
        void update(const clock_info& clock);
        void draw_at_point(point pos) const;
        
    private:
        float       duration_; // default duration
        bool        is_paused_;
        
        frames_vec  frames_;
        int         cur_frame_;
        
        float       elapsed_;
    };
    
} // namespace anim
} // namespace x2d
using namespace x2d::anim;

#endif // __X2D_ANIMATION_H__
