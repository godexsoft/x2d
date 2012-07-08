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
#include <boost/shared_ptr.hpp>

namespace x2d { 
    
    class object;
    
    namespace config {
        
        class configuration;
        
    } // namespace config
    using namespace x2d::config;
    
namespace physics {

    class body_part
    {
    public:
        body_part(configuration& conf, std::string obj_key = "");
        
    private:
        configuration&  config_;

        boost::shared_ptr<object> object_;        
    };
    
} // namespace physics
} // namespace x2d
using namespace x2d::physics;

#endif // __X2D_BODY_PART_H__
