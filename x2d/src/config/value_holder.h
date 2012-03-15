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
        : key_(k)
        , def_val_(def)
        {            
        }

        T get(configuration& c) const;
        
    private:
        std::string     key_;
        T               def_val_;
    };

    // special version for <list>
    template<>
    class value_holder<std::vector<std::string> >
    {
    public:
        value_holder(const std::string& k, const std::vector<std::string>& def)
        : key_(k)
        , def_val_(def)
        , dist_(0.0, 1.0)
        {            
        }
        
        std::vector<std::string> get(configuration& c) const;
        std::string random(configuration& c) const;
        
    private:
        double next_rand() const
        {
            return dist_(gen_);
        }
        
        std::string               key_;
        std::vector<std::string>  def_val_;
        
        static boost::random::mt19937 gen_;
        boost::random::uniform_real_distribution<> dist_;
    };
    
} // namespace config
} // namespace x2d
using namespace x2d::config;
    
#endif // __X2D_VALUE_HOLDER_H__
