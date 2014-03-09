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
#include <vector>

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
        POLYGON_TYPE,
        CIRCLE_TYPE
    };
        
    class body_part
    {
    public:
        body_part(configuration& conf,
            const float& density, const float& restitution, const float& friction,
                  const uint16_t& mask, const uint16_t& category, bool is_sensor);
        
    protected:
        configuration&              config_;
        
        float   density_;
        float   restitution_;
        float   friction_;
        uint16_t   mask_;
        uint16_t   category_;
        bool    is_sensor_;
    };
    
    class body_part_box
    : public body_part
    {
    public:
        body_part_box(configuration& conf, const boost::shared_ptr<body>& b,
            const float& density, const float& restitution, const float& friction,
            const uint16_t & mask, const uint16_t& category,
            const size& bottom_left, const size& top_right, bool is_sensor);
        
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
            const uint16_t& mask, const uint16_t& category,
            const float& radius, bool is_sensor);
        
    private:
        float    radius_;
    };

    class body_part_polygon
    : public body_part
    {
    public:
        body_part_polygon(configuration& conf, const boost::shared_ptr<body>& b,
            const float& density, const float& restitution, const float& friction,
            const uint16_t& mask, const uint16_t& category,
            const std::vector<float>& points, bool is_sensor);
        
    private:
        std::vector<float>    points_;
    };
    
} // namespace physics
} // namespace x2d
using namespace x2d::physics;

#endif // __X2D_BODY_PART_H__
