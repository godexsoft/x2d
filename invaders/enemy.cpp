//
//  enemy.cpp
//  invaders
//
//  Created by Alex Kremer on 6/10/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "enemy.h"
#include "log.h"

enemy::enemy(kernel& k, configuration& conf, const object_traits& t)
: object(k, conf, t)
, config_(conf)
, y_from_(conf.get_value<float>("settings.enemy_offset_from"))
, y_to_(conf.get_value<float>("settings.enemy_offset_to"))
, roundtrip_time_(conf.get_value<float>("settings.enemy_roundtrip_time"))
, inter_(y_, y_from_, y_to_, y_from_, roundtrip_time_)
{   
    connect_update();
    connect_render(WORLD_SPACE);
    
    position( glm::vec2(0, y_) );
}

void enemy::update(const clock_info& clock)
{
    inter_.update(clock.delta_time);
    position( glm::vec2( position().x - 100*clock.delta_time, y_) );
    
    object::update(clock);
}