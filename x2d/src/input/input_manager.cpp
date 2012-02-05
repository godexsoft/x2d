//
//  input_manager.cpp
//  x2d
//
//  Created by Alex Kremer on 2/4/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "input_manager.h"
#include "kernel.h"

namespace x2d {
namespace input {

    struct get_world_touches 
    : public std::unary_function<const touch, const touch> 
    {
        get_world_touches(kernel& k) 
        : kernel_(k)
        {
        }
        
        const touch operator()(const touch& t) 
        {        
            boost::shared_ptr<viewport> vp = kernel_.get_viewport_at(t.location());
            if(vp)
            {
                // translate point to point inside viewport
                point p = t.location();
                point pp = t.prev_location();

                p.x -= vp->box().origin.x;
                p.y -= vp->box().origin.y;            
                pp.x -= vp->box().origin.x;
                pp.y -= vp->box().origin.y;            
                
                // get world location from point inside viewport
                p = vp->get_world_location(p);
                pp = vp->get_world_location(pp);
                
                return touch(p, pp, t.tap_count(), t.timestamp());
            }
            
            return touch();
        }
        
        kernel& kernel_;
    };
    
    void input_manager::on_touches_began(const std::vector<touch>& touches)
    {
        std::vector<touch> touches_world;        
        touches_world.reserve(touches.size());
        
        std::transform(touches.begin(), touches.end(), 
            std::back_inserter(touches_world), 
                get_world_touches(kernel_));
        
        // remove invalid touches
        touches_world.erase(
            std::remove_if(touches_world.begin(), touches_world.end(), 
                boost::bind(&touch::is_valid, _1) == false), touches_world.end() );
        
        kernel_.dispatch_touches_began(SCREEN_SPACE, touches);
        if(!touches_world.empty())        
            kernel_.dispatch_touches_began(WORLD_SPACE,  touches_world);
    }
    
    void input_manager::on_touches_moved(const std::vector<touch>& touches)
    {
        std::vector<touch> touches_world;        
        touches_world.reserve(touches.size());
        
        std::transform(touches.begin(), touches.end(), 
            std::back_inserter(touches_world), 
                get_world_touches(kernel_));
        
        // remove invalid touches
        touches_world.erase(
            std::remove_if(touches_world.begin(), touches_world.end(), 
                boost::bind(&touch::is_valid, _1) == false), touches_world.end() );
        
        kernel_.dispatch_touches_moved(SCREEN_SPACE, touches);        
        if(!touches_world.empty())        
            kernel_.dispatch_touches_moved(WORLD_SPACE,  touches_world);
    }
    
    void input_manager::on_touches_ended(const std::vector<touch>& touches)
    {
        std::vector<touch> touches_world;        
        touches_world.reserve(touches.size());
        
        std::transform(touches.begin(), touches.end(), 
            std::back_inserter(touches_world), 
                get_world_touches(kernel_));
        
        // remove invalid touches
        touches_world.erase(
            std::remove_if(touches_world.begin(), touches_world.end(), 
                boost::bind(&touch::is_valid, _1) == false), touches_world.end() );
        
        kernel_.dispatch_touches_ended(SCREEN_SPACE, touches);
        if(!touches_world.empty())        
            kernel_.dispatch_touches_ended(WORLD_SPACE,  touches_world);
    }

} // namespace input
} // namespace x2d
