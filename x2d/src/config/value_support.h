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
#include "value.h"

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

#include "object_support.h"

namespace x2d {
namespace config {

    /**
     * @brief Value config type support: <float>, <int>, etc.
     */
    template<typename T>
    class value_cfg
    : public cfg_base
    { 
    public:
        /**
         * Construction from static value.
         *
         * @param[in] v       The value to hold
         */
        value_cfg(const T& v)
        : val_(v)
        {
        }
        
        /**
         * Construction from randomized value.
         *
         * @param[in] min   Minimum value to generate
         * @param[in] max   Maximum value to generate
         */
        value_cfg(const T& min, const T& max)
        : val_(min, max)
        {
        }
        
        /**
         * @return a copy of the value or randomizer
         */
        value<T> get() const
        {
            return val_;
        }
        
    private:        
        value<T> val_;
    };
    
    /**
     * Special version for list_value
     */
    template<>
    class value_cfg< std::vector<std::string> >
    : public cfg_base
    {
    public:
        /**
         * Construction from static value.
         *
         * @param[in] v       The value to hold
         */
        value_cfg(const std::vector<std::string>& v)
        : val_(v)
        {
        }
        
        /**
         * @return a copy of the value or randomizer
         */
        list_value<std::string> get() const
        {
            return val_;
        }
        
    private:
        list_value<std::string> val_;
    };
    
    /**
     * @brief Value parsing support.
     */
    template<typename T>
    class value_parser
    {
    public:
        /**
         * @param[in] str String representation of value
         * @return Parsed value of required type
         * 
         * For a list of supported value types see specializations of this template method.
         */
        static T parse(const std::string& str);
    };
    
} // namespace config
} // namespace x2d
using namespace x2d::config;

#endif // __X2D_VALUE_SUPPORT_H__
