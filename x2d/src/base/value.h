//
//  value.h
//  x2d
//
//  Created by Alex Kremer on 2/23/13.
//  Copyright (c) 2013 godexsoft. All rights reserved.
//

#ifndef __X2D_VALUE_H__
#define __X2D_VALUE_H__

#include <string>
#include <vector>
#include <boost/random.hpp>
#include "exceptions.h"
#include "color_info.h"
#include "platform.h"

namespace x2d {

    // Shared global generator for all x2d randomness
    static boost::random::mt19937 GENERATOR = boost::random::mt19937(platform::time::current_time());
    
    /**
     * Holds a value in range. Will return random values from the range on request.
     * If constructed with one argument will hold that single value and always return it on request.
     */
    template<typename T>
    class value
    {
    public:
        /**
         * Constructor from single cached value.
         * @param[in] v Value to store
         */
        value(const T& v)
        : from_(T())
        , to_(T())
        , value_(v)
        , chunk_(0.0)
        , is_rand_(false)
        {
        }
        
        /**
         * Constructor from two values. Will produce random value in range.
         * @param[in] from Minimum value to generate
         * @param[in] to Maximum value to generate
         */
        value(const T& from, const T& to)
        : dist_(0.0, 1.0)
        , from_(from > to ? to : from)
        , to_(from < to ? to : from)
        , value_(to_ - from_)
        , chunk_(1.0/(double)value_)
        , is_rand_(true)
        {
            // If random ends up with same value everytime - cache it
            if(to_ == from_)
            {
                is_rand_ = false;
                value_ = to_;
            }
        }
        
        /**
         * Get a value.
         * @return Value of the type specified at template instantiation
         */
        T get() const
        {
            if(is_rand_)
            {
                // This version works fine with floats and doubles.
                // Check the specializations out for int and long - it's in the cpp file.
                return T((double)from_ + (double)value_ * dist_(x2d::GENERATOR));
            }
            else
            {
                return value_;
            }
        }
        
        /**
         * Convenience access operator.
         */
        T operator *() const
        {
            return get();
        }
        
    private:
        boost::random::uniform_real_distribution<> dist_;
        
        T from_;
        T to_;
        T value_;
        double chunk_;
        bool is_rand_;
    };
    
    /**
     * Special version for bool
     */
    template<>
    class value<bool>
    {
    public:
        /**
         * Default constructor. Assumes that we want random bool value.
         */
        value()
        : value_(0.0f, 1.0f)
        {
        }
        
        /**
         * Constructor from single cached value.
         * @param[in] v Value to store
         */
        value(const bool& v)
        : value_(v?1.0f:0.0f)
        {
        }
        
        /**
         * Constructor from two values. Will produce random value in range.
         * @param[in] from Minimum value to generate
         * @param[in] to Maximum value to generate
         */
        value(const bool& from, const bool& to)
        : value_(from?1.0f:0.0f, to?1.0f:0.0f)
        {
        }
        
        /**
         * Get a value.
         * @return Value of the type specified at template instantiation
         */
        bool get() const
        {
            return bool(*value_ > 0.5f);
        }
        
        /**
         * Convenience access operator.
         */
        bool operator *() const
        {
            return get();
        }
        
    private:
        value<float> value_;
    };
    
    /**
     * Special version for vec2
     */
    template<>
    class value<glm::vec2>
    {
    public:
        /**
         * Constructor from single cached value.
         * @param[in] v Value to store
         */
        value(const glm::vec2& v)
        : x_(v.x)
        , y_(v.y)
        {
        }
        
        /**
         * Constructor from two values. Will produce random value in range.
         * @param[in] from Minimum value to generate
         * @param[in] to Maximum value to generate
         */
        value(const glm::vec2& from, const glm::vec2& to)
        : x_(from.x, to.x)
        , y_(from.y, to.y)
        {
        }

        /**
         * Get a value.
         * @return Value of the type specified at template instantiation
         */
        glm::vec2 get() const
        {
            return glm::vec2(*x_, *y_);
        }
        
        /**
         * Convenience access operator.
         */
        glm::vec2 operator *() const
        {
            return get();
        }
        
    private:
        value<float> x_;
        value<float> y_;
    };

    /**
     * Special version for vec3
     */
    template<>
    class value<glm::vec3>
    {
    public:
        /**
         * Constructor from single cached value.
         * @param[in] v Value to store
         */
        value(const glm::vec3& v)
        : x_(v.x)
        , y_(v.y)
        , z_(v.z)
        {
        }
        
        /**
         * Constructor from two values. Will produce random value in range.
         * @param[in] from Minimum value to generate
         * @param[in] to Maximum value to generate
         */
        value(const glm::vec3& from, const glm::vec3& to)
        : x_(from.x, to.x)
        , y_(from.y, to.y)
        , z_(from.z, to.z)
        {
        }
        
        /**
         * Get a value.
         * @return Value of the type specified at template instantiation
         */
        glm::vec3 get() const
        {
            return glm::vec3(*x_, *y_, *z_);
        }
        
        /**
         * Convenience access operator.
         */
        glm::vec3 operator *() const
        {
            return get();
        }
        
    private:
        value<float> x_;
        value<float> y_;
        value<float> z_;
    };
    
    /**
     * Special version for color_info
     */
    template<>
    class value<color_info>
    {
    public:
        /**
         * Constructor from single cached value.
         * @param[in] v Value to store
         */
        value(const color_info& v)
        : r_(v.r)
        , g_(v.g)
        , b_(v.b)
        , a_(v.a)
        {
        }
        
        /**
         * Constructor from two values. Will produce random value in range.
         * @param[in] from Minimum value to generate
         * @param[in] to Maximum value to generate
         */
        value(const color_info& from, const color_info& to)
        : r_(from.r, to.r)
        , g_(from.g, to.g)
        , b_(from.b, to.b)
        , a_(from.a, to.a)
        {
        }
        
        /**
         * Get a value.
         * @return Value of the type specified at template instantiation
         */
        color_info get() const
        {
            return color_info(*r_, *g_, *b_, *a_);
        }
        
        /**
         * Convenience access operator.
         */
        color_info operator *() const
        {
            return get();
        }
        
    private:
        value<float> r_;
        value<float> g_;
        value<float> b_;
        value<float> a_;
    };

    /**
     * This template provides random selected values from a list.
     * Note: makes a copy of the passed vector.
     */
    template<typename T>
    class list_value
    {
    public:
        list_value(const std::vector<T>& lst, int from=0, int to=-1)
        : lst_(lst)
        , rnd_(from, to == -1 ? lst_.size()-1 : to)
        {
            if(to == -1)
            {
                to = lst_.size()-1;
            }
            
            if(lst_.empty())
            {
                throw sys_exception("list_value - passed vector can't be empty.");
            }
            
            if(from < 0 || from > lst_.size()-1)
            {
                throw sys_exception("list_value - 'from' value can't be smaller than 0 or larger than the size of the passed vector.");
            }

            if(to < 0 || to > lst_.size()-1)
            {
                throw sys_exception("list_value - 'to' value can't be smaller than 0 or larger than the size of the passed vector.");
            }
        }
        
        /**
         * Get a random picked value.
         * @return Value of the type specified at template instantiation
         */
        T get() const
        {
            return lst_.at(*rnd_);
        }
        
        /**
         * Convenience access operator.
         */
        T operator *() const
        {
            return get();
        }
        
    private:
        std::vector<T> lst_;
        value<int> rnd_;
    };
    
    // Forward declarations of specializations to prevent ill-formed program
    template<> int value<int>::get() const;
    template<> long value<long>::get() const;
    template<> short value<short>::get() const;
    
} // namespace x2d
using namespace x2d;
    
#endif // __X2D_VALUE_H__
