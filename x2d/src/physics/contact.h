//
//  contact.h
//  x2d
//
//  Created by Alex Kremer on 7/17/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_CONTACT_H__
#define __X2D_CONTACT_H__

#include <boost/shared_ptr.hpp>

#include "Box2D/Box2D.h"
#include "glm.hpp"

namespace x2d {
namespace physics {
    
    /**
     * @brief box2d-based Contact listener
     */
    class contact_listener
    : public b2ContactListener
    {
    public:
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
        void PreSolve(b2Contact* contact, const b2Manifold* old_manifold);
        void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    };
    
} // namespace physics
} // namespace x2d
using namespace x2d::physics;

#endif // __X2D_CONTACT_H__
