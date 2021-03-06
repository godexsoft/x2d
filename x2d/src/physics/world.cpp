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
    
    void world_bare::set_allow_sleeping(bool f)
    {
        world_.SetAllowSleeping(f);
    }
    
    const glm::vec2 world_bare::get_gravity() const
    {
        b2Vec2 g = world_.GetGravity();
        return glm::vec2(g.x, g.y);
    }
    
    void world_bare::set_contact_listener(b2ContactListener& cl)
    {
        world_.SetContactListener(&cl);
    }
} // namespace physics
} // namespace x2d
