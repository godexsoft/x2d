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

#include <boost/thread/detail/singleton.hpp>
#include <boost/shared_ptr.hpp>

#include "Box2D/Box2D.h"
#include "glm.hpp"

#include "contact.h"
#include "clock.h"
#include "config.h"
#include "body.h"

namespace x2d {
namespace physics {
    
    /**
     * @brief World wrapper
     */
    class world_bare
    {
        friend class boost::detail::thread::singleton<world_bare>;
        
    public:
        b2Body* new_body(const glm::vec2& pos, float angle=0, body_type btype = DYNAMIC_BODY)
        {
            b2BodyDef def;
            switch(btype)
            {
                case STATIC_BODY:
                    def.type = b2_staticBody;
                    break;
                case KINEMATIC_BODY:
                    def.type = b2_kinematicBody;
                    break;
                case DYNAMIC_BODY:
                    def.type = b2_dynamicBody;
                    break;
            }
            
            def.position.Set(pos.x * global_scale(),
                             pos.y * global_scale());

            if(angle!=0) 
            {
                def.angle = angle;
            }
            
            return world_.CreateBody(&def);
        }
        
        void destroy_body(b2Body* b)
        {
            world_.DestroyBody(b);
        }

        void update(const clock_info& ci) 
        {
            world_.Step(1.0f/60.f, 8, 3);
        }
        
        void set_gravity(const glm::vec2& g);
        
        const glm::vec2 get_gravity() const;
        
        void set_allow_sleeping(bool f);
        
        float global_scale() const;
        
        void set_contact_listener(b2ContactListener& cl);
        
    private:
        world_bare();
        
        b2World          world_;
        contact_listener contact_listener_;
    };
    
    typedef boost::detail::thread::singleton<world_bare> world;
    
} // namespace physics
} // namespace x2d
using namespace x2d::physics;

#endif // __X2D_WORLD_H__
