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
    scene(kernel& k, configuration& conf)
    : config_(conf)
    {        
        objects_.push_back( config_.create_object("objects.player") );
        
        but1_ = config_.create_object("objects.hud.button1");
        but2_ = but1_->child_by_name("button2");
        but3_ = but2_->child_by_name("button3");

        but1_->set_on_input_began( boost::bind(&scene::inp1_began, this, _1) );
        but2_->set_on_input_began( boost::bind(&scene::inp2_began, this, _1) );
        but3_->set_on_input_began( boost::bind(&scene::inp3_began, this, _1) );
        
        but1_->set_on_input_ended( boost::bind(&scene::inp1_ended, this, _1) );
        but2_->set_on_input_ended( boost::bind(&scene::inp2_ended, this, _1) );
        but3_->set_on_input_ended( boost::bind(&scene::inp3_ended, this, _1) );
        
//        objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
    }
    
private:
    
    bool inp1_began(const glm::vec2& location);
    bool inp2_began(const glm::vec2& location);
    bool inp3_began(const glm::vec2& location);
    bool inp1_ended(const glm::vec2& location);
    bool inp2_ended(const glm::vec2& location);
    bool inp3_ended(const glm::vec2& location);
    
    configuration&  config_;
    boost::shared_ptr<object> buttons_;
    boost::shared_ptr<object> but1_, but2_, but3_;
    std::vector< boost::shared_ptr<base_object> >   objects_;
};

#endif // __SCENE_H__
