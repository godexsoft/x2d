//
//  scene.cpp
//  invaders
//
//  Created by Alex Kremer on 2/21/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "scene.h"
#include "game.h"

#include <boost/lexical_cast.hpp>

scene::scene(kernel& k, configuration& conf, game& g)
: base_object(k)
, config_(conf)
, game_(g)
, player_( conf.create_object<player>("objects.player") )
//, hud_( conf.create_object<hud>("objects.ui.hud") )
//, platform_width_( conf.get_value<float>("objects.scenary.platform.width") )
//, finished_(false)
//, start_sfx_( conf.get_object<sfx>("sounds.start") )
//, success_sfx_( conf.get_object<sfx>("sounds.success") )
//, fail_sfx_( conf.get_object<sfx>("sounds.fail") )
{       
    connect_update();    
    connect_touch_input(WORLD_SPACE);
    connect_accelerometer_input();

    // get all zones and configure them
    off_screen_zone_ = config_.get_object<zone>("zones.off_screen");
    off_screen_zone_->set_trigger( boost::bind(&scene::on_off_screen, this, _1) );
    off_screen_zone_->enable();

//    win_banner_ = config_.create_object("objects.ui.win");
//    score_holder_ = win_banner_->child_by_name("score_holder");
//    lose_banner_ = config_.create_object("objects.ui.lose");
    
#ifdef DEBUG
    // for debug only
    objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
#endif
    
//    start_sfx_->play();
}

void scene::update(const clock_info& clock) 
{ 
//    hud_->update_fuel( player_->fuel_percent() );
//    hud_->update_indicators( player_->is_good_orientation(), player_->is_good_velocity() );
}

void scene::on_off_screen(object& obj)
{
    LOG("Bullet hit zone 0x%X!", &obj);
    player_->release(obj);
}

void scene::touch_input_began(space s, const std::vector<touch>& touches) 
{
//    if(finished_)
//    {
//        // restart the game
//        game_.start_game();
//        return;
//    }
//    
    player_->fire(true);
}

void scene::touch_input_ended(space s, const std::vector<touch>& touches) 
{
    player_->fire(false);
}

void scene::accelerometer_input(const glm::vec3& acceleration)
{
    player_->move(acceleration.x);
}