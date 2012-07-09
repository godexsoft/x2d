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

    body::body(kernel& k, configuration& conf, bool dynamic)
    : kernel_(k)
    , config_(conf)
    , dynamic_(dynamic)
    , body_(world::instance().new_body())
    {        
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(1.0f, 1.0f);
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        
        body_->CreateFixture(&fixtureDef);
        
        LOG("Setting b2Body userdata to 0x%X", this);
        body_->SetUserData(this);
    }
    
    void body::add_part(const boost::shared_ptr<body_part>& part)
    {
        parts_.push_back(part);
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