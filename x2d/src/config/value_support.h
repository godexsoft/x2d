//
//  value_support.h
//  x2d
//
//  Created by Alex Kremer on 2/1/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_VALUE_SUPPORT_H__
#define __X2D_VALUE_SUPPORT_H__

#include "resource_manager.h"
#include "platform.h"

#include <boost/shared_ptr.hpp>
#include <boost/random.hpp>

#include "object_support.h"

namespace x2d {
namespace config {

    /**
     * @brief '<random>' config type support
     */
    template<typename T>
    class random_cfg
    : public cfg_base
    { 
    public:
        random_cfg(resource_manager& res_man, const T& min, const T& max)
        : cfg_base(res_man)
        , gen_(platform::time::current_time())
        , dist_(min, max)
        {
        }
        
        T get()
        {    
            return dist_(gen_);
        }
        
    private:        
        boost::random::mt19937 gen_;
        boost::random::uniform_real_distribution<> dist_;
    };
    
    /**
     * @brief Value config type support: <float>, <int>, etc.
     */
    template<typename T>
    class value_cfg
    : public cfg_base
    { 
    public:
        value_cfg(resource_manager& res_man, const T& v)
        : cfg_base(res_man)
        , val_(v)
        {
        }
        
        value_cfg(resource_manager& res_man, const boost::shared_ptr<random_cfg<T> >& rnd)
        : cfg_base(res_man)
        , val_(0.0f)
        , random_(rnd)
        {
        }
        
        T get()
        {    
            if(!random_)
            {
                return val_;
            }
            else                
            {
                return random_->get();
            }
        }
        
    private:        
        T val_;
        boost::shared_ptr< random_cfg<T> > random_;
    };
    
} // namespace config
} // namespace x2d
using namespace x2d::config;

#endif // __X2D_VALUE_SUPPORT_H__
