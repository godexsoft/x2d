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
#include "glm.hpp"
#include "clock.h"

namespace x2d {
namespace anim {
        
    class animation;
    
    /**
     * @brief This class represents a single frame in an animation
     */
    class frame
    {
        friend class animation;
        
    public:
        /**
         * @param[in] spr   The sprite to use for this frame
         * @param[in] dur   Duration of this frame expressed in seconds
         */
        frame( const boost::shared_ptr<sprite>& spr, const float& dur )
        : sprite_(spr)
        , duration_(dur)
        {            
        }
        
        /**
         * Draw the frame.
         */
        void draw() const;
        
        /**
         * @return frame size
         */
        const size box() const
        {
            return sprite_->box();
        }
        
    private:
        boost::shared_ptr<sprite> sprite_;
        float                     duration_;
    };
    
    typedef std::vector<frame> frames_vec;
    
    /**
     * @brief This class provides basic animation support
     */
    class animation
    {
    public:
        /**
         * @param[in] d Default duration for each frame in animation
         * @param[in] pivot The pivot for the animation
         * @param[in] flip_x Sets flipping on x axis
         * @param[in] flip_y Sets flipping on y axis
         */
        animation(float d, const glm::vec2& pivot=glm::vec2(0,0), bool flip_x=false, bool flip_y=false)
        : duration_(d)
        , pivot_(pivot)
        , flip_x_(flip_x)
        , flip_y_(flip_y)
        , is_paused_(false)
        , is_hidden_(false)
        , cur_frame_(0)
        , elapsed_(0.0f)
        {            
        }
        
        /**
         * Add a frame to the animation
         * @param[in] f Frame to add
         */
        void add(const frame& f)
        {
            frames_.push_back(f);
        }
        
        /**
         * Start the animation at first frame
         */
        void start()
        {
            reset();
            is_paused_ = false;
        }
        
        /**
         * Reset animation states
         */
        void reset()
        {
            cur_frame_ = 0;
            elapsed_ = 0.0f;
        }
        
        /**
         * Freeze the animation
         */
        void pause()
        {
            is_paused_ = true;            
        }
        
        /**
         * Resume animation after freeze
         */
        void resume()
        {
            is_paused_ = false;
        }
        
        void hide();
        void show();
        
        void flip_x(bool flag);
        
        void flip_y(bool flag);
        
        void pivot(const glm::vec2& p);
        const glm::vec2& pivot() const;
        
        /**
         * Update animation with given clock info
         * @param[in] clock  The clock info
         */
        void update(const clock_info& clock);
        
        /**
         * Draw the current frame of the animation
         */
        void draw() const;
        
        /**
         * @return size of current frame
         */
        const size box() const
        {
            return frames_.at(cur_frame_).box();
        }
        
    private:
        float       duration_; // default duration
        glm::vec2   pivot_;
        bool        flip_x_;
        bool        flip_y_;

        bool        is_paused_;
        bool        is_hidden_;
        
        frames_vec  frames_;
        int         cur_frame_;
        
        float       elapsed_;
    };
    
} // namespace anim
} // namespace x2d
using namespace x2d::anim;

#endif // __X2D_ANIMATION_H__
