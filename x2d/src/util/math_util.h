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
#include "glm.hpp"
#include "log.h"

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

        inline const size operator *(float f) const 
        { 
            return size(width*f, height*f); 
        }
        
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

        inline operator const glm::vec3() const { return glm::vec3(x, y, 0.0f); }
        inline operator const glm::vec2() const { return glm::vec2(x, y); }
        
        float x;
        float y;
    };

    struct rect
    {        
#ifdef __APPLE__
        rect(const CGRect& r)
        : origin(r.origin)
        , area(r.size)
        {            
        }
        
        inline operator CGRect() { return CGRectMake(origin.x, origin.y, area.width, area.height); }
        inline operator const CGRect() const { return CGRectMake(origin.x, origin.y, area.width, area.height); }
#endif  
        
        rect()
        {            
        }
        
        rect(float x, float y, float width, float height)
        : origin(x, y)
        , area(width, height)
        {            
        }
      
        inline bool contains_point(const point& p)
        {
            if(p.x >= origin.x && p.x <= origin.x+area.width)
                if(p.y >= origin.y && p.y <= origin.y+area.height)
                    return true;
            
            return false;
        }
        
        point origin;
        size area;
    };
    
    struct bbox
    {
        glm::vec3 a, b, c, d;
        
        bbox(const glm::vec3& aa, const glm::vec3& bb, const glm::vec3& cc, const glm::vec3& dd)
        : a(aa)
        , b(bb)
        , c(cc)
        , d(dd)
        {}
        
        inline bool contains_point(const glm::vec3& p)
        {
            float aa = triangle_area(a,b,p);
            float bb = triangle_area(b,c,p);
            float cc = triangle_area(c,d,p);
            float dd = triangle_area(d,a,p);
            
            if ((aa>0 && bb>0 && cc>0 && dd>0) ||
                (aa<0 && bb<0 && cc<0 && dd<0))
            {
                return true;
			}
            
			return false;
        }
        
        static float triangle_area(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
        {
			return (c.x*b.y-b.x*c.y)-(c.x*a.y-a.x*c.y)+(b.x*a.y-a.x*b.y);
		}
    };
    
    enum alignment
    {
        LEFT_ALIGN = 1,
        CENTER_ALIGN,
        RIGHT_ALIGN
    };
       
} // namespace math
} // namespace x2d
using namespace x2d::math;

#endif // __X2D_MATH_UTIL_H__
