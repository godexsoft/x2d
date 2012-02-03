//
//  scene.h
//  x2d
//
//  Created by Alex Kremer on 1/31/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifndef __SCENE_H__
#define __SCENE_H__

#include "clock.h"
#include "timer.h"
#include "interpolator.h"
#include "graphics_engine.h"
#include "texture.h"
#include "liverpool_manager.h"
#include "resource_manager.h"
#include "sprite.h"
#include "configuration.h"
#include "custom_obj.h"
#include "fps_counter.h"

#include <vector>

class scene 
{
public:
    scene(kernel& k, liverpool_manager lvp_man)
    : res_man_(lvp_man)
    , config_(k, res_man_, "res/main.xml")
    {        
        objects_.push_back( boost::shared_ptr<base_object>( new custom_obj(k, config_, "animations.player") ) );
        objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
    }
    
private:
    resource_manager          res_man_;
    configuration             config_;

    std::vector< boost::shared_ptr<base_object> >   objects_;
};

#endif // __SCENE_H__
