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
#include "spawner.h"
#include "fps_counter.h"

#include <vector>

class scene 
{
public:
    scene(kernel& k, configuration& conf)
    : config_(conf)
//    , spawner_(k, config_)
    {        
        objects_.push_back( config_.create_object("objects.player") );
        objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
        
//        spawner_.wave_delay(2.0f);
//        spawner_.start();
    }
    
private:
    configuration&  config_;
    std::vector< boost::shared_ptr<base_object> >   objects_;
    
//    spawner         spawner_;
};

#endif // __SCENE_H__
