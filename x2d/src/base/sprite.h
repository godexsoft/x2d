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
         */
        sprite(const boost::shared_ptr<texture>& t, 
               const point& origin, 
               const size& sz, 
               const glm::vec2& pivot=glm::vec2(0,0)
        )
        : t_(t)
        , origin_(origin)
        , size_(sz)
        , pivot_(pivot)
        {            
        }
        
        /**
         * @param[in] pos  Location to draw the sprite
         */
        void draw_at_point(point pos) const;
        
    private:
        boost::shared_ptr<texture>   t_;
        point                        origin_;
        size                         size_;
        glm::vec2                    pivot_;
    };
    
} // namespace base
} // namespace x2d

#endif // __X2D_SPRITE_H__
