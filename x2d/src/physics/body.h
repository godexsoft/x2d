//
//  body.h
//  x2d
//
//  Created by Alex Kremer on 07/08/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_BODY_H__
#define __X2D_BODY_H__

#include <vector>

#include "log.h"
#include "kernel.h"

#include "body_part.h"
#include "glm.hpp"
#include "Box2D/Box2D.h"

namespace x2d { 
        
    class object;
        
namespace config {
            
    class configuration;
            
} // namespace config
using namespace x2d::config;

namespace physics {

    class body
    {
    public:
        body(kernel& k, configuration& conf, object& obj,
             bool dynamic, bool bullet, bool fixed_rotation,
             const float& linear_damping);
        
        ~body();
        
        void add_part(const boost::shared_ptr<body_part>& part);
        
        void createFixture(b2FixtureDef* fix);
        
        glm::vec2 position() const;
        void position(const glm::vec2& p);        
        
        float angle() const;
        
        size get_default_box_size() const;
        
        // apply/set stuff
        void apply_force_to_center(const glm::vec2& f);
        
        void set_linear_velocity(const glm::vec2& v);
        glm::vec2 get_linear_velocity() const;
        
        void set_linear_damping(const float& d);
        
    private:
        typedef std::vector<boost::shared_ptr<body_part> > parts_vec;
        
        kernel&         kernel_;
        configuration&  config_;        
        object&         object_;

        bool         dynamic_;
        
        b2Body      *body_;
        parts_vec    parts_;
    };
    
} // namespace physics
} // namespace x2d
using namespace x2d::physics;

#endif // __X2D_BODY_H__
