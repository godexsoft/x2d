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
    
    // Wrappers for boost.random to get rid of the annoying difference between int and real distributions
    template<typename T>
    struct wrapped_distribution;
    
    template<>
    struct wrapped_distribution<float> : public boost::random::uniform_real_distribution<float>
    {
        wrapped_distribution(const float& min, const float& max)
        : boost::random::uniform_real_distribution<float>(min, max) {}
        wrapped_distribution() {}
    };
    
    template<>
    struct wrapped_distribution<double> : public boost::random::uniform_real_distribution<double>
    {
        wrapped_distribution(const double& min, const double& max)
        : boost::random::uniform_real_distribution<double>(min, max) {}
        wrapped_distribution() {}
    };

    template<>
    struct wrapped_distribution<int> : public boost::random::uniform_int_distribution<int>
    {
        wrapped_distribution(const int& min, const int& max)
        : boost::random::uniform_int_distribution<int>(min, max) {}
        wrapped_distribution() {}
    };

    template<>
    struct wrapped_distribution<long> : public boost::random::uniform_int_distribution<long>
    {
        wrapped_distribution(const long& min, const long& max)
        : boost::random::uniform_int_distribution<long>(min, max) {}
        wrapped_distribution() {}
    };

    template<>
    struct wrapped_distribution<short> : public boost::random::uniform_int_distribution<short>
    {
        wrapped_distribution(const short& min, const short& max)
        : boost::random::uniform_int_distribution<short>(min, max) {}
        wrapped_distribution() {}
    };
    
    
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
        value(const T& v = T())
        : value_(v)
        , is_rand_(false)
        {
        }
        
        /**
         * Constructor from two values. Will produce random value in range.
         * @param[in] from Minimum value to generate
         * @param[in] to Maximum value to generate
         */
        value(const T& from, const T& to)
        : from_(from>to?to:from)
        , to_(from<to?to:from)
        , dist_(from_, to_)
        , is_rand_(true)
        {
            // If random ends up with same value everytime - cache it
            if(to == from)
            {
                is_rand_ = false;
                value_ = to;
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
                return dist_(x2d::GENERATOR);
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
        
        inline T mid() const
        {
            return (min()+max())/2;
        }
        
        inline T min() const
        {
            if(is_rand_)
            {
                return from_;
            }
            
            return value_;
        }
        
        inline T max() const
        {
            if(is_rand_)
            {
                return to_;
            }

            return value_;
        }
        
    private:
        T value_;
        T from_;
        T to_;
        wrapped_distribution<T> dist_;
        bool is_rand_;
    };
    
    /**
     * Special version for std::string
     */
    template<>
    class value<std::string>
    {
    public:
        /**
         * Constructor from single cached value.
         * @param[in] v Value to store
         */
        value(const std::string& v)
        : value_(v)
        {
        }
        
        /**
         * Get the value.
         * @return Value of the type specified at template instantiation
         */
        std::string get() const
        {
            return value_;
        }
        
        /**
         * Convenience access operator.
         */
        std::string operator *() const
        {
            return get();
        }
        
    private:
        std::string value_;
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
     * Special version for point
     */
    template<>
    class value<point>
    {
    public:
        /**
         * Constructor from single cached value.
         * @param[in] v Value to store
         */
        value(const point& v)
        : x_(v.x)
        , y_(v.y)
        {
        }
        
        /**
         * Constructor from two values. Will produce random value in range.
         * @param[in] from Minimum value to generate
         * @param[in] to Maximum value to generate
         */
        value(const point& from, const point& to)
        : x_(from.x, to.x)
        , y_(from.y, to.y)
        {
        }
        
        /**
         * Get a value.
         * @return Value of the type specified at template instantiation
         */
        point get() const
        {
            return point(*x_, *y_);
        }
        
        /**
         * Convenience access operator.
         */
        point operator *() const
        {
            return get();
        }
        
    private:
        value<float> x_;
        value<float> y_;
    };    
    
    /**
     * Special version for size
     */
    template<>
    class value<size>
    {
    public:
        /**
         * Constructor from single cached value.
         * @param[in] v Value to store
         */
        value(const size& v)
        : w_(v.width)
        , h_(v.height)
        {
        }
        
        /**
         * Constructor from two values. Will produce random value in range.
         * @param[in] from Minimum value to generate
         * @param[in] to Maximum value to generate
         */
        value(const size& from, const size& to)
        : w_(from.width, to.width)
        , h_(from.height, to.height)
        {
        }
        
        /**
         * Get a value.
         * @return Value of the type specified at template instantiation
         */
        size get() const
        {
            return size(*w_, *h_);
        }
        
        /**
         * Convenience access operator.
         */
        size operator *() const
        {
            return get();
        }
        
    private:
        value<float> w_;
        value<float> h_;
    };
    
    /**
     * Special version for rect
     */
    template<>
    class value<rect>
    {
    public:
        /**
         * Constructor from single cached value.
         * @param[in] v Value to store
         */
        value(const rect& v)
        : o_(v.origin)
        , a_(v.area)
        {
        }
        
        /**
         * Constructor from two values. Will produce random value in range.
         * @param[in] from Minimum value to generate
         * @param[in] to Maximum value to generate
         */
        value(const rect& from, const rect& to)
        : o_(from.origin, to.origin)
        , a_(from.area, to.area)
        {
        }
        
        /**
         * Get a value.
         * @return Value of the type specified at template instantiation
         */
        rect get() const
        {
            point o = *o_;
            size a = *a_;
            
            return rect(o.x, o.y, a.width, a.height);
        }
        
        /**
         * Convenience access operator.
         */
        rect operator *() const
        {
            return get();
        }
        
    private:
        value<point> o_;
        value<size> a_;
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
     * Special version for glm::vec4
     */
    template<>
    class value<glm::vec4>
    {
    public:
        /**
         * Constructor from single cached value.
         * @param[in] v Value to store
         */
        value(const glm::vec4& v = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f))
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
        value(const glm::vec4& from, const glm::vec4& to)
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
        glm::vec4 get() const
        {
            return glm::vec4(*r_, *g_, *b_, *a_);
        }
        
        /**
         * Convenience access operator.
         */
        glm::vec4 operator *() const
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
        value(const color_info& v = color_info())
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
     * @brief Optional value with default placeholder.
     */
    template<typename T>
    class optional_value
    {
    public:
        /**
         * Constructor with default placeholder. 
         * Note: the def argument is not value<> so that default values can't be random.
         */
        optional_value(const T& def = T())
        : def_(def)
        {
        }
        
        optional_value(const optional_value<T>& r)
        : value_(r.value_)
        , def_(r.def_)
        {
        }
        
        /**
         * Sets the optional value.
         * @param[in] v The value to set
         */
        void set(const value<T>& v)
        {
            value_ = v;
        }
        
        /**
         * Get the value or the default placeholder if value was not set.
         * @return Value of the type specified at template instantiation
         */
        T get() const
        {
            return value_.get_value_or(def_).get();
        }
        
        /**
         * Convenience access operator.
         */
        T operator *() const
        {
            return get();
        }
        
        /**
         * Safe bool conversion.
         * Used to check whether the value was set.
         * @return true if value was set. false otherwise.
         */
        operator bool() const
        {
            return static_cast<bool>(value_);
        }
        
        operator value<T>() const
        {
            return value_.get_value_or(def_);
        }
        
    private:
        value<T> def_;
        boost::optional<value<T> > value_;        
    };
    
    /**
     * @brief This template provides random selected values from a list.
     *
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
                to = lst_.empty() ? 0 : lst_.size()-1;
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
            if(lst_.empty())
            {
                throw sys_exception("list_value::get() - get invoked on empty vector. no random value to return.");
            }
            
            return lst_.at(*rnd_);
        }
        
        /**
         * Get a copy of the whole list.
         * @return A copy of the vector
         */
        std::vector<T> get_list() const
        {
            return lst_;
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
    
    /**
     * @brief This template provides an easy way to store cached values which can be invalidated
     * and checked if the value is still valid or not.
     */
    template<typename T>
    class cached_value
    {
    public:
        cached_value(const T& v)
        : value_(v)
        , is_valid_(true)
        {
        }
        
        /**
         * Invalidate the cached value.
         */
        void invalidate()
        {
            is_valid_ = false;
        }
        
        /**
         * Assign a new value effectively making the cached value valid again.
         * @param[in] v The new value
         */
        cached_value& operator= (const T& v)
        {
            value_ = v;
            is_valid_ = true;
            
            return *this;
        }
        
        /**
         * Safe boolean check.
         * @return true if cached value is still valid. false otherwise.
         */
        operator bool ()
        {
            return is_valid_;
        }
        
    private:
        T value_;
        bool is_valid_;
    };
    
} // namespace x2d
using namespace x2d;
    
#endif // __X2D_VALUE_H__
