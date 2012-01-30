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
#include "resource.h"
#include "texture.h"
#include "math_util.h"

namespace x2d {
namespace base {

    class sprite
    {
    public:
        sprite(resource_ptr<texture>& t, const point& origin, const size& sz)
        : t_(t)
        , origin_(origin)
        , size_(sz)
        {            
        }
        
        void draw_at_point(point pos) const;
        
    private:
        resource_ptr<texture>   t_;
        point                   origin_;
        size                    size_;
    };
    
} // namespace base
} // namespace x2d

#endif // __X2D_SPRITE_H__
