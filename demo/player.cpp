//
//  player.cpp
//  x2d
//
//  Created by Alex Kremer on 4/14/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "player.h"
#include "log.h"

#define kMainThrust 10000
#define kMass	 80   
#define kGravity 40
#define kLateralThrustThreshold 0.1
#define kLateralSpeed	10
#define kRotationSpeed	100   

#define kMaxVelocity				75   //Pixels/s
#define kMaxRotation				8   //Degrees

player::player(kernel& k, configuration& conf, const object_traits& t)
: object(k, conf, t)
, finished_( false )
, thrust_( false )
, max_fuel_( conf.get_value<float>("objects.player.initial_fuel") )
, fuel_( max_fuel_ )
, accel_( 0.0f )
, rotation_velocity_( 0.0f )
, velocity_( conf.get_value<glm::vec2>("objects.player.initial_velocity") )
{   
    connect_update();
    connect_render(WORLD_SPACE);
    
    // get child object links
    main_thrust_ = child_by_name("ship_main_thrust");
    left_thrust_ = child_by_name("ship_left_thrust");
    right_thrust_ = child_by_name("ship_right_thrust");
    
    // probably invisible initially :)
    main_thrust_->visible(thrust_);
    left_thrust_->visible(thrust_);
    right_thrust_->visible(thrust_);
}

bool player::landing_allowed()
{
    return ( (velocity_.y >= -kMaxVelocity) 
            && (fabsf(rotation()) <= kMaxRotation) );
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
    
    glm::vec2 force(0.0f, -kMass * kGravity);
    position( position() + glm::vec3(velocity_.x * clock.delta_time, 
                                     velocity_.y * clock.delta_time, 0.0f) );
    
    if(position().x < -160.0f)
    {
        position( glm::vec2(320.0f + position().x, position().y) );
    }
    else if(position().x > 160.0f)
    {
        position( glm::vec2(0.0f - position().x, position().y) );
    }
    
    if(position().y > 240.0f) 
    {
        position( glm::vec2(position().x, 240.0f) );
        velocity_.y = 0.0f;
    }
    
    rotation( rotation() + rotation_velocity_ * clock.delta_time );
    
    orientation_.x = sinf( glm::radians(rotation()) );
    orientation_.y = cosf( glm::radians(rotation()) );    
    
    if(thrust && fuel_ > 0.0f)
    {
        force.x = -orientation_.x * kMainThrust;
        force.y = orientation_.y  * kMainThrust;
        
        main_thrust_->visible(true);
    }
    else
    {
        main_thrust_->visible(false);
    }
    
    velocity_.x += force.x / kMass * clock.delta_time;
    velocity_.y += force.y / kMass * clock.delta_time;
    
    if( fabsf(accel_) >= kLateralThrustThreshold ) 
    {
        thrust = true;
        velocity_.x += (accel_ > 0.0f ? accel_ - kLateralThrustThreshold 
                        : accel_ + kLateralThrustThreshold) * kLateralSpeed;
        rotation_velocity_ = -(accel_ > 0.0f ? accel_ - kLateralThrustThreshold 
                               : accel_ + kLateralThrustThreshold) * kRotationSpeed;

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