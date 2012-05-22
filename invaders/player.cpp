//
//  player.cpp
//  invaders
//
//  Created by Alex Kremer on 5/21/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "player.h"
#include "bullet.h"
#include "log.h"

player::player(kernel& k, configuration& conf, const object_traits& t)
: object(k, conf, t)
, config_(conf)
, fire_(false)
, last_fire_(0.0)
{   
    connect_update();
    connect_render(WORLD_SPACE);
    
    position( glm::vec2(-200, position().y) );
}

void player::fire(bool f)
{
    fire_ = f;
}

void player::move(float accel)
{
    position( glm::vec2(position().x, position().y - accel) );
}

void player::update(const clock_info& clock)
{
    object::update(clock);
    
    if(fire_ && clock.time - last_fire_ > 0.3)
    {
        last_fire_ = clock.time;        
        spawner_->spawn();
    }
}