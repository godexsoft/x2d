//
//  scene.h
//  x2d
//
//  Created by Alex Kremer on 1/31/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifndef __SCENE_H__
#define __SCENE_H__

#include "app_framework.h"

class scene
: public base_object
{
public:
    scene(app_framework& app)
    : base_object(app.get_kernel())
    , app_(app)
    , spwnr_(app_.get_config().create_sys_object<spawner>("spawner"))
    {
        connect_touch_input(WORLD_SPACE);
        app_.get_config().create_object("floor_obj");
    }
    
    void touch_input_began(space s, const std::vector<touch>& touches)
    {
        spwnr_->spawn();
    }
    
private:
    app_framework&  app_;
    boost::shared_ptr<spawner> spwnr_;
};

#endif // __SCENE_H__
