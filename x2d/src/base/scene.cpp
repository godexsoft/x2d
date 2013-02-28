//
//  scene.cpp
//  x2d
//
//  Created by Alex Kremer on 2/24/13.
//  Copyright (c) 2013 godexsoft. All rights reserved.
//

#include "scene.h"
#include "configuration.h"

namespace x2d {
namespace base {

    scene::scene(configuration& c, const object_traits& t)
    : object(c, t)
    , config_(c)
    {
        LOG("Scene - constructor");
    }
    
    void scene::on_transition_from()
    {
        LOG("Scene - on transition from");
        destroy_self(); // kill ASAP by default
    }
    
    void scene::on_transition_to()
    {
        LOG("Scene - on transition to");
    }

    configuration& scene::get_config() const
    {
        return config_;
    }
    
    kernel& scene::get_kernel() const
    {
        return config_.get_kernel();
    }
    
} // namespace base
} // namespace x2d