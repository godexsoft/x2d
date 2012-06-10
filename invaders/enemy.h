//
//  enemy.h
//  invaders
//
//  Created by Alex Kremer on 6/10/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <deque>

#include "object.h"
#include "configuration.h"
#include "sound.h"
#include "interpolator.h"

class enemy
: public object
{
public:
    enemy(kernel& k, configuration& conf, const object_traits& t);
    
protected:
    virtual void update(const clock_info& clock);
    
private:
    configuration& config_;
    
    float                  y_from_;
    float                  y_to_;
    float                  roundtrip_time_;
    
    float                  y_;
    quadratic_interpolator inter_;
};

#endif // __ENEMY_H__
