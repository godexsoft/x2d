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
    small_stuff_spawner_ = config_.create_object("objects.scenary.small_stuff_spawner");    
    cloud_spawner_ = config_.create_object("objects.scenary.cloud_spawner");    
    
    // get all zones and configure them
    scenary_destroyer_ = config_.get_object<zone>("zones.destroy_scenary");
    scenary_destroyer_->set_trigger( boost::bind(&scene::on_destroy_object, this, _1) );
    
    house_destroyer_ = config_.create_object("objects.scenary.scenary_destroyer");
    
    // for debug only
    objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
}

void scene::update(const clock_info& clock) 
{ 
    camera_->position( camera_->position() + glm::vec2(250.0f * clock.delta_time, 0.0f) );
    camera_->zoom( camera_->zoom() - 0.01*clock.delta_time );
}

void scene::on_destroy_object(object& obj)
{
    // we don't really know which one is holding the object :)
    house_spawner_->release(obj);
    small_stuff_spawner_->release(obj);
    cloud_spawner_->release(obj);
}
