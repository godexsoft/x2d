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
, player_( conf.create_object<player>("objects.player") )
, platform_width_( conf.get_value<float>("objects.scenary.platform.width") )
{       
    connect_update();    
    camera_ = config_.get_object<camera>("camera");
        
    // get all zones and configure them
    crash_zone_ = config_.get_object<zone>("zones.crash_or_land");
    crash_zone_->set_trigger( boost::bind(&scene::on_land, this, _1) );

    // create all scenary objects
    objects_.push_back( config_.create_object("objects.background") );
    objects_.push_back( config_.create_object("objects.scenary.moon_surface") );
    platform_ = config_.create_object("objects.scenary.platform");
    fuel_bar_ = config_.create_object("objects.ui.fuel_bar");
    fuel_bar_filling_ = fuel_bar_->child_by_name("fuel_bar_filling");
    
    // for debug only
    objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
}

void scene::update(const clock_info& clock) 
{ 
    // update size of fuel filling
    fuel_bar_filling_->box( size(1.0f, player_->fuel_percent()) );
}

void scene::on_land(object& obj)
{
    float player_x = player_->world_position().x;
    float platform_x = platform_->world_position().x;
    
    bool on_platform = false;
    if( fabsf(player_x - platform_x) < platform_width_/2.0f )
        on_platform = true;
    
    if( player_->landing_allowed() && on_platform )
    {
        player_->finish();
    } 
    else
    {     
        player_->crash();
    }
}
