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
    
    class object;
    
namespace physics {
    
    /**
     * @brief contains x2d contact information
     */
    class contact_info
    {
    public:
        contact_info(const std::string& ident, const boost::shared_ptr<object>& obj)
        : ident_(ident)
        , object_(obj)
        {}
        
        inline const boost::shared_ptr<object> get_object() const
        {
            return object_;
        }
        
        inline const std::string& get_ident() const
        {
            return ident_;
        }
        
    private:
        std::string ident_;
        boost::shared_ptr<object> object_;
    };
    
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
