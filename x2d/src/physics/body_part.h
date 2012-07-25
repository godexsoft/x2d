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
    
    enum body_part_type
    {
        INVALID_TYPE = 0,
        BOX_TYPE,
        CIRCLE_TYPE
    };
        
    class body_part
    {
    public:
        body_part(configuration& conf,
            const float& density, const float& restitution, const float& friction,
                  const signed short& mask, const signed short& category);
        
    protected:
        configuration&              config_;
        
        float   density_;
        float   restitution_;
        float   friction_;
        signed short   mask_;
        signed short   category_;
    };
    
    class body_part_box
    : public body_part
    {
    public:
        body_part_box(configuration& conf, const boost::shared_ptr<body>& b,
            const float& density, const float& restitution, const float& friction,
            const signed short& mask, const signed short& category,
            const size& bottom_left, const size& top_right);
        
    private:
        size    bl_;
        size    tr_;
    };
    
    class body_part_circle
    : public body_part
    {
    public:
        body_part_circle(configuration& conf, const boost::shared_ptr<body>& b,
            const float& density, const float& restitution, const float& friction,
            const signed short& mask, const signed short& category,
            const float& radius);
        
    private:
        float    radius_;
    };
    
} // namespace physics
} // namespace x2d
using namespace x2d::physics;

#endif // __X2D_BODY_PART_H__
