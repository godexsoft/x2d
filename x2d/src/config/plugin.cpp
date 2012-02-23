//
//  plugin.cpp
//  x2d
//
//  Created by Alex Kremer on 23/02/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "plugin.h"

namespace x2d {
namespace config {
    
    void plugin::add_to_config(const config_key& key, const boost::shared_ptr<cfg_base>& obj)        
    {
        config_.config_[key] = obj;
    }
        
} // namespace config
} // namespace x2d
