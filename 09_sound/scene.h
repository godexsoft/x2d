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
#include "custom_obj.h"
#include "sound.h"

#include <vector>

class scene 
{
public:
    scene(kernel& k, configuration& conf)
    : config_(conf)
    , bg_track_( config_.get_object<music>("sounds.bg") )
    {
        objects_.push_back( boost::shared_ptr<base_object>( new custom_obj(k, config_) ) );
        objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
        
        bg_track_->play();
    }
    
private:
    configuration&  config_;
    std::vector< boost::shared_ptr<base_object> >   objects_;
    boost::shared_ptr<music>                        bg_track_;
};

#endif // __SCENE_H__
