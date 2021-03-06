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
        b2Fixture* f1 = contact->GetFixtureA();
        b2Fixture* f2 = contact->GetFixtureB();
        
        b2Body* b1 = f1->GetBody();
        b2Body* b2 = f2->GetBody();
        
        object* o1 = static_cast<object*>(b1->GetUserData());
        object* o2 = static_cast<object*>(b2->GetUserData());
        
        // fixture collisions first
        o1->on_collision_begin(*static_cast<std::string*>(f1->GetUserData()),
                               *static_cast<std::string*>(f2->GetUserData()) );
        o1->on_collision_begin(o2);

        o2->on_collision_begin(*static_cast<std::string*>(f2->GetUserData()),
                               *static_cast<std::string*>(f1->GetUserData()) );
        o2->on_collision_begin(o1);
    }
    
    void contact_listener::EndContact(b2Contact* contact)
    {
        b2Fixture* f1 = contact->GetFixtureA();
        b2Fixture* f2 = contact->GetFixtureB();
        
        b2Body* b1 = f1->GetBody();
        b2Body* b2 = f2->GetBody();
        
        object* o1 = static_cast<object*>(b1->GetUserData());
        object* o2 = static_cast<object*>(b2->GetUserData());
        
        o1->on_collision_end(*static_cast<std::string*>(f1->GetUserData()),
                             *static_cast<std::string*>(f2->GetUserData()) );
        o1->on_collision_end(o2);
        
        o2->on_collision_end(*static_cast<std::string*>(f2->GetUserData()),
                             *static_cast<std::string*>(f1->GetUserData()) );
        o2->on_collision_end(o1);
    }
    
    void contact_listener::PreSolve(b2Contact* contact, const b2Manifold* old_manifold)
    {
        b2Fixture* f1 = contact->GetFixtureA();
        b2Fixture* f2 = contact->GetFixtureB();
        
        b2Body* b1 = f1->GetBody();
        b2Body* b2 = f2->GetBody();
        
        object* o1 = static_cast<object*>(b1->GetUserData());
        object* o2 = static_cast<object*>(b2->GetUserData());

        if(!o1->on_collision_presolve(*static_cast<std::string*>(f1->GetUserData()),
                                      *static_cast<std::string*>(f2->GetUserData()), o2 ))
        {
            contact->SetEnabled(false);
        }
        
        if(!o2->on_collision_presolve(*static_cast<std::string*>(f2->GetUserData()),
                                      *static_cast<std::string*>(f1->GetUserData()), o1 ))
        {
            contact->SetEnabled(false);   
        }
    }
    
    void contact_listener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
    {        
    }
    
} // namespace physics
} // namespace x2d
