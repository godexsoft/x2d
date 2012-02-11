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
