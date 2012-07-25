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
    , spwnr_(config_.create_sys_object<spawner>("spawner"))
    {
        connect_touch_input(WORLD_SPACE);

        config_.create_object("floor_obj");
        
#ifdef DEBUG
        objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
#endif
    }
    
    void touch_input_began(space s, const std::vector<touch>& touches) 
    {
        spwnr_->spawn();
    }
    
private:
    configuration&  config_;
    boost::shared_ptr<spawner> spwnr_;
    std::vector< boost::shared_ptr<base_object> >   objects_;
};

#endif // __SCENE_H__
