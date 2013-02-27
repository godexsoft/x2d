//
//  second.h
//  x2d
//
//  Created by Alex Kremer on 2/24/13.
//  Copyright (c) 2013 godexsoft. All rights reserved.
//

#ifndef __SECOND_SCENE_H__
#define __SECOND_SCENE_H__

#include "app_framework.h"
#include "interpolator.h"

class second_scene
: public scene
{
public:
    second_scene(configuration& c, const object_traits& t)
    : scene(c, t)
    , x_offset_(0.0f)
    , mover_(x_offset_, 1.5f, 0.5f, 0.3f)
    {
        bg_ = child_by_name("background");
        
        connect_touch_input(WORLD_SPACE);
        connect_update();
    }
    
    void touch_input_ended(space s, const std::vector<touch>& touches);
    
    void on_transition_to();
    void on_transition_from();
    
    void update(const clock_info& clock);

private:
    boost::shared_ptr<object> bg_;
    
    float x_offset_;
    linear_interpolator mover_;
};

#endif // __SECOND_SCENE_H__
