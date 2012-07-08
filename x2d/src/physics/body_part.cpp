//
//  body_part.cpp
//  x2d
//
//  Created by Alex Kremer on 07/08/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "body_part.h"
#include "configuration.h"

namespace x2d {    
namespace physics {

    body_part::body_part(configuration& conf, std::string obj_key) 
    : config_(conf)
    {
        if(!obj_key.empty()) {
            object_ = config_.create_object(obj_key);
        }
    }
    
} // namespace physics
} // namespace x2d