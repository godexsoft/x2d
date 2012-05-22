//
//  player.h
//  invaders
//
//  Created by Alex Kremer on 5/21/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <deque>

#include "object.h"
#include "configuration.h"
#include "sound.h"

class bullet;

class player 
: public object
{
public:
    player(kernel& k, configuration& conf, const object_traits& t);
    
    void fire(bool f);
    void move(float accel);

protected:
    virtual void update(const clock_info& clock);
    
private:
    configuration& config_;
    
    bool    fire_;
    double  last_fire_;
};

#endif // __PLAYER_H__
