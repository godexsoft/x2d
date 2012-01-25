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

#ifdef __APPLE__
#include <CoreGraphics/CoreGraphics.h>
#endif

namespace x2d {
namespace math {
    
    class vector_2d 
    {
    public:
        vector_2d()
        : x_(0.0f)
        , y_(0.0f) 
        { 
        }
        
        vector_2d(float x, float y)
        : x_(x)
        , y_(y) 
        { 
        }
        
#ifdef __APPLE__
        vector_2d(CGPoint point)
        : x_(point.x)
        , y_(point.y) 
        { 
        }
#endif
        
        vector_2d& operator +=(const vector_2d& v) 
        {
            x_ += v.x_;
            y_ += v.y_;
            return (*this);
        }
        
        vector_2d& operator -=(const vector_2d& v) 
        {
            x_ -= v.x_;
            y_ -= v.y_;
            return (*this);
        }
        
        vector_2d& operator *=(float t) 
        {
            x_ *= t;
            y_ *= t;
            return (*this);
        }
        
        vector_2d& operator /=(float t) 
        {
            float f = 1.0f / t;
            x_ *= f;
            y_ *= f;
            return (*this);
        }
        
        vector_2d operator -(void) const	
        {
            return (vector_2d(-x_, -y_));
        }        
        
        vector_2d operator +(const vector_2d& v) const 
        {
            return (vector_2d(x_ + v.x_, y_ + v.y_));
        }
        
        vector_2d operator -(const vector_2d& v) const 
        {
            return (vector_2d(x_ - v.x_, y_ - v.y_));
        }
        
        vector_2d operator *(float t) const 
        {
            return (vector_2d(x_ * t, y_ * t));
        }	
        
        vector_2d operator /(float t) const 
        {
            float f = 1.0f / t;
            return (vector_2d(x_ * f, y_ * f));
        }
		
        vector_2d& normalize(void) 
        {
            return (*this /= sqrtf(x_ * x_ + y_ * y_));
        }
        
        bool operator ==(const vector_2d& v) const 
        {
            return ((x_ == v.x_) && (y_ == v.y_));
        }
        
        bool operator !=(const vector_2d& v) const 
        {
            return ((x_ != v.x_) || (y_ != v.y_));
        }	
        
        static float dist(const vector_2d& l, const vector_2d& r);
        static float dist_squared(const vector_2d& l, const vector_2d& r);
        
        inline static float mag_squared(const vector_2d& v) 
        {
            return (v.x_ * v.x_ + v.y_ * v.y_);
        }
        
        inline static float mag(const vector_2d& v) 
        {
            return (sqrtf(v.x_ * v.x_ + v.y_ * v.y_));
        }	

#ifdef __APPLE__
        // Transparent usage of vector_2d as CGPoint
        inline operator CGPoint() { return CGPointMake(x_, y_); }
        inline operator const CGPoint() const { return CGPointMake(x_, y_); }
#endif
        
        inline const float X() const { return x_; };
        inline const float Y() const { return y_; };
        
        void X( float x ) { x_ = x; };
        void Y( float y ) { y_ = y; };
        
    private:
        float x_;
        float y_;
    };
    
    
    struct size
    {
        size(float w, float h)
        : width(w)
        , height(h)
        {            
        }
        
#ifdef __APPLE_
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
        point(float ix, float iy)
        : x(ix)
        , y(iy)
        {            
        }
        
#ifdef __APPLE_
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
#ifdef __APPLE_
        rect(const CGRect& r)
        : origin(r.origin)
        , size(r.size)
        {            
        }
        
        inline operator CGRect() { return CGRectMake(origin.x, origin.y, size.width, size.height); }
        inline operator const CGRect() const { return CGRectMake(origin.x, origin.y, size.width, size.height); }
#endif
        
        point origin;
        size size;        
    };
    
    inline vector_2d operator *(float t, const vector_2d& v) {
        return (vector_2d(t * v.X(), t * v.Y()));
    }
    
    inline void point_transform(float out[4], const float m[16], const float in[4]);
    inline void matrix_multiply(float* product, const float* a, const float* b);
    inline bool matrix_invert(const float* m, float* out);
        
    inline float to_rad( float deg );
    inline float to_deg( float rad );
            
    inline float clamp(float v, float min, float max)
    {
        return fmaxf(min, fminf(v, max));
    }

} // namespace math
} // namespace x2d
using namespace x2d::math;

#endif // __X2D_MATH_UTIL_H__
