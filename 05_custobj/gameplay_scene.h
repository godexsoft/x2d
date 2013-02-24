//
//  scene.h
//  x2d
//
//  Created by Alex Kremer on 1/31/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
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
#include "zone.h"
#include "custom_obj.h"
#include "fps_counter.h"

#include <vector>

class scene 
{
public:
    scene(kernel& k, configuration& conf)
    : config_(conf)
    , zone_first_( config_.get_object<zone>("zones.first") )
    , zone_finish_level_( config_.get_object<zone>("zones.finish_level") )
    {        
        // set triggers for zones
        zone_first_->set_trigger(boost::bind(&scene::zone_first_act, this, _1));
        zone_finish_level_->set_trigger(boost::bind(&scene::zone_finish_level_act, this, _1));
        
        objects_.push_back( config_.create_object("objects.player") );
        objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
    }
    
private:
    
    void zone_first_act(object& obj)
    {
        LOG("First zone action");
    }

    void zone_finish_level_act(object& obj)
    {
        LOG("Level finish action");
    }

    
    configuration&  config_;
    
    boost::shared_ptr<zone> zone_first_;
    boost::shared_ptr<zone> zone_finish_level_;
    std::vector< boost::shared_ptr<base_object> >   objects_;
};

#endif // __SCENE_H__
