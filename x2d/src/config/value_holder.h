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
    
} // namespace config
} // namespace x2d
using namespace x2d::config;
    
#endif // __X2D_VALUE_HOLDER_H__
