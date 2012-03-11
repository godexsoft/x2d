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
#include "font.h"
#include "fps_counter.h"
#include "custom_obj.h"

#include <vector>

class scene 
{
public:
    scene(kernel& k, configuration& conf)
    : config_(conf)
    {
//        objects_.push_back( boost::shared_ptr<base_object>( new custom_obj(k, config_.get_resman(), 
//            config_.get_object<font>("fonts.sony_sketch_stroked") ) ) );
        objects_.push_back( config_.create_object("str") );
        objects_.push_back( config_.create_object("str2") );
        objects_.push_back( config_.create_object("str3") );
        objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
    }
    
private:
    configuration&  config_;
    std::vector< boost::shared_ptr<base_object> >   objects_;
};

#endif // __SCENE_H__
