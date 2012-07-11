//
//  scene.h
//  x2d
//
//  Created by Alex Kremer on 1/31/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifndef __SCENE_H__
#define __SCENE_H__

#include "configuration.h"
#include "fps_counter.h"

#include <vector>

class scene 
: public base_object
{
public:
    scene(kernel& k, configuration& conf)
    : base_object(k)
    , config_(conf)
    {
        connect_touch_input(WORLD_SPACE);

        objects_.push_back( config_.create_object("floor_obj") );
        objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
    }
    
    void touch_input_began(space s, const std::vector<touch>& touches) 
    {
        boost::shared_ptr<object> obj = config_.create_object("box_obj");
        
        glm::vec2 pos;
        point p = touches.at(0).location();
        pos.x = p.x;
        pos.y = p.y;
        
        obj->position(pos);
        objects_.push_back( obj );
    }

    
private:
    configuration&  config_;
    std::vector< boost::shared_ptr<base_object> >   objects_;
};

#endif // __SCENE_H__
