//
//  value_holder.cpp
//  x2d
//
//  Created by Alex Kremer on 2/8/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "value_holder.h"
#include "math_util.h"
#include "configuration.h"

namespace x2d {
namespace config {
        
    template<>
    float value_holder<float>::get(configuration& c) const
    {
        if(!key_.empty() && c.exists(key_))
            return c.get_value<float>(key_);

        return def_val_;
    }

    template<>
    int value_holder<int>::get(configuration& c) const
    {
        if(!key_.empty() && c.exists(key_))
            return c.get_value<int>(key_);
        
        return def_val_;
    }
    
    template<>
    std::string value_holder<std::string>::get(configuration& c) const
    {
        if(!key_.empty() && c.exists(key_))
            return c.get_value<std::string>(key_);
        
        return def_val_;
    }
    
    template<>
    point value_holder<point>::get(configuration& c) const
    {
        if(!key_.empty() && c.exists(key_))
            return c.get_value<point>(key_);
        
        return def_val_;
    }
    
    template<>
    size value_holder<size>::get(configuration& c) const
    {
        if(!key_.empty() && c.exists(key_))
            return c.get_value<size>(key_);
        
        return def_val_;
    }
    
    template<>
    rect value_holder<rect>::get(configuration& c) const
    {
        if(!key_.empty() && c.exists(key_))
            return c.get_value<rect>(key_);
        
        return def_val_;
    }
    
    template<>
    color_info value_holder<color_info>::get(configuration& c) const
    {
        if(!key_.empty() && c.exists(key_))
            return c.get_value<color_info>(key_);
        
        return def_val_;
    }
    
    template<>
    vector_2d value_holder<vector_2d>::get(configuration& c) const
    {
        if(!key_.empty() && c.exists(key_))
            return c.get_value<vector_2d>(key_);
        
        return def_val_;
    }
    
    template<>
    vector_25d value_holder<vector_25d>::get(configuration& c) const
    {
        if(!key_.empty() && c.exists(key_))
            return c.get_value<vector_25d>(key_);
        
        return def_val_;
    }
            
} // namespace config
} // namespace x2d
