//
//  body.cpp
//  x2d
//
//  Created by Alex Kremer on 07/08/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "body.h"
#include "world.h"

namespace x2d {    
namespace physics {

    body::body(kernel& k, configuration& conf, bool dynamic, 
        const glm::vec2& pos, float angle)
    : kernel_(k)
    , config_(conf)
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

    float body::angle() const 
    {
        return body_->GetAngle();
    }
    
} // namespace physics
} // namespace x2d