//
//  hud.cpp
//  x2d
//
//  Created by Alex Kremer on 4/28/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//
//  HAPPY BIRTHDAY, EGOR!!! :-)
//

#include "hud.h"
#include "log.h"

hud::hud(kernel& k, configuration& conf, const object_traits& t)
: object(k, conf, t)
, config_(conf)
{   
    connect_update();
    connect_render(WORLD_SPACE);
 
    fuel_bar_filling_ = child_by_name("fuel_bar")->child_by_name("fuel_bar_filling");
    orientation_icon_ = child_by_name("orientation_meter")->child_by_name("orientation_meter_icon");
    velocity_icon_ =    child_by_name("velocity_meter")->child_by_name("velocity_meter_icon");
    
    red_ =   config_.get_object<sprite>("graphics.all.light_red");
    green_ = config_.get_object<sprite>("graphics.all.light_green");
}

void hud::update(const clock_info& clock)
{    
    object::update(clock);
}

void hud::update_fuel(float f)
{
    // update size of fuel filling
    fuel_bar_filling_->box( size(1.0f, f) );
}

void hud::update_indicators(bool orientation, bool velocity)
{
    if(orientation)
    {
        orientation_icon_->set_sprite( green_ );
    }
    else
    {
        orientation_icon_->set_sprite( red_ );
    }
    
    if(velocity)
    {
        velocity_icon_->set_sprite( green_ );
    }
    else
    {
        velocity_icon_->set_sprite( red_ );
    }
}