//
//  custom_obj.h
//  x2d
//
//  Created by Alex Kremer on 2/1/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_CUSTOM_OBJ_H__
#define __X2D_CUSTOM_OBJ_H__

#include "base_object.h"
#include "configuration.h"
#include "platform.h"

class custom_obj 
: public base_object
{
public:
    custom_obj(kernel& k, configuration& conf)
    : base_object(k)
    {    
        connect_update();
        connect_touch_input(SCREEN_SPACE);
    }
    
protected:

    void update(const clock_info& ci)
    {
    }
    
    void touch_input_began(space s, const std::vector<touch>& touches)
    {
    }
};

#endif // __X2D_CUSTOM_OBJ_H__
