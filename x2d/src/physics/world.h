//
//  world.h
//  x2d
//
//  Created by Alex Kremer on 7/8/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_WORLD_H__
#define __X2D_WORLD_H__

#include <boost/pool/detail/singleton.hpp>
#include <boost/shared_ptr.hpp>

#include "Box2D/Box2D.h"

#include "clock.h"
#include "config.h"

namespace x2d {
namespace physics {
    
    /**
     * @brief World wrapper
     */
    class world_bare
    {
        friend class boost::details::pool::singleton_default<world_bare>;
        
    public:
        b2Body* new_body()
        {
            b2BodyDef def;
            def.type = b2_dynamicBody;            
            def.position.Set(0.0f, 4.0f);
            
            return world_.CreateBody(&def);
        }        

        void update(const clock_info& ci) 
        {
            world_.Step(1.0f/60.f, 8, 3);
        }
        
    private:
        world_bare()
        : world_(b2Vec2(0.0f, -10.0f))
        {        
        }
        
        b2World world_;
    };
    
    typedef boost::details::pool::singleton_default<world_bare> world;
    
} // namespace physics
} // namespace x2d
using namespace x2d::physics;

#endif // __X2D_WORLD_H__
