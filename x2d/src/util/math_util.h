//
//  math_util.h
//  x2d
//
//  Created by Alex Kremer on 1/23/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_MATH_UTIL_H__
#define __X2D_MATH_UTIL_H__

#include <cmath>
#include <cstring>

#ifdef __APPLE__
#include <CoreGraphics/CoreGraphics.h>
#endif

namespace x2d {
namespace math {
    
    struct size
    {
        size()
        : width(0.0f)
        , height(0.0f)
        {            
        }
        
        size(float w, float h)
        : width(w)
        , height(h)
        {            
        }
        
#ifdef __APPLE__
        size(const CGSize& s)
        : width(s.width)
        , height(s.height)
        {            
        }
        
        inline operator CGSize() { return CGSizeMake(width, height); }
        inline operator const CGSize() const { return CGSizeMake(width, height); }
#endif
        
        float width;
        float height;
    };

    
    struct point
    {
        point()
        : x(0.0f)
        , y(0.0f)
        {            
        }
        
        point(float ix, float iy)
        : x(ix)
        , y(iy)
        {            
        }
        
#ifdef __APPLE__
        point(const CGPoint& p)
        : x(p.x)
        , y(p.y)
        {            
        }
        
        inline operator CGPoint() { return CGPointMake(x, y); }
        inline operator const CGPoint() const { return CGPointMake(x, y); }
#endif
        
        float x;
        float y;
    };

    struct rect
    {        
#ifdef __APPLE__
        rect(const CGRect& r)
        : origin(r.origin)
        , size(r.size)
        {            
        }
        
        inline operator CGRect() { return CGRectMake(origin.x, origin.y, size.width, size.height); }
        inline operator const CGRect() const { return CGRectMake(origin.x, origin.y, size.width, size.height); }
#endif  
        
        rect()
        {            
        }
        
        rect(float x, float y, float width, float height)
        : origin(x, y)
        , size(width, height)
        {            
        }
      
        inline bool contains_point(const point& p)
        {
            if(p.x >= origin.x && p.x <= origin.x+size.width)
                if(p.y >= origin.y && p.y <= origin.y+size.height)
                    return true;
            
            return false;
        }
        
        point origin;
        size size;        
    };
       
} // namespace math
} // namespace x2d
using namespace x2d::math;

#endif // __X2D_MATH_UTIL_H__
