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
        size& bottomLeft, size& topRight,
        float density, float restitution, float friction) 
    : config_(conf)
    , body_(b)
    , bl_(bottomLeft)
    , tr_(topRight)
    , density_(density)
    , restitution_(restitution)
    , friction_(friction)
    {
        size box = body_->get_default_box_size();
        
        // box must be defined in order to create a shape
        // TODO: throw exception instead?
        assert(box.width != 0 && box.height != 0);
        
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
        shape.SetAsBox(box.width/2, box.height/2);        
        
        b2FixtureDef fix;
        fix.shape = &shape;
        fix.density = density_;
        fix.restitution = restitution_;
        fix.friction = friction_;

        body_->createFixture(&fix);
    }
    
} // namespace physics
} // namespace x2d