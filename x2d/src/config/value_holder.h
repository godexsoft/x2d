//
//  value_holder.h
//  x2d
//
//  Created by Alex Kremer on 2/8/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifndef __X2D_VALUE_HOLDER_H__
#define __X2D_VALUE_HOLDER_H__

#include <string>
#include <vector>
#include <boost/random.hpp>

namespace x2d {
namespace config {
    
    class configuration;

    template<typename T>
    class value_holder
    {
    public:
        value_holder(const std::string& k, const T& def)
        : set_(true)
        , key_(k)
        , def_val_(def)
        {            
        }
        
        value_holder(const T& def=T())
        : set_(false)
        , def_val_(def)
        {
        }

        T get(configuration& c) const;
        
        const bool is_set() const
        {
            return set_;
        }
        
    private:
        bool            set_;
        std::string     key_;
        T               def_val_;
    };

    // special version for <list>
    template<>
    class value_holder<std::vector<std::string> >
    {
    public:
        value_holder(const std::string& k, const std::vector<std::string>& def)
        : set_(true)
        , key_(k)
        , def_val_(def)
        , dist_(0.0, 1.0)
        {
        }
        
        value_holder(const std::vector<std::string>& def=std::vector<std::string>())
        : set_(false)
        , def_val_(def)
        , dist_(0.0, 1.0)
        {
        }
        
        std::vector<std::string> get(configuration& c) const;
        std::string random(configuration& c) const;
        
        const bool is_set() const
        {
            return set_;
        }
        
    private:
        double next_rand() const
        {
            return dist_(gen_);
        }
        
        bool                      set_;
        std::string               key_;
        std::vector<std::string>  def_val_;
        
        static boost::random::mt19937 gen_;
        boost::random::uniform_real_distribution<> dist_;
    };
    
} // namespace config
} // namespace x2d
using namespace x2d::config;
    
#endif // __X2D_VALUE_HOLDER_H__
