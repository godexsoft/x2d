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
     * Note: the default template generates no random numbers.
     * Only certain types are supported with random.
     */
    template<typename T>
    class random_cfg
    : public cfg_base
    { 
    public:
        random_cfg(const T& min, const T& max)
        {
        }
        
        T get()
        {    
            throw sys_exception("Random is not supported for this type.");
        }
    };

    
    /**
     * @brief '<random>' for int type config type support
     */
    template<>
    class random_cfg<int>
    : public cfg_base
    { 
    public:
        /**
         * @param[in] min     Min possible value to generate
         * @param[in] max     Max possible value to generate
         */
        random_cfg(const int& min, const int& max)
        : gen_(platform::time::current_time())
        , dist_(min, max)
        {
        }
        
        /**
         * Return next random value
         */
        int get()
        {    
            return dist_(gen_);
        }
        
    private:        
        boost::random::mt19937 gen_;
        boost::random::uniform_int_distribution<> dist_;
    };

    
    /**
     * @brief '<random>' for float type config type support
     */
    template<>
    class random_cfg<float>
    : public cfg_base
    { 
    public:
        /**
         * @param[in] min     Min possible value to generate
         * @param[in] max     Max possible value to generate
         */
        random_cfg(const float& min, const float& max)
        : gen_(platform::time::current_time())
        , dist_(min, max)
        {
        }
        
        /**
         * Return next random value
         */
        float get()
        {    
            return dist_(gen_);
        }
        
    private:        
        boost::random::mt19937 gen_;
        boost::random::uniform_real_distribution<> dist_;
    };
    
    /**
     * @brief '<random>' for vec3 type config type support
     */
    template<>
    class random_cfg<glm::vec3>
    : public cfg_base
    { 
    public:
        /**
         * @param[in] min     Min possible value to generate
         * @param[in] max     Max possible value to generate
         */
        random_cfg(const glm::vec3& min, const glm::vec3& max)
        : gen_(platform::time::current_time())
        , dist_x_(min.x, max.x)
        , dist_y_(min.y, max.y)
        , dist_z_(min.z, max.z)
        {
        }
        
        /**
         * Return next random vector
         */
        glm::vec3 get()
        {    
            return glm::vec3(dist_x_(gen_), dist_y_(gen_), dist_z_(gen_));
        }
        
    private:        
        boost::random::mt19937 gen_;
        boost::random::uniform_real_distribution<> dist_x_;
        boost::random::uniform_real_distribution<> dist_y_;
        boost::random::uniform_real_distribution<> dist_z_;
    };

    /**
     * @brief '<random>' for vec2 type config type support
     */
    template<>
    class random_cfg<glm::vec2>
    : public cfg_base
    { 
    public:
        /**
         * @param[in] min     Min possible value to generate
         * @param[in] max     Max possible value to generate
         */
        random_cfg(const glm::vec2& min, const glm::vec2& max)
        : gen_(platform::time::current_time())
        , dist_x_(min.x, max.x)
        , dist_y_(min.y, max.y)
        {
        }
        
        /**
         * Return next random vector
         */
        glm::vec2 get()
        {    
            return glm::vec2(dist_x_(gen_), dist_y_(gen_));
        }
        
    private:        
        boost::random::mt19937 gen_;
        boost::random::uniform_real_distribution<> dist_x_;
        boost::random::uniform_real_distribution<> dist_y_;
    };
    
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
         * Construction from random generator.
         *
         * @param[in] res_man The resource manager for cfg_base
         * @param[in] rnd     Random generator to hold
         */
        value_cfg(const boost::shared_ptr<random_cfg<T> >& rnd)
        : val_()
        , random_(rnd)
        {
        }
        
        /**
         * Return static value or next random value if holding a rondom generator.
         */
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
