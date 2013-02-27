//
//  first.cpp
//  x2d
//
//  Created by Alex Kremer on 2/24/13.
//  Copyright (c) 2013 godexsoft. All rights reserved.
//

#include "first.h"
#include "second.h"

void first_scene::on_transition_to()
{
    // will be:
    // apply_fx( get_config().get_object<fx>("effects.slide_in") );
    mover_.reset();
}

void first_scene::on_transition_from()
{
    // disconnect all input
    disconnect_touch_input(WORLD_SPACE);

    // will be (assuming that effect runs in 0.3 seconds):
    // apply_fx( get_config().get_object<fx>("effects.slide_out") );

    set_lifetime(0.3f);
}

void first_scene::touch_input_ended(space s, const std::vector<touch>& touches)
{
    if(mover_.is_completed())
    {
        get_app().switch_to( get_config().create_scene<second_scene>("scenes.second") );
    }
}

void first_scene::update(const clock_info& clock)
{
    if(!mover_.is_completed())
    {
        // FIXME: this is just for demo.
        // It's a very bad way to achieve this effect as it's
        // recalculating the position everytime we update.
        // Instead it should be done using an effect which just changes OpenGL matrix while rendering.
        mover_.update(clock.delta_time);
        bg_->camera_space_position(glm::vec2(x_offset_, 0.5f));
    }
    
    scene::update(clock);
}
