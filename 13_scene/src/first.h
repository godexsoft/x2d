//
//  first.h
//  x2d
//
//  Created by Alex Kremer on 2/24/13.
//  Copyright (c) 2013 godexsoft. All rights reserved.
//

#ifndef __FIRST_SCENE_H__
#define __FIRST_SCENE_H__

#include "app_framework.h"
#include "interpolator.h"

class first_scene
: public scene
{
public:
    first_scene(configuration& c)
    : scene(c)
    , x_offset_(0.0f)
    , mover_(x_offset_, -0.5f, 0.5f, 0.3f)
    {
        bg_ = get_config().create_object("scenes.first.background");
        add_child(bg_);
        
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

#endif // __FIRST_SCENE_H__
