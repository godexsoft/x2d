//
//  body_part.cpp
//  x2d
//
//  Created by Alex Kremer on 07/08/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "body_part.h"
#include "configuration.h"

namespace x2d {    
namespace physics {

    body_part::body_part(configuration& conf, const boost::shared_ptr<body>& b,
        size& bottomLeft, size& topRight) 
    : config_(conf)
    , body_(b)
    , bl_(bottomLeft)
    , tr_(topRight)
    {
        b2PolygonShape shape;
        shape.SetAsBox(bl_.width, bl_.height);        
        
        b2FixtureDef fix;
        fix.shape = &shape;
        fix.density = 1.0f;
        fix.restitution = 0.8f;
        fix.friction = 0.1f;

        body_->createFixture(&fix);
    }
    
} // namespace physics
} // namespace x2d