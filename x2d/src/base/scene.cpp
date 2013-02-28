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
    , on_transition_from_(config_)
    , on_transition_to_(config_)
    {
        LOG("Scene - constructor");
        
        if(!t.on_transition_from.empty())
        {
            on_transition_from_.set_script(config_.create_sys_object<script>(t.on_transition_from));
        }
        
        if(!t.on_transition_to.empty())
        {
            on_transition_to_.set_script(config_.create_sys_object<script>(t.on_transition_to));
        }
    }
    
    void scene::on_transition_from()
    {
        if(on_transition_from_)
        {
            LOG("Scene - on transition from - call user defined script/callback");
            on_transition_from_(this);
        }
        else
        {
            LOG("Scene - on transition from default behavior");
            destroy_self(); // kill ASAP by default
        }
    }
    
    void scene::on_transition_to()
    {
        if(on_transition_to_)
        {
            LOG("Scene - on transition to - call user defined script/callback");
            on_transition_to_(this);
        }
        else
        {
            LOG("Scene - on transition to default behavior");
        }
    }

    configuration& scene::get_config() const
    {
        return config_;
    }
    
    kernel& scene::get_kernel() const
    {
        return config_.get_kernel();
    }
    
    void scene::switch_to(const boost::shared_ptr<scene>& sc) const
    {
        config_.switch_to(sc);
    }
    
} // namespace base
} // namespace x2d