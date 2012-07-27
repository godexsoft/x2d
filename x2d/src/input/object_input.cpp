//
//  object_input.cpp
//  x2d
//
//  Created by Alex Kremer on 7/26/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "object_input.h"
#include "object.h"

namespace x2d {
namespace input {
    
    void object_input_manager::register_object(object* obj)
    {
        LOG("-- 0x%X Registering object for input", obj);
        objects_.push_back(obj);
    }
    
    void object_input_manager::deregister_object(object* obj)
    {
        LOG("-- 0x%X Deregistering object from input", obj);
        objects_.erase( std::remove(
            objects_.begin(),
            objects_.end(),
                obj),
                    objects_.end());
    }
    
    void object_input_manager::touch_input_began(space s, const std::vector<touch>& touches)
    {
        for(obj_deque::iterator it = objects_.begin(); it != objects_.end(); ++it)
        {
            // TODO: use pivot and offset of rect
            glm::vec3 pos = (*it)->world_position();
            size b = (*it)->box();
            rect r(pos.x, pos.y, b.width, b.height);
                
            for(std::vector<touch>::const_iterator tit = touches.begin();
                tit != touches.end(); ++tit)
            {
                if(r.contains_point(tit->location()))
                {
                    // TODO: report vec2 inside box of object
                    (*it)->on_input(glm::vec2(tit->location().x, tit->location().y));
                }
            }
        }
    }
    
    void object_input_manager::touch_input_moved(space s, const std::vector<touch>& touches)
    {
        
    }
    
} // namespace input
} // namespace x2d
