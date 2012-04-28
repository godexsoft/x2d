//
//  player.cpp
//  x2d
//
//  Created by Alex Kremer on 4/14/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "player.h"
#include "log.h"

player::player(kernel& k, configuration& conf, const object_traits& t)
: object(k, conf, t)
, config_(conf)
, finished_( false )
, thrust_( false )
, max_fuel_( conf.get_value<float>("objects.player.initial_fuel") )
, fuel_( max_fuel_ )
, accel_( 0.0f )
, rotation_velocity_( 0.0f )
, velocity_( conf.get_value<glm::vec2>("objects.player.initial_velocity") )
, thrust_sfx_( conf.get_object<sfx>("sounds.thrust") )
{   
    connect_update();
    connect_render(WORLD_SPACE);
    
    // get child object links
    main_thrust_ = child_by_name("ship_main_thrust");
    left_thrust_ = child_by_name("ship_left_thrust");
    right_thrust_ = child_by_name("ship_right_thrust");
}

bool player::landing_allowed()
{
    return ( (velocity_.y >= -config_.get_value<float>("settings.max_velocity")) 
            && (fabsf(rotation()) <= config_.get_value<float>("settings.max_rotation")) );
}

void player::finish()
{
    // make sure to turn off engines
    main_thrust_->visible(false);
    left_thrust_->visible(false);
    right_thrust_->visible(false);
    
    finished_ = true;    
}

float player::fuel_percent() const
{
    return fuel_/max_fuel_;
}

void player::main_thrust(bool enabled)
{
    if(enabled && thrust_ != enabled)
    {
        thrust_sfx_->play();
    }
    
    thrust_ = enabled;
}

void player::lateral_thrust(float accel)
{
    accel_ = accel;
}

void player::update(const clock_info& clock)
{
    if(finished_)
        return;
    
    object::update(clock);
    bool thrust = thrust_;
    
    glm::vec2 force(0.0f, -config_.get_value<float>("settings.mass") 
        * config_.get_value<float>("settings.gravity"));
    position( position() + glm::vec3(velocity_.x * clock.delta_time, 
                                     velocity_.y * clock.delta_time, 0.0f) );
    
    size fr = camera_->frustum();
    
    if(position().x < -fr.width/2)
    {
        position( glm::vec2(fr.width + position().x, position().y) );
    }
    else if(position().x > fr.width/2)
    {
        position( glm::vec2(0.0f - position().x, position().y) );
    }
    
    if(position().y > fr.height/2) 
    {
        position( glm::vec2(position().x, fr.height/2) );
        velocity_.y = 0.0f;
    }
    
    rotation( rotation() + rotation_velocity_ * clock.delta_time );
    
    orientation_.x = sinf( glm::radians(rotation()) );
    orientation_.y = cosf( glm::radians(rotation()) );    
    
    if(thrust && fuel_ > 0.0f)
    {
        force.x = -orientation_.x * config_.get_value<float>("settings.main_thrust");
        force.y = orientation_.y  * config_.get_value<float>("settings.main_thrust");
        
        main_thrust_->visible(true);
    }
    else
    {
        main_thrust_->visible(false);
    }
    
    velocity_.x += force.x / config_.get_value<float>("settings.mass") * clock.delta_time;
    velocity_.y += force.y / config_.get_value<float>("settings.mass") * clock.delta_time;
    
    if( fabsf(accel_) >= config_.get_value<float>("settings.lateral_th") ) 
    {
        thrust = true;
        velocity_.x += (accel_ > 0.0f ? accel_ - config_.get_value<float>("settings.lateral_th") 
                        : accel_ + config_.get_value<float>("settings.lateral_th")) 
                        * config_.get_value<float>("settings.lateral_speed");
        rotation_velocity_ = -(accel_ > 0.0f ? accel_ - config_.get_value<float>("settings.lateral_th") 
                               : accel_ + config_.get_value<float>("settings.lateral_th")) 
                                * config_.get_value<float>("settings.rotation_speed");

        // display thrust only if we have fuel left
        if(fuel_ > 0.0f)
        {
            // toggle thrust display
            if( accel_ > 0.0f )
            {
                left_thrust_->visible(true);
            } 
            else
            {
                right_thrust_->visible(true);
            }
        }
    } 
    else
    {
        left_thrust_->visible(false);
        right_thrust_->visible(false);
    }
    
    if(thrust)
    {
        fuel_ -= clock.delta_time;
    }
    
    if(fuel_ < 0.0f)
    {
        fuel_ = 0.0f;
    }
}