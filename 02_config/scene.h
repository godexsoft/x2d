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
    , config_(res_man_, "res/main.xml")
    {        
        objects_.push_back( boost::shared_ptr<base_object>( new custom_obj(k, config_, "txtr.house", vector_2d(100, 100), vector_2d(100, 200)) ) );
        objects_.push_back( boost::shared_ptr<base_object>( new custom_obj(k, config_, "txtr.cafe", vector_2d(10, 10), vector_2d(200, 140)) ) );
        objects_.push_back( boost::shared_ptr<base_object>( new custom_obj(k, config_, "txtr.tree", vector_2d(40, 60), vector_2d(30, 40)) ) );
        objects_.push_back( boost::shared_ptr<base_object>( new custom_obj(k, config_, "txtr.lantern", vector_2d(20, 80), vector_2d(120, 150)) ) );
        objects_.push_back( boost::shared_ptr<base_object>( new custom_obj(k, config_, "txtr.fence", vector_2d(10, 110), vector_2d(310, 310)) ) );
        objects_.push_back( boost::shared_ptr<base_object>( new custom_obj(k, config_, "txtr.cloud1", vector_2d(120, 130), vector_2d(300, 240)) ) );
        objects_.push_back( boost::shared_ptr<base_object>( new custom_obj(k, config_, "txtr.cloud2", vector_2d(100, 110), vector_2d(10, 40)) ) );
        objects_.push_back( boost::shared_ptr<base_object>( new custom_obj(k, config_, "txtr.cloud3", vector_2d(200, 300), vector_2d(210, 40)) ) );        
        
        objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
    }
    
private:
    resource_manager          res_man_;
    configuration             config_;

    std::vector< boost::shared_ptr<base_object> >   objects_;
};

#endif // __SCENE_H__
