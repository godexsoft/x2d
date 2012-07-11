//
//  body_part.h
//  x2d
//
//  Created by Alex Kremer on 07/08/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_BODY_PART_H__
#define __X2D_BODY_PART_H__

#include "log.h"
#include "math_util.h"
#include <boost/shared_ptr.hpp>

namespace x2d {     
namespace config {
        
    class configuration;
        
} // namespace config
using namespace x2d::config;
    
namespace physics {

    class body;
    
    class body_part
    {
    public:
        body_part(configuration& conf, const boost::shared_ptr<body>& b,
            size& bottomLeft, size& topRight,
            float density, float restitution, float friction);
        
    private:
        configuration&              config_;
        boost::shared_ptr<body>     body_;
        
        size    bl_;
        size    tr_;
        float   density_;
        float   restitution_;
        float   friction_;
    };
    
} // namespace physics
} // namespace x2d
using namespace x2d::physics;

#endif // __X2D_BODY_PART_H__
