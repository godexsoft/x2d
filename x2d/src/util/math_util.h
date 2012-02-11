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
        
        inline const float x() const { return x_; };
        inline const float y() const { return y_; };
        
        void x( float xx ) { x_ = xx; };
        void y( float yy ) { y_ = yy; };
        
    private:
        float x_;
        float y_;
    };
    
    class vector_25d
    : public vector_2d
    {
    public:
        vector_25d()
        : vector_2d(0.0f, 0.0f)
        , z_(0.0f)
        { 
        }
        
        vector_25d(float x, float y, float z)
        : vector_2d(x, y)
        , z_(z)
        { 
        }
        
#ifdef __APPLE__
        vector_25d(CGPoint point)
        : vector_2d(point.x, point.y)
        , z_(0.0f) 
        { 
        }
#endif
        
        inline const float z() const { return z_; }
        void z( float zz ) { z_ = zz; }
      
    private:
        float z_;
    };
    
    
    struct size
    {
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
    
    inline vector_2d operator *(float t, const vector_2d& v) 
    {
        return (vector_2d(t * v.x(), t * v.y()));
    }
            
    float to_rad( float deg );
    float to_deg( float rad );
            
    inline float clamp(float v, float min, float max)
    {
        return fmaxf(min, fminf(v, max));
    }

    // affine
    class affine_matrix 
    {
        union {
            float data_[9];
            struct {
                float
                d0, d1, d2,
                d3, d4, d5,
                d6, d7, d8;
            };
        };
        
        affine_matrix(float d[]);   // used by static initializers only
        
    public:
        // create identity
        affine_matrix()
        {
            // fill with 0.0f and then make identity
            d1=0.0f; d2=0.0f; d3=0.0f;
            d0=1.0f; d4=1.0f; d8=1.0f;
            d5=0.0f; d6=0.0f; d7=0.0f;
        }
        
        affine_matrix(const affine_matrix& m) 
        {
            memcpy(data_, m.data_, 9 * sizeof(float));
        }
        
        static const affine_matrix translation(float x, float y);
        static const affine_matrix scale(float x, float y);
        static const affine_matrix rotate(float angle);
        const affine_matrix inverse();
        
        double determinant_of_minor(int y, int x);
        double determinant();
        
        // apply to a vector and point
        vector_2d apply(const vector_2d& v);
        point apply(const point& p);
        
        // operators
        affine_matrix& operator *=(const affine_matrix& m);
    };
    
} // namespace math
} // namespace x2d
using namespace x2d::math;

#endif // __X2D_MATH_UTIL_H__
