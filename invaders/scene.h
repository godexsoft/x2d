//
//  scene.h
//  invaders
//
//  Created by Alex Kremer on 5/21/12.
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
#include "fps_counter.h"

#include <vector>

#include "player.h"
//#include "hud.h"

class game;

class scene 
: public base_object
{
public:
    scene(kernel& k, configuration& conf, game& g);
    
protected:
    void update(const clock_info& clock);
    void touch_input_began(space s, const std::vector<touch>& touches);
    void touch_input_ended(space s, const std::vector<touch>& touches);
    void accelerometer_input(const glm::vec3& acceleration);

private:
    
    void on_off_screen(object& obj);
    
    configuration   &config_;
    game            &game_;
//    boost::shared_ptr<camera> camera_;
    
    std::vector< boost::shared_ptr<base_object> >   objects_;
    boost::shared_ptr<zone> off_screen_zone_;
//    
//    boost::shared_ptr<object> explosion_;
//    boost::shared_ptr<object> platform_;
//        
//    // win/lose banners
//    boost::shared_ptr<object> win_banner_;
//    boost::shared_ptr<object> score_holder_;
//    boost::shared_ptr<object> lose_banner_;
//    
    boost::shared_ptr<player> player_;    
//    boost::shared_ptr<hud>    hud_;
//
//    float platform_width_;    
//    bool finished_;
//    
//    // sounds
//    boost::shared_ptr<sfx>  start_sfx_;
//    boost::shared_ptr<sfx>  success_sfx_;
//    boost::shared_ptr<sfx>  fail_sfx_;
};

#endif // __SCENE_H__
