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
    void crash();
    void finish();
    
protected:
    virtual void update(const clock_info& clock);
    void touch_input_began(space s, const std::vector<touch>& touches);
    void touch_input_ended(space s, const std::vector<touch>& touches);
    void accelerometer_input(const glm::vec3& acceleration);
    
private:
    bool        finished_;
    bool        thrust_;
    float       fuel_;
    float       accel_;

    float       rotation_velocity_;
    glm::vec2   velocity_;
    glm::vec2   orientation_;
};

#endif // __PLAYER_H__
