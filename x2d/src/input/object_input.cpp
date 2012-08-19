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
        objects_.insert(std::make_pair(obj, RELEASED));
    }
    
    void object_input_manager::deregister_object(object* obj)
    {
        LOG("-- 0x%X Deregistering object from input", obj);
        objects_.erase(obj);
    }
    
    void object_input_manager::touch_input_began(space s, const std::vector<touch>& touches)
    {
        for(obj_map::iterator it = objects_.begin(); it != objects_.end(); ++it)
        {
            bbox bx = it->first->screen_bbox();
            
            for(std::vector<touch>::const_iterator tit = touches.begin();
                tit != touches.end(); ++tit)
            {
                if(bx.contains_point(tit->location()))
                {
                    // TODO: report vec2 inside box of object
                    it->second = PRESSED;
                    it->first->on_input_began(glm::vec2(tit->location().x, tit->location().y));
                }
            }
        }
    }
    
    void object_input_manager::touch_input_moved(space s, const std::vector<touch>& touches)
    {
        for(obj_map::iterator it = objects_.begin(); it != objects_.end(); ++it)
        {
            bbox bx = it->first->screen_bbox();
            
            for(std::vector<touch>::const_iterator tit = touches.begin();
                tit != touches.end(); ++tit)
            {
                if(bx.contains_point(tit->location()))
                {
                    // slide in
                    if(it->second == RELEASED)
                    {
                        // TODO: report vec2 inside box of object
                        it->second = PRESSED;
                        it->first->on_input_began(glm::vec2(tit->location().x, tit->location().y));
                    }
                    else
                    {
                        // finger sliding inside object
                        it->first->on_input_moved(glm::vec2(tit->location().x, tit->location().y));
                    }
                }
                else if(bx.contains_point(tit->prev_location()))
                {
                    // slide out
                    if(it->second == PRESSED)
                    {
                        it->first->on_input_ended(glm::vec2(tit->location().x, tit->location().y));
                        it->second = RELEASED;
                    }
                }
            }
        }
    }

    void object_input_manager::touch_input_ended(space s, const std::vector<touch>& touches)
    {
        for(obj_map::iterator it = objects_.begin(); it != objects_.end(); ++it)
        {
            bbox bx = it->first->screen_bbox();
            
            for(std::vector<touch>::const_iterator tit = touches.begin();
                tit != touches.end(); ++tit)
            {
                if(bx.contains_point(tit->location()))
                {
                    if(it->second == PRESSED)
                    {
                        // TODO: report vec2 inside box of object
                        it->second = RELEASED;
                        it->first->on_input_ended(glm::vec2(tit->location().x, tit->location().y));
                    }
                }
            }
        }
    }

} // namespace input
} // namespace x2d
