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

    body::body(kernel& k, configuration& conf, object& obj,
        bool dynamic, bool bullet, bool fixed_rotation,
        const float& linear_damping)
    : kernel_(k)
    , config_(conf)
    , object_(obj)
    , dynamic_(dynamic)
    , body_(world::instance().new_body(glm::vec2(obj.position().x, obj.position().y),
                                       glm::radians(obj.rotation()), dynamic))
    {        
        body_->SetUserData(&object_);
        
        body_->SetBullet(bullet);
        body_->SetFixedRotation(fixed_rotation);
        body_->SetLinearDamping(linear_damping);
    }
    
    body::~body()
    {
        world::instance().destroy_body(body_);
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
        return glm::vec2(position.x / world::instance().global_scale(),
                         position.y / world::instance().global_scale());
    }
    
    void body::position(const glm::vec2& p)
    {
        body_->SetTransform(
            b2Vec2(p.x * world::instance().global_scale(),
                   p.y * world::instance().global_scale()),
                        body_->GetAngle());
    }

    float body::angle() const
    {
        return body_->GetAngle();
    }
    
    size body::get_default_box_size() const
    {
        return object_.box() * object_.scale();
    }
    
    void body::apply_force_to_center(const glm::vec2& f)
    {
        body_->ApplyForceToCenter(b2Vec2(f.x, f.y));
    }
    
    void body::set_linear_velocity(const glm::vec2& v)
    {
        body_->SetLinearVelocity(b2Vec2(v.x, v.y));
    }
    
    glm::vec2 body::get_linear_velocity() const
    {
        b2Vec2 v = body_->GetLinearVelocity();
        return glm::vec2(v.x, v.y);
    }
    
    void body::set_linear_damping(const float& d)
    {
        body_->SetLinearDamping(d);
    }
    
} // namespace physics
} // namespace x2d