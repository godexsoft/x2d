//
//  second.cpp
//  x2d
//
//  Created by Alex Kremer on 2/24/13.
//  Copyright (c) 2013 godexsoft. All rights reserved.
//

#include "second.h"
#include "first.h"

void second_scene::on_transition_to()
{
    mover_.reset();
}

void second_scene::on_transition_from()
{
    // disconnect all input
    disconnect_touch_input(WORLD_SPACE);

    set_lifetime(0.3f);
}

void second_scene::touch_input_ended(space s, const std::vector<touch>& touches)
{
    if(mover_.is_completed())
    {
        get_app().switch_to( get_config().create_scene<first_scene>("scenes.first") );
    }
}

void second_scene::update(const clock_info& clock)
{
    if(!mover_.is_completed())
    {
        mover_.update(clock.delta_time);
        bg_->camera_space_position(glm::vec2(x_offset_, 0.5f));        
    }
}
