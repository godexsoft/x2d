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
#include "sound.h"
#include "interpolator.h"
#include "platform.h"

class custom_obj 
: public base_object
{
public:
    custom_obj(kernel& k, configuration& conf)
    : base_object(k)
    , sfx_( conf.get_object<sfx>("sounds.swoosh") )
    , mus_(1.0f)
    , ci_(mus_, 0.0f, 0.3f, 1.0f, 0.0f, 3.0f)
    {    
        connect_update();
        connect_touch_input(SCREEN_SPACE);
    }
    
private:
    boost::shared_ptr<sfx>  sfx_;
    
    float mus_;
    cubic_interpolator ci_;
    
protected:

    void update(const clock_info& ci)
    {
        ci_.update(ci.delta_time);
        sound_engine::instance().music_volume(mus_);
    }
    
    void touch_input_began(space s, const std::vector<touch>& touches)
    {
        sfx_->play();
    }
};

#endif // __X2D_CUSTOM_OBJ_H__
