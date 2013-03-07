//
//  color_info.h
//  x2d
//
//  Created by Alex Kremer on 2/2/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_COLOR_INFO_H__
#define __X2D_COLOR_INFO_H__

namespace x2d {
namespace base {
        
    /**
     * @brief Color information RGBA
     */
    struct color_info
    {        
        color_info(float rr, float gg, float bb, float aa=1.0f)
        : r(rr)
        , g(gg)
        , b(bb)
        , a(aa)
        {            
        }
        
        color_info()
        : r(0.0f)
        , g(0.0f)
        , b(0.0f)
        , a(0.0f)
        {
        }
        
        float r, g, b, a;
    };
    
} // namespace base
} // namespace x2d
using namespace x2d::base;

#endif // __X2D_COLOR_INFO_H__
