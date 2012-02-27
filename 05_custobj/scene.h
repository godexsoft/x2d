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
#include "zone.h"
#include "custom_obj.h"
#include "fps_counter.h"

#include <vector>

class scene 
{
public:
    scene(kernel& k, configuration& conf)
    : config_(conf)
    , ctx_players_(boost::shared_ptr<context>( new context() ) )
    , zone_(boost::shared_ptr<zone>( new rectangular_zone(k, rect(-160, -240, 50, 480)) ) )
    {        
        objects_.push_back( config_.create_object("objects.player") );
        objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
    }
    
private:
    configuration&  config_;
    
    boost::shared_ptr<context> ctx_players_;
    boost::shared_ptr<zone> zone_;
    std::vector< boost::shared_ptr<base_object> >   objects_;
};

#endif // __SCENE_H__
