//
//  body_part.cpp
//  x2d
//
//  Created by Alex Kremer on 07/08/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "body_part.h"
#include "configuration.h"
#include "world.h"

namespace x2d {
namespace physics {

    body_part::body_part(configuration& conf, const std::string& ident,
        const float& density, const float& restitution, const float& friction,
        const uint16_t& mask, const uint16_t& category, bool is_sensor)
    : config_(conf)
    , ident_(ident)
    , density_(density)
    , restitution_(restitution)
    , friction_(friction)
    , mask_(mask)
    , category_(category)
    , is_sensor_(is_sensor)
    {
    }

    body_part_box::body_part_box(configuration& conf, const std::string& ident, const boost::shared_ptr<body>& b,
        const float& density, const float& restitution, const float& friction,
        const uint16_t& mask, const uint16_t& category,
        const size& bottom_left, const size& top_right, bool is_sensor)
    : body_part(conf, ident, density, restitution, friction, mask, category, is_sensor)
    , bl_(bottom_left)
    , tr_(top_right)
    {
        size box = b->get_default_box_size();
        
        // box must be defined in order to create a shape
        // TODO: throw exception instead?
        assert(box.width != 0 && box.height != 0);
        
        b2FixtureDef fix;
        fix.density = density_;
        fix.restitution = restitution_;
        fix.friction = friction_;
        fix.isSensor = is_sensor_;

        if(category_ != 0)
        {
            fix.filter.categoryBits = category_;
        }
        
        if(mask_ != 0)
        {
            fix.filter.maskBits = mask_;
        }
        
        if(bl_.width != 0 && bl_.height != 0)
        {
            // negative coordinates
            box.width += bl_.width;
            box.height += bl_.height;
        }
        
        if(tr_.width != 0 && tr_.height != 0) 
        {
            // positive coordinates
            box.width -= bl_.width;
            box.height -= bl_.height;
        }
        
        b2PolygonShape shape;
        shape.SetAsBox(box.width * world::instance().global_scale()/2,
                       box.height* world::instance().global_scale()/2);
        fix.shape = &shape;
        fix.userData = &ident_;
        
        b->createFixture(&fix);
    }
    
    body_part_circle::body_part_circle(configuration& conf, const std::string& ident,
        const boost::shared_ptr<body>& b, const float& density, const float& restitution,
        const float& friction, const uint16_t& mask, const uint16_t& category,
        const float& x, const float& y, const float& radius, bool is_sensor)
    : body_part(conf, ident, density, restitution, friction, mask, category, is_sensor)
    , x_(x)
    , y_(y)
    , radius_(radius)
    {
        b2FixtureDef fix;
        fix.density = density_;
        fix.restitution = restitution_;
        fix.friction = friction_;
        fix.isSensor = is_sensor_;
        
        if(category_ != 0)
        {
            fix.filter.categoryBits = category_;
        }
        
        if(mask_ != 0)
        {
            fix.filter.maskBits = mask_;
        }
        
        if(radius_ == 0.0f)
        {
            size box = b->get_default_box_size();
            
            // box must be defined in order to create a shape
            // TODO: throw exception instead?
            assert(box.width != 0 && box.height != 0);
            
            // half of the largest side
            radius_ = fmaxf(box.width, box.height) / 2.0f;
        }
        
        b2CircleShape shape;
        shape.m_radius = radius_ * world::instance().global_scale();
        shape.m_p = b2Vec2(x_ * world::instance().global_scale(),
                           y_ * world::instance().global_scale());
        
        fix.shape = &shape;
        fix.userData = &ident_;
        
        b->createFixture(&fix);
    }
    
    
    body_part_polygon::body_part_polygon(configuration& conf, const std::string& ident,
        const boost::shared_ptr<body>& b, const float& density, const float& restitution,
        const float& friction, const uint16_t& mask, const uint16_t& category,
        const std::vector<float>& points, bool is_sensor)
    : body_part(conf, ident, density, restitution, friction, mask, category, is_sensor)
    , points_(points)
    {
        b2FixtureDef fix;
        fix.density = density_;
        fix.restitution = restitution_;
        fix.friction = friction_;
        fix.isSensor = is_sensor_;
        
        if(category_ != 0)
        {
            fix.filter.categoryBits = category_;
        }
        
        if(mask_ != 0)
        {
            fix.filter.maskBits = mask_;
        }
        
        b2PolygonShape shape;
        shape.m_vertexCount = points_.size()/2;
        
        int j = 0;        
        for(int i = 0; i < points_.size() / 2; ++i)
        {
            float x = points_[j++] - b->get_pivot().x;
            float y = points_[j++] - b->get_pivot().y;
            
            x *= world::instance().global_scale();
            y *= world::instance().global_scale();
            
            shape.m_vertices[i].Set(x, y);
        }
        
        fix.shape = &shape;
        fix.userData = &ident_;
        
        b->createFixture(&fix);
    }
    
} // namespace physics
} // namespace x2d