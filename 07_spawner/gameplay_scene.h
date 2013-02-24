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
: public base_object
{
public:
    scene(kernel& k, configuration& conf)
    : base_object(k)
    , config_(conf)
    {        
        player_ = config_.create_object("objects.player");        

        connect_touch_input(WORLD_SPACE);
    }
    
protected:
    void touch_input_began(space s, const std::vector<touch>& touches) 
    {
        glm::vec2 pos;
        point p = touches.at(0).location();
        pos.x = p.x;
        pos.y = p.y;
        
        player_->position(pos);
    }
    
    void touch_input_moved(space s, const std::vector<touch>& touches) 
    {
        glm::vec2 pos;
        point p = touches.at(0).location();
        pos.x = p.x;
        pos.y = p.y;
        
        player_->position(pos);
    }
    
private:
    configuration&  config_;
    boost::shared_ptr< object > player_;
};

#endif // __SCENE_H__
