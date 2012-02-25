//
//  interpolator.h
//  x2d
//
//  Created by Alex Kremer on 1/23/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//
//  Happy birthday, Verochka! :-)
//

#pragma once
#ifndef __X2D_INTERPOLATOR_H__
#define __X2D_INTERPOLATOR_H__

#include "glm.hpp"

namespace x2d {
namespace util {

    class time_interpolator
    {
    public:
        time_interpolator(float t)
        : total_time_(t)
        , elapsed_time_(0.0f)
        {
        }
        
        virtual ~time_interpolator()
        {        
        }
        
        virtual void update(float dt)
        {
            elapsed_time_ += dt;
            
            if(elapsed_time_ > total_time_)
                reset();
            
            calculate();
        }
        
        virtual void reset()
        {
            elapsed_time_ = 0.0f;
        }
        
        virtual void calculate() = 0;
        
    protected:
        float total_time_;
        float elapsed_time_;
    };

    /**
     * Linear interpolator
     */
    class linear_interpolator
    : public time_interpolator
    {
    public:
        linear_interpolator(float& v, float min, float max, float t)
        : time_interpolator(t)
        , val_(v)
        , min_(min)
        , max_(max)
        {
        }
        
        virtual void calculate()
        {        
            float b = glm::clamp( elapsed_time_ / total_time_, 0.0f, 1.0f);
            val_ = min_ * (1.0f - b) + max_ * b;
        }

    private:
        float & val_;
        float min_, max_;
    };

    /**
     * Quadratic interpolator
     */
    class quadratic_interpolator
    : public time_interpolator
    {
    public:
        quadratic_interpolator(float& v, float min, float mid, float max, float t)
        : time_interpolator(t)
        , val_(v)
        , min_(min)
        , mid_(mid)
        , max_(max)
        {
        }
        
        virtual void calculate()
        {        
            float b = glm::clamp( elapsed_time_ / total_time_, 0.0f, 1.0f);
            float a = 1.0f - b;
            val_ = min_*a*a + mid_*2*a*b + max_*b*b;
        }
        
    private:
        float & val_;
        float min_, mid_, max_;
    };
  
    /**
     * Cubic interpolator
     */
    class cubic_interpolator
    : public time_interpolator
    {
    public:
        cubic_interpolator(float& v, float min, 
                               float mid1, float mid2, 
                               float max, float t)
        : time_interpolator(t)
        , val_(v)
        , min_(min)
        , mid1_(mid1)
        , mid2_(mid2)
        , max_(max)
        {
        }
        
        virtual void calculate()
        {        
            float b = glm::clamp( elapsed_time_ / total_time_, 0.0f, 1.0f);
            float a = 1.0f - b;
            val_ = min_*a*a*a + mid1_*3*a*a*b + mid2_*3*a*b*b + max_*b*b*b;
        }
        
    private:
        float & val_;
        float min_, mid1_, mid2_, max_;
    };
    
} // namespace util
} // namespace x2d
using namespace x2d::util;

#endif // __X2D_INTERPOLATOR_H__
