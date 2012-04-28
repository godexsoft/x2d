//
//  scene.cpp
//  x2d
//
//  Created by Alex Kremer on 1/31/12.
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
, platform_width_( conf.get_value<float>("objects.scenary.platform.width") )
, finished_(false)
, start_sfx_( conf.get_object<sfx>("sounds.start") )
, success_sfx_( conf.get_object<sfx>("sounds.success") )
, fail_sfx_( conf.get_object<sfx>("sounds.fail") )
{       
    connect_update();    
    connect_touch_input(WORLD_SPACE);
    connect_accelerometer_input();

    camera_ = config_.get_object<camera>("camera");
        
    // get all zones and configure them
    crash_zone_ = config_.get_object<zone>("zones.crash_or_land");
    crash_zone_->set_trigger( boost::bind(&scene::on_land, this, _1) );
    
    // enable the zone as it's shared (get_object) 
    // and can be disabled from prev. game 
    crash_zone_->enable();
    
    // create all scenary objects
    objects_.push_back( config_.create_object("objects.background") );
    objects_.push_back( config_.create_object("objects.scenary.moon_surface") );
    explosion_ = config_.create_object("objects.scenary.explosion"); 
    platform_ = config_.create_object("objects.scenary.platform");
    fuel_bar_ = config_.create_object("objects.ui.fuel_bar");
    fuel_bar_filling_ = fuel_bar_->child_by_name("fuel_bar_filling");
    
    win_banner_ = config_.create_object("objects.ui.win");
    score_holder_ = win_banner_->child_by_name("score_holder");
    lose_banner_ = config_.create_object("objects.ui.lose");
        
#ifdef DEBUG
    // for debug only
    objects_.push_back( boost::shared_ptr<base_object>( new fps_counter(k) ) );
#endif
    
    start_sfx_->play();
}

void scene::update(const clock_info& clock) 
{ 
    // update size of fuel filling
    fuel_bar_filling_->box( size(1.0f, player_->fuel_percent()) );
}

void scene::on_land(object& obj)
{
    // thanks, we are done with the crash zone
    crash_zone_->disable();
    
    float player_x = player_->world_position().x;
    float platform_x = platform_->world_position().x;
    
    bool on_platform = false;
    if( fabsf(player_x - platform_x) < platform_width_/2.0f )
    {
        on_platform = true;
    }
    
    player_->finish();
    finished_ = true;
    
    if( player_->landing_allowed() && on_platform )
    {
        // landed
        score_holder_->text( score_holder_->text() + 
            boost::lexical_cast<std::string>( 
                floor(player_->fuel_percent() * 100) ) );

        win_banner_->visible(true);        
        success_sfx_->play();
    } 
    else
    {     
        // draw explosion. position is initially an offset
        explosion_->position( player_->world_position() + explosion_->position() );
        explosion_->visible(true);
        
        lose_banner_->visible(true);        
        fail_sfx_->play();
    }
}

void scene::touch_input_began(space s, const std::vector<touch>& touches) 
{
    if(finished_)
    {
        // restart the game
        game_.start_game();
        return;
    }
    
    player_->main_thrust(true);
}

void scene::touch_input_ended(space s, const std::vector<touch>& touches) 
{
    player_->main_thrust(false);
}

void scene::accelerometer_input(const glm::vec3& acceleration)
{
    player_->lateral_thrust(acceleration.x);
}