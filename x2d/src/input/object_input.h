//
//  object_input.h
//  x2d
//
//  Created by Alex Kremer on 7/26/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_OBJECT_INPUT_H__
#define __X2D_OBJECT_INPUT_H__

#include "base_object.h"
#include "glm.hpp"
#include "log.h"

#include <deque>

namespace x2d {

    // forward declare kernel to make friends
    class kernel;
    
namespace input {
    
    class object_input_manager
    : public base_object
    {
    public:
        object_input_manager(kernel& k)
        : base_object(k)
        {
            connect_touch_input(CAMERA_SPACE);
        }
        
        void register_object(object* obj);
        void deregister_object(object* obj);
            
        void touch_input_began(space s, const std::vector<touch>& touches);
        void touch_input_moved(space s, const std::vector<touch>& touches);        
        void touch_input_ended(space s, const std::vector<touch>& touches);
        
    private:
        
        enum input_state
        {
            RELEASED = 0,
            PRESSED,
        };
        
        typedef std::map<object*, input_state> obj_map;
        obj_map objects_;
    };
    
} // namespace input
} // namespace x2d
using namespace x2d::input;

#endif // __X2D_OBJECT_INPUT_H__
