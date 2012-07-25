//
//  world.cpp
//  x2d
//
//  Created by Alex Kremer on 7/8/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "world.h"
#include "log.h"

namespace x2d {
namespace physics {

    world_bare::world_bare()
    : world_(b2Vec2(0.0f, -10.0f))
    {
        world_.SetContactListener(&contact_listener_);
    }
    
    float world_bare::global_scale() const
    {
        return 0.1f;
    }
    
    void world_bare::set_gravity(const glm::vec2& g)
    {
        world_.SetGravity(b2Vec2(g.x, g.y));
    }
    
} // namespace physics
} // namespace x2d
