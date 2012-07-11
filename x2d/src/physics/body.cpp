//
//  body.cpp
//  x2d
//
//  Created by Alex Kremer on 07/08/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "body.h"
#include "world.h"

#include "object.h"

namespace x2d {    
namespace physics {

    body::body(kernel& k, configuration& conf, const object& obj,
        bool dynamic, const glm::vec2& pos, float angle)
    : kernel_(k)
    , config_(conf)
    , object_(obj)
    , dynamic_(dynamic)
    , body_(world::instance().new_body(pos, glm::radians(angle), dynamic))
    {        
        LOG("Setting b2Body userdata to 0x%X", this);
        body_->SetUserData(this);
    }
    
    void body::add_part(const boost::shared_ptr<body_part>& part)
    {
        parts_.push_back(part);        
    }
    
    void body::createFixture(b2FixtureDef* fix)
    {
        body_->CreateFixture(fix);
    }
    
    glm::vec2 body::position() const 
    {
        b2Vec2 position = body_->GetPosition();
        return glm::vec2(position.x, position.y);
    }
    
    void body::position(const glm::vec2& p)
    {
        // TODO: find a way to change position at runtime
    }

    float body::angle() const 
    {
        return body_->GetAngle();
    }
    
    size body::get_default_box_size() const
    {
        return object_.box();
    }
    
} // namespace physics
} // namespace x2d