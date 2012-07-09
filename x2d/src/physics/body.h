//
//  body.h
//  x2d
//
//  Created by Alex Kremer on 07/08/2012.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_BODY_H__
#define __X2D_BODY_H__

#include <vector>

#include "log.h"
#include "kernel.h"

#include "body_part.h"
#include "glm.hpp"
#include "Box2D/Box2D.h"

namespace x2d { 
        
    class object;
        
namespace config {
            
    class configuration;
            
} // namespace config
using namespace x2d::config;

namespace physics {

    class body
    {
    public:
        body(kernel& k, configuration& conf, bool dynamic);
        
        void add_part(const boost::shared_ptr<body_part>& part);
        
        glm::vec2 position() const;
        float angle() const;
        
    private:
        typedef std::vector<boost::shared_ptr<body_part> > parts_vec;
        
        kernel&         kernel_;
        configuration&  config_;

        bool         dynamic_;
        
        b2Body      *body_;
        parts_vec    parts_;
    };
    
} // namespace physics
} // namespace x2d
using namespace x2d::physics;

#endif // __X2D_BODY_H__
