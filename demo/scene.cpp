//
//  scene.cpp
//  x2d
//
//  Created by Alex Kremer on 1/31/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "scene.h"

scene::scene(kernel& k, configuration& conf)
: base_object(k)
, config_(conf)
{   
    connect_update();
    
    camera_ = config_.get_object<camera>("camera");
    
    objects_.push_back( config_.create_object("objects.background") );
    objects_.push_back( config_.create_object("objects.scenary.road") );
    objects_.push_back( config_.create_object("objects.player") );
    
    // create the spawners
    house_spawner_ = config_.create_object("objects.scenary.house_spawner");
    
    // for debug only
    objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
}

void scene::update(const clock_info& clock) 
{ 
    camera_->position( camera_->position() + glm::vec2(50.0f * clock.delta_time, 0.0f) );
}

