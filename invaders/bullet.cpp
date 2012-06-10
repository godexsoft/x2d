//
//  bullet.cpp
//  invaders
//
//  Created by Alex Kremer on 5/21/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "bullet.h"
#include "log.h"

bullet::bullet(kernel& k, configuration& conf, const object_traits& t)
: object(k, conf, t)
, config_(conf)
{   
    connect_update();
    connect_render(WORLD_SPACE);
}

void bullet::update(const clock_info& clock)
{
    object::update(clock);
    
    position( glm::vec2( position().x + clock.delta_time * 500.0, position().y ) );
}