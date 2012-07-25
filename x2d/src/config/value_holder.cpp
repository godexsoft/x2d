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
    short value_holder<short>::get(configuration& c) const
    {
        if(!key_.empty() && c.exists(key_))
            return c.get_value<short>(key_);
        
        return def_val_;
    }
    
    template<>
    bool value_holder<bool>::get(configuration& c) const
    {
        if(!key_.empty() && c.exists(key_))
            return c.get_value<bool>(key_);
        
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
    glm::vec2 value_holder<glm::vec2>::get(configuration& c) const
    {
        if(!key_.empty() && c.exists(key_))
            return c.get_value<glm::vec2>(key_);
        
        return def_val_;
    }
    
    template<>
    glm::vec3 value_holder<glm::vec3>::get(configuration& c) const
    {
        if(!key_.empty() && c.exists(key_))
            return c.get_value<glm::vec3>(key_);
        
        return def_val_;
    }
   
    std::vector<std::string> value_holder<std::vector<std::string> >::get(configuration& c) const
    {
        if(!key_.empty() && c.exists(key_))
            return c.get_value<std::vector<std::string> >(key_);
        
        return def_val_;
    }
    
    std::string value_holder<std::vector<std::string> >::random(configuration& c) const
    {
        if(!key_.empty() && c.exists(key_))
        {
            std::vector<std::string> vec = c.get_value<std::vector<std::string> >(key_);
            return vec.at(static_cast<int>( next_rand() * vec.size() ));
        }
        
        return def_val_.at(static_cast<int>( next_rand() * def_val_.size() ));
    }
    
    // keep the linker happy
    boost::random::mt19937 value_holder<std::vector<std::string> >::gen_ = boost::random::mt19937(platform::time::current_time());
    
} // namespace config
} // namespace x2d
