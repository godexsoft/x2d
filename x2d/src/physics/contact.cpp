//
//  contact.cpp
//  x2d
//
//  Created by Alex Kremer on 7/17/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "contact.h"
#include "object.h"
#include "log.h"

namespace x2d {
namespace physics {

    void contact_listener::BeginContact(b2Contact* contact)
    {
        b2Body* b1 = contact->GetFixtureA()->GetBody();
        b2Body* b2 = contact->GetFixtureB()->GetBody();
        
        object* o1 = static_cast<object*>(b1->GetUserData());
        object* o2 = static_cast<object*>(b2->GetUserData());
        
        LOG("Begin contact for objects 0x%X and 0x%X", o1, o2);
    }
    
    void contact_listener::EndContact(b2Contact* contact)
    {
        LOG("End contact 0x%X", contact);
    }
    
    void contact_listener::PreSolve(b2Contact* contact, const b2Manifold* old_manifold)
    {
    }
    
    void contact_listener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
    {        
    }
    
} // namespace physics
} // namespace x2d
