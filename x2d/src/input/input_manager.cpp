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
    
    struct get_camera_touches
    : public std::unary_function<const touch, const touch>
    {
        get_camera_touches(kernel& k)
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
                
                // translate screen-space pixel size to viewport-space pixel size
                float xs = vp->get_camera()->frustum().width / vp->box().area.width;
                float ys = vp->get_camera()->frustum().height / vp->box().area.height;
   
                return touch(point(p.x*xs, p.y*ys), point(pp.x*xs, pp.y*ys),
                             t.tap_count(), t.timestamp());
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
        
        std::vector<touch> touches_camera;
        touches_camera.reserve(touches.size());
        
        std::transform(touches.begin(), touches.end(),
            std::back_inserter(touches_camera),
                get_camera_touches(kernel_));
        
        // remove invalid touches
        touches_camera.erase(
            std::remove_if(touches_camera.begin(), touches_camera.end(),
                boost::bind(&touch::is_valid, _1) == false), touches_camera.end() );
        
        kernel_.dispatch_touches_began(SCREEN_SPACE, touches);
        
        if(!touches_world.empty())
            kernel_.dispatch_touches_began(WORLD_SPACE,  touches_world);
        
        if(!touches_camera.empty())
            kernel_.dispatch_touches_began(CAMERA_SPACE,  touches_camera);
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
        
        std::vector<touch> touches_camera;
        touches_camera.reserve(touches.size());
        
        std::transform(touches.begin(), touches.end(),
            std::back_inserter(touches_camera),
                get_camera_touches(kernel_));
        
        // remove invalid touches
        touches_camera.erase(
            std::remove_if(touches_camera.begin(), touches_camera.end(),
                boost::bind(&touch::is_valid, _1) == false), touches_camera.end() );
        
        kernel_.dispatch_touches_moved(SCREEN_SPACE, touches);
        
        if(!touches_world.empty())
            kernel_.dispatch_touches_moved(WORLD_SPACE,  touches_world);
        
        if(!touches_camera.empty())
            kernel_.dispatch_touches_moved(CAMERA_SPACE,  touches_camera);
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
        
        std::vector<touch> touches_camera;
        touches_camera.reserve(touches.size());
        
        std::transform(touches.begin(), touches.end(),
            std::back_inserter(touches_camera),
                get_camera_touches(kernel_));
        
        // remove invalid touches
        touches_camera.erase(
            std::remove_if(touches_camera.begin(), touches_camera.end(),
                boost::bind(&touch::is_valid, _1) == false), touches_camera.end() );
        
        kernel_.dispatch_touches_ended(SCREEN_SPACE, touches);
        
        if(!touches_world.empty())
            kernel_.dispatch_touches_ended(WORLD_SPACE,  touches_world);
        
        if(!touches_camera.empty())
            kernel_.dispatch_touches_ended(CAMERA_SPACE,  touches_camera);
    }
    
    void input_manager::on_acceleration(float x, float y, float z)
    {
        kernel_.dispatch_accelerometer_input( glm::vec3(x, y, z) );
    }

} // namespace input
} // namespace x2d
