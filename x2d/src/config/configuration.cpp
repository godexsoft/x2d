//
//  configuration.cpp
//  x2d
//
//  Created by Alex Kremer on 1/30/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "configuration.h"

namespace x2d {
namespace config {
        
    configuration::configuration(resource_manager& res_man, const std::string& cfg_path)
    : res_man_(res_man)
    {
        
    }
    
} // namespace config    
} // namespace x2d