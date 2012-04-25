//
//  player.h
//  x2d
//
//  Created by Alex Kremer on 4/14/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "object.h"
#include "configuration.h"

class player 
: public object
{
public:
    player(kernel& k, configuration& conf, const object_traits& t);
    
    bool landing_allowed();
    void finish();
    
    float fuel_percent() const;
    
    void main_thrust(bool enabled);
    void lateral_thrust(float accel);
    
protected:
    virtual void update(const clock_info& clock);
    
private:
    bool        finished_;
    bool        thrust_;
    
    float       max_fuel_;
    float       fuel_;
    float       accel_;

    float       rotation_velocity_;
    glm::vec2   velocity_;
    glm::vec2   orientation_;
    
    // child objects
    boost::shared_ptr<object> main_thrust_;
    boost::shared_ptr<object> left_thrust_;
    boost::shared_ptr<object> right_thrust_;
};

#endif // __PLAYER_H__
