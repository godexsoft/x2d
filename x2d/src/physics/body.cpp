//
//  body.cpp
//  x2d
//
//  Created by Alex Kremer on 07/08/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "body.h"

namespace x2d {    
namespace physics {

    body::body(kernel& k, configuration& conf, bool dynamic)
    : kernel_(k)
    , config_(conf)
    , dynamic_(dynamic)
    {        
        LOG("Setting b2Body userdata to 0x%X", this);
        body_->SetUserData(this);
    }
    
    void body::add_part(const boost::shared_ptr<body_part>& part)
    {
        parts_.push_back(part);
    }
    
} // namespace physics
} // namespace x2d