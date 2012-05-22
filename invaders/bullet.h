//
//  bullet.h
//  invaders
//
//  Created by Alex Kremer on 5/21/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __BULLET_H__
#define __BULLET_H__

#include "object.h"
#include "configuration.h"
#include "sound.h"

class bullet 
: public object
{
public:
    bullet(kernel& k, configuration& conf, const object_traits& t);
    
protected:
    virtual void update(const clock_info& clock);
    
private:
    configuration& config_;
};

#endif // __BULLET_H__
