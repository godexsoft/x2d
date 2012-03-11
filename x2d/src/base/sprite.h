//
//  sprite.h
//  x2d
//
//  Created by Alex Kremer on 1/29/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_SPRITE_H__
#define __X2D_SPRITE_H__

#include "math_util.h"
#include "texture.h"
#include "math_util.h"
#include "glm.hpp"

namespace x2d {
namespace base {

    /**
     * @brief Represents a sprite
     */
    class sprite
    {
    public:
        /**
         * Create a sprite from a texture
         * @param[in] t         Texture to use
         * @param[in] origin    Offset inside texture
         * @param[in] sz        Sprite size
         * @param[in] pivot     The pivot
         * @param[in] flip_x    Sets flipping on x axis
         * @param[in] flip_y    Sets flipping on y axis
         */
        sprite(const boost::shared_ptr<texture>& t, 
               const point& origin, 
               const size& sz, 
               const glm::vec2& pivot=glm::vec2(0,0),
               bool flip_x=false,
               bool flip_y=false
        )
        : t_(t)
        , origin_(origin)
        , size_(sz)
        , pivot_(pivot)
        , flip_x_(flip_x)
        , flip_y_(flip_y)
        {            
        }
        
        /**
         * Draw the sprite
         */
        void draw() const;
        
    private:
        boost::shared_ptr<texture>   t_;
        point                        origin_;
        size                         size_;
        glm::vec2                    pivot_;
        bool                         flip_x_;
        bool                         flip_y_;
    };
    
} // namespace base
} // namespace x2d

#endif // __X2D_SPRITE_H__
